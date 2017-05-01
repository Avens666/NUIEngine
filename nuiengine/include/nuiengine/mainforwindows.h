//  **************************************
//  File:        mainforwindows.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/12
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************
#ifndef K_MAINFORWINDOWS_H
#define K_MAINFORWINDOWS_H

#define CURSOR_ARROW 0
#define CURSOR_HAND 1
#define CURSOR_CROSS 2

#define NUI_USER_MSG_BEGIN 10000
#define NUI_USER_MSG_END 90000

#include "KScreen.h"
class NUI_API CNUIInstance
{
public:
	CNUIInstance();
	~CNUIInstance();
	void NUIWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void create(KNUIInitPara&);
	void release();
	KScreen* getScreen();
	void SendTimerEvent(int num);
	int RunModalEventLoop(bool& bExit);
	int RunEventLoop(bool& bExit);

	void SendOSMessage(int message, int wParam, int lParam);
	void ResizeWindow(void* dst, int w, int h);

	KScreen m_screen;
	bool m_b_move_wnd;
	HWND m_hWnd;
};

void SetMouseCursorStyle(kn_dword dwStyleID);

 


//keyboard
#define Key_spaceKey                VK_SPACE
#define Key_returnKey               VK_RETURN
#define Key_escapeKey               VK_ESCAPE
#define Key_backspaceKey            VK_BACK
#define Key_deleteKey               VK_DELETE         
#define Key_insertKey               VK_INSERT         
#define Key_tabKey                  VK_TAB;
#define Key_leftKey                 VK_LEFT           
#define Key_rightKey                VK_RIGHT          
#define Key_upKey                   VK_UP             
#define Key_downKey                 VK_DOWN           
#define Key_homeKey                 VK_HOME           
#define Key_endKey                  VK_END            
#define Key_pageUpKey               VK_PRIOR          
#define Key_pageDownKey             VK_NEXT           
#define Key_F1Key                   VK_F1             
#define Key_F2Key                   VK_F2             
#define Key_F3Key                   VK_F3             
#define Key_F4Key                   VK_F4             
#define Key_F5Key                   VK_F5             
#define Key_F6Key                   VK_F6             
#define Key_F7Key                   VK_F7             
#define Key_F8Key                   VK_F8             
#define Key_F9Key                   VK_F9             
#define Key_F10Key                  VK_F10            
#define Key_F11Key                  VK_F11            
#define Key_F12Key                  VK_F12            
#define Key_F13Key                  VK_F13            
#define Key_F14Key                  VK_F14            
#define Key_F15Key                  VK_F15            
#define Key_F16Key                  VK_F16            
#define Key_numberPad0              VK_NUMPAD0        
#define Key_numberPad1              VK_NUMPAD1        
#define Key_numberPad2              VK_NUMPAD2        
#define Key_numberPad3              VK_NUMPAD3        
#define Key_numberPad4              VK_NUMPAD4        
#define Key_numberPad5              VK_NUMPAD5        
#define Key_numberPad6              VK_NUMPAD6        
#define Key_numberPad7              VK_NUMPAD7        
#define Key_numberPad8              VK_NUMPAD8        
#define Key_numberPad9              VK_NUMPAD9        
#define Key_numberPadAdd            VK_ADD            
#define Key_numberPadSubtract       VK_SUBTRACT       
#define Key_numberPadMultiply       VK_MULTIPLY       
#define Key_numberPadDivide         VK_DIVIDE         
#define Key_numberPadSeparator      VK_SEPARATOR      
#define Key_numberPadDecimalPoint   VK_DECIMAL        
#define Key_numberPadEquals         0x92 /*VK_OEM_NEC_EQUAL*/  
#define Key_numberPadDelete         VK_DELETE         
#define Key_playKey                 0x30000;
#define Key_stopKey                 0x30001;
#define Key_fastForwardKey          0x30002;
#define Key_rewindKey               0x30003;

class NUI_API SystemClipboard
{
public:
    /** Copies a string of text onto the clipboard */
    static void copyTextToClipboard (const kn_string& text);

    /** Gets the current clipboard's contents.

        Obviously this might have come from another app, so could contain
        anything..
    */
    static kn_string getTextFromClipboard();
};


#endif
