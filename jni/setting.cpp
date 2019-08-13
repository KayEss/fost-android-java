/*
    Copyright 2014-2016 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <fost/log>


namespace {
    const fostlib::module c_module(fostlib::c_android, "setting");

    const fostlib::setting<fostlib::json> c_logging(
            "fost-android/setting.cpp",
            "Logging", "Configuration",
            fostlib::json(), true);
    std::unique_ptr<fostlib::log::global_sink_configuration> g_sinks;

    std::list<std::unique_ptr<fostlib::settings>> g_settings;
    std::list<std::unique_ptr<fostlib::setting<fostlib::json>>> g_setting;
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
        fostlib::log::info(c_module)
                ("domain", domain)
                ("settings", parsed);
        return true;
    } catch (fostlib::exceptions::exception &e) {
        fostlib::log::info(c_module)
                ("exception", e.what())
                ("data", e.data());
        return false;
    }
}


extern "C" JNIEXPORT bool JNICALL
Java_com_felspar_android_Setting_fromString(
        JNIEnv *env, jobject self, jstring jdomain, jstring jsection, jstring jname, jstring jvalue
) {
    try {
        fostlib::string
                domain = fostlib::jni_cast<fostlib::string>(env, jdomain),
                section = fostlib::jni_cast<fostlib::string>(env, jsection),
                name = fostlib::jni_cast<fostlib::string>(env, jname),
                value = fostlib::jni_cast<fostlib::string>(env, jvalue);

        g_setting.push_back(
                std::unique_ptr<fostlib::setting<fostlib::json>>(
                        new fostlib::setting<fostlib::json>(domain, section, name, value)));
        fostlib::log::info(c_module)
                ("domain", domain)
                ("section", section)
                ("name", name)
                ("value", value);
        return true;
    } catch (fostlib::exceptions::exception &e) {
        fostlib::log::error(c_module)
                ("exception", e.what())
                ("data", e.data());
        return false;
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_felspar_android_Setting_readString(
        JNIEnv *env, jobject self, jstring jsection, jstring jname
) {
    try {
        fostlib::string
                section = fostlib::jni_cast<fostlib::string>(env, jsection),
                name = fostlib::jni_cast<fostlib::string>(env, jname);
        if (not fostlib::setting<fostlib::string>::exists(section, name))
            return env->NewStringUTF("");
        return env->NewStringUTF(
                fostlib::setting<fostlib::string>::value(section, name).shrink_to_fit());
    } catch (fostlib::exceptions::exception &e) {
        fostlib::log::info(c_module)
                ("exception", e.what())
                ("data", e.data());
        return env->NewStringUTF("");
    }
}

