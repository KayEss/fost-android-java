/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-android.hpp"
#include <fost/urlhandler>


/**
 * This code still fetches the assets via JNI calls back up to Java.
 * There seems to be some problem with this and threading that can
 * cause the Java side to crash. It might be better to use the native
 * asset APIs and see if they allow us to switch to the threaded
 * web servers. This implementation will go in
 * [`assets.cpp`](./assets.cpp).
 */


namespace {
    /**
     * This function fetches an asset from the asset manager, or returns
     * a nullptr
     */
    f5::shared_buffer<jbyte> assetdata(fostlib::string path) {
        JNIEnv *env = fostlib::get_environment();
        jmethodID jloadBytes(env->GetStaticMethodID(
                com::felspar::android::Asset, "loadBytes",
                "(Ljava/lang/String;)[B"));
        jstring asset(reinterpret_cast<jstring>(
                env->NewLocalRef(env->NewStringUTF(path.shrink_to_fit()))));
        jobject load_result(env->CallStaticObjectMethod(
                com::felspar::android::Asset, jloadBytes, asset));
        /// It's a bit confusing to have a pointer to this in `jbytes`...
        jbyteArray *jbytes(reinterpret_cast<jbyteArray *>(&load_result));
        if (*jbytes == nullptr) {
            return f5::shared_buffer<jbyte>();
        } else {
            std::size_t length(env->GetArrayLength(*jbytes));
            f5::shared_buffer<jbyte> buffer(length);
            env->GetByteArrayRegion(*jbytes, 0, length, buffer.begin());
            return buffer;
        }
    }

    /**
     * Returns the correct response for the provided data.
     */
    std::pair<boost::shared_ptr<fostlib::mime>, int> assetresponse(
            f5::shared_buffer<jbyte> bytes, const fostlib::string &path) {
        if (bytes.data()) {
            return std::make_pair(
                    boost::shared_ptr<fostlib::mime>(new fostlib::binary_body(
                            bytes.begin(), bytes.end(),
                            fostlib::mime::mime_headers(),
                            fostlib::urlhandler::mime_type(
                                    fostlib::coerce<boost::filesystem::wpath>(
                                            path)))),
                    200);
        } else {
            return std::make_pair(
                    boost::shared_ptr<fostlib::mime>(new fostlib::text_body(
                            "<html><head><title>404</title></head>"
                            "<body><h1>404</h1><pre>"
                            + path + "</pre></body></html>")),
                    404);
        }
    }


    /**
     * View that assumes a directory structure is available in the asset
     * manager. Handles working out the correct path to find the asset at.
     */
    const class assets : public fostlib::urlhandler::view {
      public:
        assets() : view("fost.android.assets") {}

        std::pair<boost::shared_ptr<fostlib::mime>, int> operator()(
                const fostlib::json &config,
                const fostlib::string &requested_path,
                fostlib::http::server::request &req,
                const fostlib::host &) const {
            fostlib::string path(
                    fostlib::coerce<fostlib::string>(config["asset"])
                    + requested_path);
            if (requested_path.empty() || requested_path.endswith("/")) {
                /// **TODO** This filename should be read from the configuration
                path += "index.html";
            }
            return assetresponse(assetdata(path), path);
        }
    } c_assets_view;


    /**
     * View that fetches a single asset from the asset manager and returns it as
     * a response.
     */
    const class asset : public fostlib::urlhandler::view {
      public:
        asset() : view("fost.andriod.asset") {}

        std::pair<boost::shared_ptr<fostlib::mime>, int> operator()(
                const fostlib::json &config,
                const fostlib::string &,
                fostlib::http::server::request &req,
                const fostlib::host &) const {
            fostlib::string assetpath =
                    fostlib::coerce<fostlib::string>(config);
            return assetresponse(assetdata(assetpath), assetpath);
        };
    } c_asset_view;
}
