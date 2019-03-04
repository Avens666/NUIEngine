# The ARMv7 is significanly faster due to the use of the hardware FPU
APP_ABI := armeabi-v7a
APP_PLATFORM := android-9

NDK_TOOLCHAIN_VERSION := 4.8

APP_STL := gnustl_static
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti

#APP_CPPFLAGS += -frtti -fexceptions
APP_CPPFLAGS += -std=c++11