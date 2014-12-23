FAJ := $(call my-dir)

include $(CLEAR_VARS)

include $(FANDK)/opts.mk

LOCAL_PATH := $(FAJ)
LOCAL_MODULE := fost-android

LOCAL_SRC_FILES := \
    fost-android.cpp \
    logger.cpp \
    setting.cpp \
    view-assets.cpp \
    webserver.cpp
LOCAL_SHARED_LIBRARIES := \
    boost-filesystem \
    boost-system \
    fost-core \
    fost-inet \
    fost-rproxy \
    fost-urlhandler
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
