/*
    Copyright 2014-2015 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <android/log.h>


namespace {
    JavaVM *g_JavaVM = nullptr;
    const fostlib::module c_jni(fostlib::c_fost, "JNI");
}
jclass com::felspar::android::Asset = nullptr;


const fostlib::module fostlib::c_android(c_jni, "android");


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    g_JavaVM = vm;
    JNIEnv *env = fostlib::get_environment();
    com::felspar::android::Asset = reinterpret_cast<jclass>(
        env->NewGlobalRef(
            env->FindClass("com/felspar/android/Asset")));
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
