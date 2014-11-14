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
