/**
    Copyright 2014-2018 Felspar Co Ltd. http://support.felspar.com/

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include "fost-android.hpp"
#include <fost/internet>
#include <fost/http.server.hpp>
#include <fost/log>
#include <fost/urlhandler>


namespace {
    fostlib::worker g_server;
    std::shared_ptr<fostlib::detail::future_result<void>> g_running;

    std::atomic<bool> g_terminate{false};
}


extern "C" JNIEXPORT void JNICALL
Java_com_felspar_android_WebServer_start(
    JNIEnv *env, jobject self
) {
    /// Start the web server and set the termination condition
    g_running = g_server([]() -> bool {
        fostlib::http::server server(fostlib::host(0), 2555);
        server(fostlib::urlhandler::service, []() {
            return g_terminate.load();
        });
    });
}


extern "C" JNIEXPORT void JNICALL
Java_com_felspar_android_WebServer_stop(
    JNIEnv *env, jobject self
) {
    /// Tell the server to stop
    g_terminate = true;
    /// Tickle the port so it notices
    fostlib::network_connection tickle(fostlib::host("localhost"), 2555);
    g_running->wait();
}

