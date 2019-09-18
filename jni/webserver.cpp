/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-android.hpp"
#include <fost/internet>
#include <fost/http.server.hpp>
#include <fost/log>
#include <fost/urlhandler>


namespace {
    fostlib::http::server *g_webserver = nullptr;
    fostlib::worker g_server;
    std::shared_ptr<fostlib::detail::future_result<void>> g_running;
}


extern "C" JNIEXPORT void JNICALL
Java_com_felspar_android_WebServer_start(
    JNIEnv *env, jobject self
) {
    /// Start the web server and set the termination condition
    g_running = g_server([]() {
        fostlib::http::server server(fostlib::host(0), 2555);
        g_webserver = &server;
        server(fostlib::urlhandler::service, []() -> bool {
            return false;
        });
    });
}


extern "C" JNIEXPORT void JNICALL
Java_com_felspar_android_WebServer_stop(
    JNIEnv *env, jobject self
) {
    if(g_webserver) {
        g_webserver->stop_server();
        g_webserver = nullptr;
     }
}

