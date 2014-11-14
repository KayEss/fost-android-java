APP_ABI := armeabi-v7a armeabi x86 # crypto++ won't build for mips
APP_PLATFORM := android-8

APP_STL := gnustl_shared
APP_CPPFLAGS += -fexceptions -frtti -std=c++11
