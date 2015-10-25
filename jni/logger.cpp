/*
    Copyright 2014-2015 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <android/log.h>
#include <fost/log>


namespace {
    class logger {
    public:
        logger(const fostlib::json &) {}

        bool operator () (const fostlib::log::message &m) const {
            int level = 3; // DEBUG
            if ( m.level() >= 0x4000u ) {
                level = 6; // ERROR
            } else if ( m.level() >= 0x1000u ) {
                level = 5; // WARN
            } else if ( m.level() >= 0x400u ) {
                level = 4; // INFO
            }
            __android_log_print(level,
                    m.module().as_string().c_str(),
                    "%s", fostlib::json::unparse(
                        fostlib::coerce<fostlib::json>(m)["body"], false).c_str());
            return true;
        }
    };
}

namespace {
    const fostlib::log::global_sink<logger> android_log_print("android_log_print");
}
