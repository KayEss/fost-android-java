/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


package com.felspar.android;

import java.io.PrintWriter;
import java.io.StringWriter;

import android.util.Log;
import android.webkit.ConsoleMessage;
import android.webkit.WebChromeClient;

public class Logger extends WebChromeClient {
    public static void log(int priority, String tag, Throwable exception) {
        final StringWriter sw = new StringWriter();
        final PrintWriter pw = new PrintWriter(sw, true);
        exception.printStackTrace(pw);
        log(priority, tag, sw.getBuffer().toString());
    }

    public boolean onConsoleMessage(ConsoleMessage cm) {
        int priority = 0x10000; // Unknown levels are logged at 'critical'\
        String name = "critical";
        switch (cm.messageLevel()) {
            case DEBUG:
                priority = 0x0100;
                name = "debug";
                break;
            case ERROR:
                priority = 0x4000;
                name = "error";
                break;
            case LOG:
                priority = 0x0400;
                name = "log";
                break;
            case TIP:
                priority = 0x0600; // Not a standard Fost level
                name = "tip";
                break;
            case WARNING:
                priority = 0x1000;
                name = "warning";
                break;
        }
        Logger.logjs(priority, name, cm.message(), cm.lineNumber(), cm.sourceId());
        return true;
    }

    public native static void log(int priority, String tag, String message);
    private native static void logjs(int priority, String level_name, String message, int line, String sourceId);
}
