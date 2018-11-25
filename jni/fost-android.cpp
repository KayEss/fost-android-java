/*
    Copyright 2014-2017 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <android/log.h>


namespace {
    JavaVM *g_JavaVM = nullptr;
    const fostlib::module c_jni(fostlib::c_fost, "JNI");

    auto g_onload() {
        static std::mutex g_onload_mutex;
        static std::vector<fostlib::jni_onload*> g_onload;
        return std::make_pair(std::unique_lock<std::mutex>(g_onload_mutex), &g_onload);
    }
}
jclass com::felspar::android::Asset = nullptr;


const fostlib::module fostlib::c_android(c_jni, "android");


fostlib::jni_onload::jni_onload(std::function<void(JNIEnv *)> onload)
: lambda(onload) {
    auto loader = g_onload();
    loader.second->push_back(this);
    __android_log_print(3, "Fost Android jni_onload", "Added onload handler");
}


extern "C" JNIEXPORT
jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_JavaVM = vm;
    JNIEnv *env = fostlib::get_environment();
    com::felspar::android::Asset = reinterpret_cast<jclass>(
        env->NewGlobalRef(
            env->FindClass("com/felspar/android/Asset")));
    auto loaders = g_onload();
    auto number = std::to_string(loaders.second->size()); // Costs us an allocation, but at least it's always safe
    __android_log_print(4, "Fost Android OnLoad", "Loaders to run %s", number.c_str());
    for ( const auto &onload : *loaders.second ) {
        __android_log_print(3, "Fost Android OnLoad", "Running loader");
        (*onload)(env);
    }
    return JNI_VERSION_1_6;
}


JNIEnv *fostlib::get_environment() {
    JNIEnv *env = nullptr;
    g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if ( env == nullptr ) {
        __android_log_print(ANDROID_LOG_WARN, "fost-android",
            "No environment available yet -- attaching thread");
        int status = g_JavaVM->AttachCurrentThread(&env, nullptr);
        if ( status != JNI_OK ) {
            __android_log_print(ANDROID_LOG_ERROR, "fost-android",
                "AttachCurrentThread failed %d", status);
            return nullptr;
        }
    }
    return env;
}

