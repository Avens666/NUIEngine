package com.nui;

import android.graphics.Bitmap;

//获取jni文件头的方法
//cd D:\SVN\papaya\trunk\03Code\NUINaviBase\android_build\bin\classes
//javah -jni -classpath . com.nui.NaviCore
 
public class NUICore 
{
	// Java向C++注册的回调列表ID
	public static final int CB_FLIP = 0; 
	public static final int CB_SEND_JAVAMSG = 1;
	public static final int CB_SIZE = 2;
	public static native void lockBitmap();
	public static native void unlockBitmap();
	public static native void initializeEngine(String strDataPath, String savPath, Bitmap image,  int iDeviceWidth, int iDeviceHeight);
	public static native void drawIntoBitmap(Bitmap image,  int iDeviceWidth, int iDeviceHeight, long elapsedTime);
	public static native void sendNUIMessage(int message, int wParam, int lParam);  
	public static native void sendNUIMessageBuf(int message, int wParam, int lParam, int iBufSize, byte[] buff);
	// 统一的Java向C++注册的回调接口
	public static native int registerStaticCallback(String strClassName, String strMethodName, String strMethodSign, int iCallbackID);	
	public static native int unregisterCallback(int iCallbackID);
	// 设置app在前台还是后台
	public static native int setAppActive(int bActive);
	
}
