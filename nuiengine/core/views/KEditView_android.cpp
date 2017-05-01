//  **************************************
//  File:        KEditView_android.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/11
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************

/************************************************************************/
/* Android 输入法调用流程  [2015-7-3]

0. Java 层新建EditText 控件，放置于SurfaceView下面，并关联输入法

1. C++ KEditView 响应Click事件，向Java 发送JMSG_IME_ON 消息，含初始文本和光标位置
2. Java 处理JMSG_IME_ON 消息，设置EditText初始文本和光标位置， 并showIME
3. Java  EditText 监听textchange事件，把来自输入法的文本变化 通过sendNUIMessageBuf传回C++
4. C++ KEditView 处理 KMSG_TYPE_IME_UPDATE 消息 ，把结果显示在KEditView上

待解决
1. KEditView在下半屏时会遮盖输入法，可以把KEditView放在屏幕上部分或者使用半透明的全屏手写
2. 输入法的send，search按键消息 Java层已通过IMEWraper::onEditorAction获取, 但未传给C++处理 

*/
/************************************************************************/

#ifdef ANDROID_NDK
#include "KEditView.h"
#include "mainforwindows.h"
#include "KTextMultiLineDrawable.h"
#include "CharSet.h"

// java message , 需要和java代码一致
#define  JMSG_IME_ON  100
#define  JMSG_IME_OFF  101

 

int JNI_SendJavaMsg(int iMsgID, int iArg1, int iArg2, const char* pszArg);

// android 向java层发消息,启用或禁用输入法
void KEditView::showIME(kn_bool b)
{
	char szArg [] = "0";

	if(b)
	{
		 
		string strText = Knstring2Utf8String(this->getText());
		JNI_SendJavaMsg(JMSG_IME_ON, m_Index, m_input_max_length, strText.c_str());
	}
	else
	{
		JNI_SendJavaMsg(JMSG_IME_OFF, 0, 0, szArg);
	}


}




#endif // ANDROID_NDK
