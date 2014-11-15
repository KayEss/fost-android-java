/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


package com.felspar.android;

public class Setting {
    public native static boolean fromJSON(String domain, String json);

    static {
        // Note that the order of library loading is important
        System.loadLibrary("gnustl_shared");
        System.loadLibrary("boost-system");
        System.loadLibrary("boost-thread");
        System.loadLibrary("boost-date_time");
        System.loadLibrary("boost-filesystem");
        System.loadLibrary("cryptopp");
        System.loadLibrary("crypto-openssl");
        System.loadLibrary("ssl-openssl");
        System.loadLibrary("fost-core");
        System.loadLibrary("fost-datetime");
        System.loadLibrary("fost-crypto");
        System.loadLibrary("fost-inet");
        System.loadLibrary("fost-schema");
        System.loadLibrary("fost-jsondb");
        System.loadLibrary("fost-urlhandler");
        System.loadLibrary("fost-beanbag-views");
        System.loadLibrary("fost-s3");
        System.loadLibrary("fost-rproxy");
        System.loadLibrary("fost-android");
    }
}
