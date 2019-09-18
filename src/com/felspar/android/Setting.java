/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


package com.felspar.android;

public class Setting {
    public native static boolean fromJSON(String domain, String json);

    public native static boolean fromString(String domain, String section, String name, String value);

    private native static String readString(String section, String name);

    public static String readStringFrom(String section, String name) throws Exception {
        String value = readString(section, name);
        if(value == null)
            throw new Exception("An error occured.");
        return value;
    }

    public static String readStringFrom(String section, String name, String defaultValue) {
        try {
            return readStringFrom(section, name);
        }catch (Exception e){
            return defaultValue;
        }
    }
}
