/*
    Copyright 2014 Felspar Co Ltd. http://support.felspar.com/
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <fost/core>
#include <jni.h>


namespace fostlib {


    /// Fetch the JNI environment for the current thread
    JNIEnv *get_environment();


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



namespace com {
    namespace felspar {
        namespace android {


            /// Global reference to the com.felspar.android.Asset class
            extern jclass Asset;


        }
    }
}

