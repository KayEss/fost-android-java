/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <fost/log>
#include <android/log.h>


namespace {
    std::list<std::unique_ptr<fostlib::settings>> g_settings;
}


extern "C" JNIEXPORT bool JNICALL
Java_com_felspar_android_Setting_fromJSON(
    JNIEnv *env, jobject self, jstring domain, jstring j_string
) {
    try {
        fostlib::string cppstring(fostlib::jni_cast<fostlib::string>(env, j_string));
        fostlib::json parsed(fostlib::json::parse(cppstring));
        g_settings.push_back(
            std::unique_ptr<fostlib::settings>(
                new fostlib::settings(
                    fostlib::jni_cast<fostlib::string>(env, domain), parsed)));
        __android_log_print(ANDROID_LOG_INFO,
            "JNI.com.felspar.android.Setting.fromJSON",
            "JSON = %s", cppstring.c_str());
        fostlib::stringstream ss;
        fostlib::setting<fostlib::json>::printAllOn(ss);
        __android_log_print(ANDROID_LOG_INFO,
            "JNI.com.felspar.android.Setting.fromJSON",
            "%s", ss.str().c_str());
        return true;
    } catch ( fostlib::exceptions::exception &e ) {
        __android_log_print(ANDROID_LOG_ERROR,
            "JNI.com.felspar.android.Setting.fromJSON",
            "Exception.what() = %s", e.what());
        return false;
    }
}

