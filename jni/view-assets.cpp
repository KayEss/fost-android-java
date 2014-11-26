/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include "fost-android.hpp"
#include <fost/urlhandler>


namespace {
    const class assets : public fostlib::urlhandler::view {
    public:
        assets()
        : view("fost.android.assets") {
        }

        std::pair<boost::shared_ptr<fostlib::mime>, int> operator () (
            const fostlib::json &config, const fostlib::string &requested_path,
            fostlib::http::server::request &req,
            const fostlib::host &
        ) const {
            fostlib::string path(
                fostlib::coerce<fostlib::string>(config["asset"]) + requested_path);
            if ( requested_path.empty() || requested_path.endswith("/") ) {
                path += "index.html";
            }
            JNIEnv *env = fostlib::get_environment();
            jmethodID jloadBytes(env->GetStaticMethodID(
                com::felspar::android::Asset,
                "loadBytes", "(Ljava/lang/String;)[B"));
            jstring asset(reinterpret_cast<jstring>(
                env->NewLocalRef(env->NewStringUTF(path.c_str()))));
            jobject load_result(env->CallStaticObjectMethod(
                    com::felspar::android::Asset, jloadBytes, asset));
            jbyteArray *jbytes(reinterpret_cast<jbyteArray*>(&load_result));
            if ( *jbytes == nullptr ) {
                return std::make_pair(
                    new fostlib::text_body(
                        "<html><head><title>404</title></head>"
                        "<body><h1>404</h1><pre>" + path + "</pre></body></html>"),
                    404);
            } else {
                std::size_t length(env->GetArrayLength(*jbytes));
                std::shared_ptr<signed char> bytes(
                    env->GetByteArrayElements(*jbytes, nullptr),
                    [env, jbytes](signed char *ptr) {
                        env->ReleaseByteArrayElements(*jbytes, ptr, 0);
                    });
                return std::make_pair(
                    new fostlib::binary_body(
                        bytes.get(), bytes.get() + length,
                        fostlib::mime::mime_headers(),
                        fostlib::urlhandler::mime_type(
                            fostlib::coerce<boost::filesystem::wpath>(path))),
                    200);
            }
        }
    } c_asset_view;
}

