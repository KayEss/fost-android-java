/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


package com.felspar.android;

import java.io.PrintWriter;
import java.io.StringWriter;

import android.util.Log;

public class Logger {
    public static void log(int priority, String tag, String message) {
        Log.println(priority, tag, message);
    }
    public static void log(int priority, String tag, Throwable exception) {
        final StringWriter sw = new StringWriter();
        final PrintWriter pw = new PrintWriter(sw, true);
        exception.printStackTrace(pw);
        log(priority, tag, sw.getBuffer().toString());
    }
}
