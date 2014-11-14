/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/core>
#include <jni.h>


namespace fostlib {


    /// Global with the JVM environment in it -- probably shouldn't be used
    extern JavaVM *g_JavaVM;


    /// Cast jstring to various C++ string types
    template<typename S>
    S jni_cast(JNIEnv *env, jstring jstr) {
        std::shared_ptr<const char> cloc(
            env->GetStringUTFChars(jstr, NULL),
            [=](const char *ptr) {
                env->ReleaseStringUTFChars(jstr, ptr);
            });
        return S(cloc.get());
    }


}

