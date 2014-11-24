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
            const fostlib::json &config, const fostlib::string &path,
            fostlib::http::server::request &req,
            const fostlib::host &
        ) const {
            fostlib::mime::mime_headers headers;
            return std::make_pair(new fostlib::text_body(
                        "<html><head><title>Asset</title></head>"
                        "<body><p>" + path + "</p></body></html>",
                    headers, "text/html"),
                200);
        }
    } c_asset_view;
}

