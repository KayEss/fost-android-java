/*
    Copyright 2016 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


package com.felspar.android;

import android.app.NativeActivity;
import android.os.Bundle;

public class NDKActivity extends NativeActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Asset.setAssets(getAssets());
        super.onCreate(savedInstanceState);
    }
}
