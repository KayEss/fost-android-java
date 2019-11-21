/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include "fost-android.hpp"
#include "messaging.hpp"
#include <fost/insert>


namespace {
    /// The registered application that we are sending messages to
    jobject g_application;

    jclass handler;
    jmethodID handler_obtainMessage;
    jmethodID handler_sendMessage;

    const fostlib::jni_onload g_loader([](JNIEnv *env) {
        handler = reinterpret_cast<jclass>(
                env->NewGlobalRef(env->FindClass("android/os/Handler")));
        handler_obtainMessage = env->GetMethodID(
                handler, "obtainMessage",
                "(ILjava/lang/Object;)Landroid/os/Message;");
        handler_sendMessage = env->GetMethodID(
                handler, "sendMessage", "(Landroid/os/Message;)Z");
    });
}


bool fostlib::send_message(json const msg) {
    JNIEnv *env = get_environment();

    jobject payload = env->NewGlobalRef(
                env->NewStringUTF(fostlib::json::unparse(msg, false).shrink_to_fit()));

    jobject message = env->NewGlobalRef(env->CallObjectMethod(
            g_application, handler_obtainMessage, 1, payload));

    bool const success = env->CallBooleanMethod(
        g_application, handler_sendMessage, message);

    return success;
}


/// For naming see <https://stackoverflow.com/a/49560732/115526>
extern "C" JNIEXPORT void JNICALL Java_com_felspar_android_Messaging_registerApplication(
    JNIEnv *env, jobject self, jobject app) {
    g_application = env->NewGlobalRef(app);

    fostlib::json msg;
    insert(msg, "display", f5::u8view{"Application has been registered"});
    send_message(msg);
}
