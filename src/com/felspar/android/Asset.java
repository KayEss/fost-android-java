/*
    Copyright 2014-2016 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


package com.felspar.android;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.res.AssetManager;
import android.util.Log;

public class Asset {
    private static final String TAG = "com.felspar.android.Assets";
    private static AssetManager assets;

    public static void setAssets(AssetManager assetManager) {
        assets = assetManager;
    }

    public static String loadString(AssetManager assetManager, String fromAssetPath) throws IOException {
        setAssets(assetManager);
        Logger.log(Log.DEBUG, TAG, fromAssetPath);
        InputStream in = assetManager.open(fromAssetPath);
        return new String(readFully(in), "UTF-8");
    }
    
    public static void copyFile(AssetManager assetManager, String fromAssetPath, File to) throws IOException {
        setAssets(assetManager);
        Logger.log(Log.DEBUG, TAG, fromAssetPath);
        InputStream in = assetManager.open(fromAssetPath);
        OutputStream out = new FileOutputStream(to);
        copyFile(in, out);
        out.close();
    }

    public static byte[] loadBytes(String fromAssetPath) {
        Logger.log(Log.DEBUG, TAG, fromAssetPath);
        try {
            if ( assets == null ) {
                Logger.log(Log.ERROR, TAG, "Must call a function that takes an AssetManager first");
                return null;
            }
            InputStream in = assets.open(fromAssetPath);
            return readFully(in);
        } catch ( IOException e ) {
            Logger.log(Log.ERROR, TAG, e);
            return null;
        }
    }

    private static byte[] readFully(InputStream inputStream) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        copyFile(inputStream, baos);
        return baos.toByteArray();
    }
    

    static private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int length = 0;
        while ( (length = in.read(buffer)) != -1 ) {
            out.write(buffer, 0, length);
        }
    }
}
