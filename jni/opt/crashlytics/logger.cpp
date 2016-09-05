/*
    Copyright 2016 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost-android.hpp>
#include <fost/log>


namespace {


    jclass crashlytics = nullptr;
    jmethodID crashlytics_log = 0;
    jmethodID crashlytics_logcat = 0;


    class logcat {
    public:
        logcat(const fostlib::json &) {}

        bool operator () (const fostlib::log::message &m) const {
            JNIEnv *env = fostlib::get_environment();

            int level = 3; // DEBUG
            if ( m.level() >= 0x4000u ) {
                level = 6; // ERROR
            } else if ( m.level() >= 0x1000u ) {
                level = 5; // WARN
            } else if ( m.level() >= 0x400u ) {
                level = 4; // INFO
            }
            jobject tag = env->NewStringUTF(m.module().as_string().c_str());
            jobject message = env->NewStringUTF(fostlib::json::unparse(m.body(), false).c_str());
            env->CallStaticVoidMethod(crashlytics, crashlytics_logcat, level, tag, message);
            return true;
        }
    };
    const fostlib::log::global_sink<logcat> c_logcat("crashlytics.logcat");


    const fostlib::jni_onload g_loader([](JNIEnv *env) {
        crashlytics = reinterpret_cast<jclass>(
            env->NewGlobalRef(
                env->FindClass("com/crashlytics/android/Crashlytics")));
        crashlytics_log = env->GetStaticMethodID(
            crashlytics, "log", "(Ljava/lang/String;)V");
        crashlytics_logcat = env->GetStaticMethodID(
            crashlytics, "log", "(ILjava/lang/String;Ljava/lang/String;)V");
    });


}

