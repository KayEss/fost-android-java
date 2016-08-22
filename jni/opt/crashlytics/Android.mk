LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(FANDK)/opts.mk

LOCAL_MODULE := fost-opt-crashlytics

LOCAL_C_INCLUDES += \
    $(FAJ)

LOCAL_SRC_FILES := \
     logger.cpp

LOCAL_WHOLE_STATIC_LIBRARIES := \
    boost-filesystem \
    boost-system \
    fost-android \
    fost-core

include $(BUILD_STATIC_LIBRARY)
