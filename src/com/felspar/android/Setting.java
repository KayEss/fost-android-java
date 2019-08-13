/*
    Copyright 2014-2016 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


package com.felspar.android;

public class Setting {
    public native static boolean fromJSON(String domain, String json);

    public native static boolean fromString(String domain, String section, String name, String value);

    private native static String readString(String section, String name);

    public static String readStringFrom(String section, String name) throws Exception {
        String value = readString(section, name);
        if(!value.startsWith("Bearer "))
            throw new Exception("An error occured. " + value);
        return value;
    }
}
