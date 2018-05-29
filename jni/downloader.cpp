/**
    Copyright 2014-2018 Felspar Co Ltd. <https://support.felspar.com/>

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
*/


#include <fost/aws>
#include <fost/insert>
#include <fost/timer>
#include <fost/config.hpp>


using namespace fostlib;


namespace {


    json s3_into_local_db(const json &config) {
        json result;
        aws::s3::bucket bucket(
            coerce<ascii_printable_string>(
                coerce<nullable<string>>(config["bucket"]).value()));
        timer taken;
        string content(bucket.get(
            coerce<boost::filesystem::wpath>(
                config["location"])));
        json db_content(json::parse(content));
        insert(result, "downloaded", db_content);
        insert(result, "time-taken", taken.seconds());
        return result;
    }


}

