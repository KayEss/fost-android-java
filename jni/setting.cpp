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
    const fostlib::setting<fostlib::json> c_logging(
        "fost-android/setting.cpp",
        "Logging", "Configuration",
        fostlib::json(), true);
    std::unique_ptr<fostlib::log::global_sink_configuration> g_sinks;

    std::list<std::unique_ptr<fostlib::settings>> g_settings;
}


extern "C" JNIEXPORT bool JNICALL
Java_com_felspar_android_Setting_fromJSON(
    JNIEnv *env, jobject self, jstring jdomain, jstring j_string
) {
    try {
        fostlib::string cppstring(fostlib::jni_cast<fostlib::string>(env, j_string)),
            domain(fostlib::jni_cast<fostlib::string>(env, jdomain));
        fostlib::json parsed(fostlib::json::parse(cppstring));
        g_settings.push_back(
            std::unique_ptr<fostlib::settings>(
                new fostlib::settings(domain, parsed)));
        g_sinks.reset(new fostlib::log::global_sink_configuration(c_logging.value()));
        fostlib::log::info().module("JNI.com.felspar.android.Setting")
            ("domain", domain)
            ("settings", parsed);
        return true;
    } catch ( fostlib::exceptions::exception &e ) {
        __android_log_print(ANDROID_LOG_ERROR,
            "JNI.com.felspar.android.Setting",
            "Exception.what() = %s", e.what());
        return false;
    }
}

