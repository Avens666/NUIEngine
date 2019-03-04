#include <math.h>
#include <jni.h>
#include <android/bitmap.h>
#include <vector>
#include <string>
#include "NE_pubhead.h"
#include "com_nui_NUICore.h"
#include "JNIRegistStaticMethod.h"

using namespace std;
//	数据文件目录, 由java传入
#define DEFAULT_DATA_PATH "/sdcard/nui/"
bool g_bLogEnable = true;
JavaVM *gJavaVM;

// Java向C++注册的回调列表ID, 需要和Java代码一致
enum
{
	CB_FLIP = 0,
	CB_SEND_JAVAMSG,
	CB_SIZE
};

static JNIRegistStaticMethod* s_pCallbackFlip = NULL;
static JNIRegistStaticMethod* s_pCallbackSendJavaMsg = NULL;

JNIRegistStaticMethod* CALLBACKS [CB_SIZE] = {0};

int StartNUIEngine(const char* szAppDataPath, const char* savPath, unsigned char* pJavaSurfaceBuf,
		int iSurfaceWidth, int iSurfaceHeight, int iDeviceWidth, int iDeviceHeight);
int InitInstance(AndroidBitmapInfo* pInfo, void* pBuf);
void SendAndroidMessage(int message, int wParam, int lParam, int iPointCount, int iBufferSize, unsigned char* pBuf);
void AndroidResize(void* dst, int w, int h);
void LockJavaBitmap();
void UnlockJavaBitmap();

string jstringtostring_utf8(JNIEnv *env, const jstring& jstr)
{
	const char *cstr = env->GetStringUTFChars(jstr, 0);
	string str = cstr;
	env->ReleaseStringUTFChars(jstr, cstr);
	return str;
}

wstring jstringtowstring(JNIEnv *env, const jstring& jstr)
{
	jsize iLen = env->GetStringLength(jstr);
	jboolean bIsCopy = 0;
	//	get java unicode16
	const WORD* pWbuffer = env->GetStringChars(jstr, &bIsCopy);
	if (pWbuffer == NULL)
	{
		return (wchar_t*)("");
	}
	const int LEN = 512;
	wchar_t wszBuffer[LEN] = {0};
	if(iLen >= LEN)
	{
		LOGI("Get_JNStringBuf iLEN = %d > %d", iLen, LEN);
		iLen = LEN - 1;
	}
	//	unicode 16 -> unicode 32
	for (int i = 0; i < iLen; i++)
	{
		wszBuffer[i] = pWbuffer[i];
	}
	env->ReleaseStringChars(jstr, pWbuffer);
	return wszBuffer;
}

JNIEXPORT void JNICALL Java_com_nui_NUICore_drawIntoBitmap(JNIEnv* env,
		jclass thiz, jobject dstBitmap, jint iDeviceWidth, jint iDeviceHeight, jlong elapsedTime)
{
	AndroidBitmapInfo dstInfo;
	void* dstPixels;
	AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
	AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
	AndroidBitmap_unlockPixels(env, dstBitmap);
}
/**
 * Draws something into the given bitmap
 * @param  env
 * @param  thiz
 * @param  dstBitmap   The bitmap to place the results of skia into
 * @param  elapsedTime The number of milliseconds since the app was started
 */
//extern "C"
JNIEXPORT void JNICALL Java_com_nui_NUICore_initializeEngine
  (JNIEnv * env, jclass thiz, jstring jstrPath, jstring jsavPath, jobject dstBitmap, jint iDeviceWidth, jint iDeviceHeight)
{
    // Grab the dst bitmap info and pixels
    AndroidBitmapInfo dstInfo;
    void* dstPixels;
    AndroidBitmap_getInfo(env, dstBitmap, &dstInfo);
    AndroidBitmap_lockPixels(env, dstBitmap, &dstPixels);
	 // Unlock the dst's pixelslockBitmap
	AndroidBitmap_unlockPixels(env, dstBitmap);
    string strPathName = jstringtostring_utf8(env, jstrPath);
    string savPathName = jstringtostring_utf8(env, jsavPath);
	LOGI ("DataPath = %s, jsavPath = %s, dstPixels = %p, rect(%d, %d, %d), device (%d, %d)",
			strPathName.c_str(), savPathName.c_str(), dstPixels, dstInfo.width, dstInfo.height, dstInfo.format, iDeviceWidth, iDeviceHeight);
	 static bool IsInit = false;
	 static void* s_pLastDstPixels = dstPixels;
	 if(!IsInit)
	 {
		 if(strPathName.empty())
		 {
			 strPathName = DEFAULT_DATA_PATH;
		 }
		 StartNUIEngine(strPathName.c_str(), savPathName.c_str(), (unsigned char*)dstPixels, dstInfo.width, dstInfo.height, iDeviceWidth,  iDeviceHeight);
		 InitInstance(&dstInfo, dstPixels);
		 IsInit = true;
	 }
	 else
	 {
		  if(s_pLastDstPixels != dstPixels)
		  {
		 	// 马上画到新的view上
		 	AndroidResize(dstPixels, dstInfo.width, dstInfo.height);
		 	s_pLastDstPixels = dstPixels;
		  }
	 }
}

