FAJ := $(call my-dir)

include $(CLEAR_VARS)

include $(FANDK)/opts.mk

LOCAL_PATH := $(FAJ)
LOCAL_MODULE := fost-android

LOCAL_SRC_FILES := \
    assets.cpp \
    fost-android.cpp \
    logger.cpp \
    setting.cpp \
    view-assets.cpp \
    webserver.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := \
    boost-filesystem \
    boost-system \
    fost-core \
    fost-inet \
    fost-urlhandler

include $(BUILD_STATIC_LIBRARY)
