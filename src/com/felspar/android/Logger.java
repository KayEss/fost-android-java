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
import android.webkit.ConsoleMessage;
import android.webkit.WebChromeClient;

public class Logger extends WebChromeClient {
    public static void log(int priority, String tag, String message) {
        Log.println(priority, tag, message);
    }

    public static void log(int priority, String tag, Throwable exception) {
        final StringWriter sw = new StringWriter();
        final PrintWriter pw = new PrintWriter(sw, true);
        exception.printStackTrace(pw);
        log(priority, tag, sw.getBuffer().toString());
    }

    public boolean onConsoleMessage(ConsoleMessage cm) {
    	int priority = Log.ERROR;
    	if ( cm.messageLevel() == ConsoleMessage.MessageLevel.DEBUG ) {
    	    priority = Log.DEBUG;
    	} else if ( cm.messageLevel() == ConsoleMessage.MessageLevel.LOG ||
    			cm.messageLevel() == ConsoleMessage.MessageLevel.TIP ) {
    		priority = Log.INFO;
    	} else if ( cm.messageLevel() == ConsoleMessage.MessageLevel.WARNING ) {
    		priority = Log.WARN;
    	}
        Logger.log(priority, "JavaScript",
            cm.message() + " -- From line " +
            cm.lineNumber() + " of " +
            cm.sourceId());
        return true;
    }
}
