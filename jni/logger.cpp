/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-android.hpp"
#include <android/log.h>
#include <fost/insert>
#include <fost/log>


namespace {


    class logger {
      public:
        logger(const fostlib::json &) {}

        bool operator()(const fostlib::log::message &m) const {
            int level = 3; // DEBUG
            if (m.level() >= 0x4000u) {
                level = 6; // ERROR
            } else if (m.level() >= 0x1000u) {
                level = 5; // WARN
            } else if (m.level() >= 0x400u) {
                level = 4; // INFO
            }
            __android_log_print(
                    level, m.module().as_string().shrink_to_fit(), "%s",
                    fostlib::json::unparse(
                            fostlib::coerce<fostlib::json>(m)["body"], false)
                            .shrink_to_fit());
            return true;
        }
    };

    const fostlib::log::global_sink<logger>
            android_log_print("android_log_print");


    const fostlib::module c_javascript("JavaScript");


}


extern "C" JNIEXPORT void JNICALL Java_com_felspar_android_Logger_log(
        JNIEnv *env, jobject self, int priority, jstring jtag, jstring jmessage) {
}


extern "C" JNIEXPORT void JNICALL Java_com_felspar_android_Logger_logjs(
        JNIEnv *env,
        jobject self,
        int priority,
        jstring level,
        jstring jmessage,
        int line,
        jstring jsource) {
    const fostlib::string message =
            fostlib::jni_cast<fostlib::string>(env, jmessage);

    fostlib::json body;
    fostlib::insert(
            body, "body",
            fostlib::json::parse(message, fostlib::json(message)));
    fostlib::insert(body, "from", "line", line);
    fostlib::insert(
            body, "from", "source",
            fostlib::jni_cast<fostlib::string>(env, jsource));

    fostlib::log::log(fostlib::log::message(
            c_javascript, priority,
            fostlib::jni_cast<fostlib::string>(env, level).u8string_transition(),
            body));
}
