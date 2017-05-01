//  **************************************
//  File:        mainforlinux.cpp
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
#ifdef KOTEI_LINUX

// for linux_qt / android / ios


#include "KScreen.h"
#include "mainforwindows.h"
#include "KView.h"

 
void CNUIInstance::NUIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	KMessage*  msg = KMessage::getMsgForWin32 (hWnd,message,wParam,lParam);
	if (msg)
	{
		m_screen.DoMessage(msg);
		SAFE_DELETE(msg);
	}

 
}


int CNUIInstance::RunModalEventLoop(bool& bExit)
{
	return 0;
}

// 设置鼠标光标式样, 必须先保证WindowClass中的cursor为空
void SetMouseCursorStyle(kn_dword dwStyleID)
{ 
 

}
 


void SystemClipboard::copyTextToClipboard (const kn_string& text)
{
 
}

kn_string SystemClipboard::getTextFromClipboard()
{
 

	return _T("");
}


#endif // KOTEI_LINUX
