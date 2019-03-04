LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#LOCAL_CPP_EXTENSION := .cpp
LOCAL_MODULE    := NUI

LOCAL_C_INCLUDES := $(BOOST_INCLUDE_PATH) \
					$(SKIA_INCLUDE_PATH) \
					$(SKIA_INCLUDE_PATH)/config \
					$(SKIA_INCLUDE_PATH)/core \
					$(SKIA_INCLUDE_PATH)/utils \
					$(SKIA_INCLUDE_PATH)/effects \
					$(SKIA_INCLUDE_PATH)/images \
					$(SKIA_INCLUDE_PATH)/lazy \
					$(LOCAL_PATH)/Include \
					$(LOCAL_PATH)/Include/baseclass \
					$(LOCAL_PATH)/Include/graphic \
					$(LOCAL_PATH)/Include/nuiengine \
					$(TINMYXML_INCLUDE_PATH)


#LOCAL_CFLAGS := -D_UNICODE	-DUNICODE	-DANDROID_NDK -DKOTEI_LINUX -DNDEBUG -DSIGSLOT_USE_POSIX_THREADS \
#					-DSK_BUILD_FOR_ANDROID \
#				-fsigned-char -fpermissive -w

LOCAL_CFLAGS := $(PUBLIC_CFLAFS)


BASE_ClASS_SOURCE := BaseClass
GRAPHIC_SOURCE := graphic/Src
CORE_SOURCE := core

LOCAL_SRC_FILES += 	$(BASE_ClASS_SOURCE)/CharSet.cpp \
					$(BASE_ClASS_SOURCE)/error.cpp \
					$(BASE_ClASS_SOURCE)/FilePath.cpp \
					$(BASE_ClASS_SOURCE)/KBufferReader.cpp \
					$(BASE_ClASS_SOURCE)/KFile.cpp \
					$(BASE_ClASS_SOURCE)/KGlobalFunction.cpp \
					$(BASE_ClASS_SOURCE)/KNFile.cpp \
					$(BASE_ClASS_SOURCE)/KProfile.cpp \
					$(GRAPHIC_SOURCE)/REEffectHelper.cpp \
					$(GRAPHIC_SOURCE)/RERect.cpp \
					$(GRAPHIC_SOURCE)/RESurface.cpp \
					$(GRAPHIC_SOURCE)/RESurfaceFactory.cpp \
					$(GRAPHIC_SOURCE)/REWindow.cpp \
					$(CORE_SOURCE)/AnimationThread.cpp \
					$(CORE_SOURCE)/DataSync.cpp \
					$(CORE_SOURCE)/Easing.cpp \
					$(CORE_SOURCE)/KConfigManager.cpp \
					$(CORE_SOURCE)/KEasingCurve.cpp \
					$(CORE_SOURCE)/KFontManager.cpp \
					$(CORE_SOURCE)/KGalleryView.cpp \
					$(CORE_SOURCE)/KGestureDetector.cpp \
					$(CORE_SOURCE)/KLog.cpp \
					$(CORE_SOURCE)/KMessage.cpp \
					$(CORE_SOURCE)/KScreen.cpp \
					$(CORE_SOURCE)/KStringManager.cpp \
					$(CORE_SOURCE)/KSurfaceManager.cpp \
					$(CORE_SOURCE)/KTimer.cpp \
					$(CORE_SOURCE)/mainforlinux.cpp \
					$(CORE_SOURCE)/mainforqt.cpp \
					$(CORE_SOURCE)/mainforwindows.cpp \
					$(CORE_SOURCE)/message.cpp \
					$(CORE_SOURCE)/messageforlinux.cpp \
					$(CORE_SOURCE)/NUIAnimation.cpp \
					$(CORE_SOURCE)/NUIInstance.cpp \
					$(CORE_SOURCE)/utility.cpp \
					$(CORE_SOURCE)/drawable/K9PatchImageDrawable.cpp \
					$(CORE_SOURCE)/drawable/KAreaBlurDrawable.cpp \
					$(CORE_SOURCE)/drawable/KBitmapTextDrawable.cpp \
					$(CORE_SOURCE)/drawable/KColorDrawable.cpp \
					$(CORE_SOURCE)/drawable/KColorTextDrawable.cpp \
					$(CORE_SOURCE)/drawable/KDrawable.cpp \
					$(CORE_SOURCE)/drawable/KImageDrawable.cpp \
					$(CORE_SOURCE)/drawable/KPathEffectDrawable.cpp \
					$(CORE_SOURCE)/drawable/KShapeDrawable.cpp \
					$(CORE_SOURCE)/drawable/KSvgAnalyzing.cpp \
					$(CORE_SOURCE)/drawable/KTextDrawable.cpp \
					$(CORE_SOURCE)/drawable/KTextMultiLineDrawable.cpp \
					$(CORE_SOURCE)/drawable/KWaveEffectDrawable.cpp \
					$(CORE_SOURCE)/views/KButtonView.cpp \
					$(CORE_SOURCE)/views/KCheckboxView.cpp \
					$(CORE_SOURCE)/views/KEditView.cpp \
					$(CORE_SOURCE)/views/KEditView_android.cpp \
					$(CORE_SOURCE)/views/KListView.cpp \
					$(CORE_SOURCE)/views/KMenuView.cpp \
					$(CORE_SOURCE)/views/KMoveableView.cpp \
					$(CORE_SOURCE)/views/KProcessView.cpp \
					$(CORE_SOURCE)/views/KSlideView.cpp \
					$(CORE_SOURCE)/views/KStaticTextView.cpp \
					$(CORE_SOURCE)/views/KSwitchButtonView.cpp \
					$(CORE_SOURCE)/views/KTextView.cpp \
					$(CORE_SOURCE)/views/KTipView.cpp \
					$(CORE_SOURCE)/views/KView.cpp \
					$(CORE_SOURCE)/views/KViewGroup.cpp \
					$(CORE_SOURCE)/views/KWaveEffectView.cpp \
					$(TINMYXML_INCLUDE_PATH)/Src/tinystr.cpp \
					$(TINMYXML_INCLUDE_PATH)/Src/tinyxml.cpp \
					$(TINMYXML_INCLUDE_PATH)/Src/tinyxmlerror.cpp \
					$(TINMYXML_INCLUDE_PATH)/Src/tinyxmlparser.cpp 
					
					
include $(BUILD_STATIC_LIBRARY)
