/**
    Copyright 2014-2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <fost/core>
#include <jni.h>


namespace fostlib {


    extern const module c_android;


    /// Fetch the JNI environment for the current thread
    JNIEnv *get_environment();


    /// Cast jstring to various C++ string types
    template<typename S>
    S jni_cast(JNIEnv *env, jstring jstr) {
        std::shared_ptr<const char> cloc(
                env->GetStringUTFChars(jstr, NULL), [=](const char *ptr) {
                    env->ReleaseStringUTFChars(jstr, ptr);
                });
        return S(cloc.get());
    }


    /// Add a lambda that is to be called when the application loads
    class jni_onload {
        std::function<void(JNIEnv *)> lambda;

      public:
        jni_onload(std::function<void(JNIEnv *)> onload);

        void operator()(JNIEnv *e) const { lambda(e); }
    };


}


namespace com {
    namespace felspar {
        namespace android {


            /// Global reference to the com.felspar.android.Asset class
            extern jclass Asset;


        }
    }
}