// 调用java的上屏方法
void CallJavaRequestRender()
{
	//JNIRequestRenderFromLocator::CallRequestedMothod();
	s_pCallbackFlip->CallRequestedMothod();
}

/*
 * Class:     com_example_HelloSkiaActivity
 * Method:    sendNUIMessage
 * Signature: (III)V
 */
//extern "C"
JNIEXPORT void JNICALL Java_com_nui_NUICore_sendNUIMessage
  (JNIEnv * env , jclass thiz, jint message, jint wParam, jint lParam)
{
	SendAndroidMessage(message, wParam, lParam, 1, 0, NULL);
}

/*
 * Class:     com_nui_NUICore
 * Method:    sendNUIMessageBuf
 * Signature: (IIII[B)V
 */
JNIEXPORT void JNICALL Java_com_nui_NUICore_sendNUIMessageBuf
  (JNIEnv * env, jclass thiz, jint message, jint wParam, jint lParam, jint iBufSize, jbyteArray pJavaBytes)
{
	jboolean bCopy = true;
	int iArraySize = env->GetArrayLength (pJavaBytes);
	ASSERT(iBufSize < iArraySize);
	// 获取一部分byte[]
	jbyte* array = env->GetByteArrayElements (pJavaBytes, 0);
	if (array == NULL)
	{
		LOGE ("Java_com_nui_NUICore_sendNUIMessageBuf GetByteArrayElements error\n");
		return;
	}
	//	wParam - pointcount
	int iPointcount = wParam;
    kn_byte* pJavaBuf = (kn_byte*)array;
	SendAndroidMessage(message, wParam, lParam, wParam, iBufSize, pJavaBuf);
	env->ReleaseByteArrayElements(pJavaBytes, array, 0);
}

JNIEXPORT void JNICALL Java_com_nui_NUICore_lockBitmap
  (JNIEnv *env, jclass)
{
	LockJavaBitmap();
}

/*
 * Class:     com_nplan_SkiaActivity
 * Method:    unlockBitmap
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_nui_NUICore_unlockBitmap
  (JNIEnv *env, jclass)
{
	UnlockJavaBitmap();
}

int JNI_SendJavaMsg(int iMsgID, int iArg1, int iArg2, const char* pszArg)
{
	int iRet = 0;
	if(s_pCallbackSendJavaMsg)
	{
		iRet = s_pCallbackSendJavaMsg->CallRequestedMothodIntRet(iMsgID, iArg1, iArg2, pszArg);
	}
	return iRet;
}

/*
 * Class:     com_koteinavi_NUICore
 * Method:    registerStaticCallback
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_nui_NUICore_registerStaticCallback
  (JNIEnv * env, jclass, jstring jstrClassName, jstring jstrMethodName, jstring jstrMethodSign, jint iCallbackID)
{
	string strClassName = jstringtostring_utf8(env, jstrClassName);
	string strMethodName = jstringtostring_utf8(env, jstrMethodName);
	string strMethodSign = jstringtostring_utf8(env, jstrMethodSign);
	LOGI ("registerStaticCallback %s | %s | %s", strClassName.c_str(), strMethodName.c_str(), strMethodSign.c_str());
	int iRet = -1;

	if(iCallbackID < CB_SIZE)
	{
		iRet = CALLBACKS[iCallbackID]->RegisterCallback(env, strClassName.c_str(), strMethodName.c_str(), strMethodSign.c_str());
	}
	return iRet;
}

/*
 * Class:     com_koteinavi_NUICore
 * Method:    unregisterCallback
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_nui_NUICore_unregisterCallback
  (JNIEnv* env , jclass, jint iCallbackID)
{
	int iRet = -1;
	if(iCallbackID < CB_SIZE)
	{
		iRet = CALLBACKS[iCallbackID]->UnregisterCallback(env);
	}
	return iRet;
}

void InitCallbacks()
{
	// 释放时机未知
	s_pCallbackFlip = new JNIRegistStaticMethod;
	s_pCallbackSendJavaMsg = new JNIRegistStaticMethod;
	CALLBACKS[CB_FLIP] = s_pCallbackFlip;
	CALLBACKS[CB_SEND_JAVAMSG] = s_pCallbackSendJavaMsg;
}

bool JNI_Struct(JNIEnv* env)
{
	//---------------Locator end--------------
	InitCallbacks();
	return true;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv *env;
	gJavaVM = vm;
	LOGI("JNI_OnLoad called");
	if (gJavaVM->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	{
		LOGE("Failed to get the environment using GetEnv()");
		return -1;
	}
	LOGI("JNI_OnLoad called 1111");
	if (!JNI_Struct(env))
	{
		LOGE("JNI_Struct error");
		return -1;
	}
	LOGI("JNI_OnLoad called 22222");
	return JNI_VERSION_1_6;
}


JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved)
{
	LOGI("##### JNI_OnUnload called");
}
