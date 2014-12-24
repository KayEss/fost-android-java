# fost-android-java #

Includes Java and JNI functionality that extends the `fost-android-ndk` project to allow certain features to be accessible from Java code.

## Use in a project ##

Typically you'll want to add this as a submodule somewhere suitable in your project. It's probably simplest to then soft link in the Java and native code.

    cd src/com
    ln -s ../../fost-android-java/src/com/felspar felspar
    cd ../../jni
    ln -s ../../fost-android-java/jni fost-android-java

You need to have `fost-android-ndk` included in your `Android.mk` before including the make file for the JNI code. E.g. the beginning of your make file should now look something like this:

    JNI := $(call my-dir)

    include $(JNI)/fost-android-ndk/Android.mk
    include $(JNI)/fost-android-java/Android.mk

    LOCAL_PATH := $(JNI)

    include $(CLEAR_VARS)

It would then go on with your local module definition and code.
