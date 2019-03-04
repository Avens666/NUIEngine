LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CPP_EXTENSION := .cpp

LOCAL_MODULE    := HMI  

NUI_PATH = $(LOCAL_PATH)/../nuiengine

LOCAL_C_INCLUDES := $(NUI_PATH)/include \
					$(NUI_PATH)/include/baseclass \
					$(NUI_PATH)/include/graphic \
					$(NUI_PATH)/include/nuiengine \
					$(TINMYXML_INCLUDE_PATH) \
					$(SKIA_INCLUDE_PATH) \
					$(SKIA_INCLUDE_PATH)/config \
					$(SKIA_INCLUDE_PATH)/core \
					$(SKIA_INCLUDE_PATH)/utils \
					$(SKIA_INCLUDE_PATH)/effects \
					$(SKIA_INCLUDE_PATH)/images \
					$(SKIA_INCLUDE_PATH)/lazy \
					$(BOOST_INCLUDE_PATH)

LOCAL_CFLAGS := $(PUBLIC_CFLAFS)

CODE_SOURCE := HelloNUI

LOCAL_SRC_FILES +=	$(CODE_SOURCE)/NuiMainView.cpp \
					$(CODE_SOURCE)/NUIApp.cpp \
					$(CODE_SOURCE)/NUIApp_android.cpp
					
					

include $(BUILD_STATIC_LIBRARY)
