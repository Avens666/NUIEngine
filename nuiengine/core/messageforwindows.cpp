//  **************************************
//  File:        messageforwindows.cpp
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
#if defined(WIN32)

#include "message.h"
#include "KLog.h"
// void MyTraceA(const char* szFormat, ... );

//#define  LOGI  MyTraceA 


//////////////////////////////////////////////////////////////////////////
// 处理触摸消息

//#if(_MSC_VER >= 1600) // Visual Studio 2010 or higher version.

//#pragma message("Visual Studio 2010 or higher version.") 
// Windows Touch define
#define MOUSEEVENTF_FROMTOUCH 0xFF515700

//	API GetTouchInputInfo无法在WinXP下运行, 会提示 GetTouchInputInfo  无法定位程序输入点 
//  使用函数指针是为了保证程序可以同时在xp和win7下运行
typedef WINUSERAPI BOOL (WINAPI *RegisterTouchWindowFn)(_In_ HWND hwnd, _In_ ULONG ulFlags);
typedef WINUSERAPI BOOL (WINAPI *UnregisterTouchWindowFn)(_In_ HWND hwnd);
typedef WINUSERAPI LPARAM (WINAPI *GetMessageExtraInfoFn)(VOID);
typedef WINUSERAPI BOOL (WINAPI *GetTouchInputInfoFn)(_In_ HTOUCHINPUT hTouchInput, _In_ UINT cInputs, __out_ecount(cInputs) PTOUCHINPUT pInputs, _In_ int cbSize);
typedef WINUSERAPI BOOL (WINAPI *CloseTouchInputHandleFn)(_In_ HTOUCHINPUT hTouchInput);

static RegisterTouchWindowFn s_pfRegisterTouchWindowFunction = NULL;
static UnregisterTouchWindowFn s_pfUnregisterTouchWindowFunction = NULL;
static GetMessageExtraInfoFn s_pfGetMessageExtraInfoFunction = NULL;
static GetTouchInputInfoFn s_pfGetTouchInputInfoFunction = NULL;
static CloseTouchInputHandleFn s_pfCloseTouchInputHandleFunction = NULL;

static bool s_bSupportTouch = false;

bool CheckTouchSupport(HWND wnd)
{
	s_pfRegisterTouchWindowFunction = (RegisterTouchWindowFn)GetProcAddress(GetModuleHandle(_T("user32.dll")), "RegisterTouchWindow");
	s_pfUnregisterTouchWindowFunction = (UnregisterTouchWindowFn)GetProcAddress(GetModuleHandle(_T("user32.dll")), "UnregisterTouchWindow");
	s_pfGetMessageExtraInfoFunction = (GetMessageExtraInfoFn)GetProcAddress(GetModuleHandle(_T("user32.dll")), "GetMessageExtraInfo");
	s_pfGetTouchInputInfoFunction = (GetTouchInputInfoFn)GetProcAddress(GetModuleHandle(_T("user32.dll")), "GetTouchInputInfo");
	s_pfCloseTouchInputHandleFunction = (CloseTouchInputHandleFn)GetProcAddress(GetModuleHandle(_T("user32.dll")), "CloseTouchInputHandle");

	bool bFuncOK = (s_pfRegisterTouchWindowFunction && s_pfUnregisterTouchWindowFunction && s_pfGetMessageExtraInfoFunction && s_pfGetTouchInputInfoFunction && s_pfCloseTouchInputHandleFunction);

	if(bFuncOK)
	{
		s_bSupportTouch = (s_pfRegisterTouchWindowFunction(wnd, 0) != 0);

	}

	return s_bSupportTouch;
}

// mouse message 是否源于touch
bool IsMouseMsgFromTouch()
{
	return (s_bSupportTouch && (s_pfGetMessageExtraInfoFunction() & MOUSEEVENTF_FROMTOUCH) == MOUSEEVENTF_FROMTOUCH); 
}


//#endif /* #if(_MSC_VER >= 1600) */


 
#pragma comment(lib,"imm32.lib")
KMessage* KMessage::getMsgForWin32(HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam)
{
	KMessageInput* pTempMsg;
	KMessage* msg = NULL;
	switch(message)
	{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_MBUTTONDBLCLK:
		case WM_RBUTTONDBLCLK:
			if((message != WM_LBUTTONDBLCLK) && IsMouseMsgFromTouch())
			{
				 //	过滤touch 产生的mouse 消息
			}
			else
			{

				msg = new KMessageMouse;
			}
			
			break;
		case WM_MOUSEWHEEL:
			msg = new KMessageMouseWheel;
			//滚轮坐标是全屏幕坐标，校正为hwnd本地坐标
			RECT rect;
			GetWindowRect(hWnd, &rect);
			lParam = ((short(HIWORD(lParam))-rect.top)<<16) | (short(LOWORD (lParam)) - rect.left ) ;

			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
			msg = new KMessageKey;
			break;
#ifndef _WIN32_WCE
		case WM_TOUCH:
			msg = new KMessageTouch(hWnd);
			break;
#endif
		//case KNUIMSG_DRAW:
		//	msg = new KMessage;
		//	msg->m_msg_type = KMSG_SCREEN_DRAW;
		//	break;

		//case KNUIMSG_DRAG:
		//case KNUIMSG_DRAG_UP:
		//	msg = new KMessageDrag;
		//	break;

		case WM_CHAR:


			// WM_CHAR 支持ASCII和Unicode， 无需再做输入法状态判断, 且ImmIsIME 不支持XP以上的系统
			//wchar_t wch  ;
			//HKL hklCurrent;
			//hklCurrent=GetKeyboardLayout(0);

			//DWORD dwVersion;
			//DWORD dwMajorVersion;
			//DWORD dwMinorVersion;
			//dwVersion	= GetVersion();

			//dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
			//dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));
			//if(dwMajorVersion <= 5 && ImmIsIME(hklCurrent))  // 如果有输入法打开
			//{
			//	HIMC hIMC = ImmGetContext(hWnd);  
			//	DWORD   dwConv,   dwSent;  
			//	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);   
			//	if((dwConv   &   IME_CMODE_LANGUAGE)   !=   IME_CMODE_ALPHANUMERIC)  
			//	{
			//		if((wParam > _T('9') || wParam < _T('*')))
			//		{
			//			break;
			//		}
			//	}
			////	if((wParam >= _T('a') && wParam <= _T('z')) ||  (wParam >= _T('A') && wParam <= _T('Z')))
			////	{
			////		break;
			////	}
			//}

			//if(g_bImming)
			//{
			//	break;
			//}

			pTempMsg = new KMessageInput; 
			pTempMsg->m_msg_class_type = KMSG_TYPE_INPUT;		
			pTempMsg->m_msg_type = KMSG_TYPE_CHAR;
			
			//重复按键次数
			pTempMsg->m_repeat_count = lParam&0x00ff;
			pTempMsg->m_key_flag = 0;
			pTempMsg->m_key_id = wParam;

			//wch = wParam;
			if ( GetKeyState(VK_CONTROL) < 0)
			{
				pTempMsg->m_key_flag |= KMSG_KEYFLAG_CONTROL;
			}
			if ( GetKeyState(VK_SHIFT) < 0)
			{
				pTempMsg->m_key_flag |= KMSG_KEYFLAG_SHIFT;
			}
			msg = pTempMsg;
			break;
			
		//case WM_IME_COMPOSITION:
 
		//	pTempMsg = new KMessageInput; 
		//	pTempMsg->m_msg_class_type = KMSG_TYPE_INPUT; //大类别
		//	HIMC hIMC;  
		//	DWORD dwSize;  
		//	if (pTempMsg->m_key_id == VK_RETURN)
		//	{
		//		break;
		//	}
		//	if(lParam & GCS_RESULTSTR)  
		//	{  
		//		hIMC = ImmGetContext(hWnd);  
		//		dwSize = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);  

		//		dwSize += sizeof(TCHAR);  

		//		TCHAR* lpstr = new TCHAR[dwSize];  
		//		memset(lpstr, 0, dwSize * sizeof(TCHAR));  
		//		ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpstr, dwSize);  

		//		ImmReleaseContext(hWnd, hIMC);  
		//		

		//		pTempMsg->m_msg_type = KMSG_TYPE_INPUT_END;
		//		pTempMsg->m_InputStr = lpstr;

		//		SAFE_DELETE_GROUP(lpstr);
		//	}  
		//	else
		//	{
		//		pTempMsg->m_msg_type = KMSG_TYPE_INPUT_ING;
		//	}

		//	msg = pTempMsg;
		//	break;
		//case KNUIMSG_USER:
		case	KMSG_USER:
			msg = (KMessage*)wParam;
			break;
		default:
			break;			
	}



	if (msg)
	{
		if (KMSG_USER != message)
		{//自定义消息，构造时确定msg，不用翻译
			msg->m_wParam = wParam;
			msg->m_lParam = lParam;
			msg->transMsgForWin32(message,wParam,lParam);
		}
	}

	return msg;
}

void KMessageKey::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		m_msg_type = KMSG_KEYDOWN;
		break;
	case WM_KEYUP:
		m_msg_type = KMSG_KEYUP;
		break;
	default:
		return;
	}

	//重复按键次数
	m_repeat_count = lParam&0x00ff;
	m_key_flag = 0;
	m_key_id = wParam;
	if ( GetKeyState(VK_CONTROL) < 0)
	{
		m_key_flag |= KMSG_KEYFLAG_CONTROL;
	}
	if ( GetKeyState(VK_SHIFT) < 0)
	{
		m_key_flag |= KMSG_KEYFLAG_SHIFT;
	}

}



void KMessageMouse::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		m_msg_type = KMSG_MOUSEMOVE;
		break;
	case WM_LBUTTONDOWN:
		m_msg_type = KMSG_LBBUTTONDOWN;
		break;
	case WM_MBUTTONDOWN:
		m_msg_type = KMSG_MBBUTTONDOWN;
		break;
	case WM_RBUTTONDOWN:
		m_msg_type = KMSG_RBBUTTONDOWN;
		break;
	case WM_MBUTTONUP:
		m_msg_type = KMSG_MBBUTTONUP;
		break;
	case WM_RBUTTONUP:
		m_msg_type = KMSG_RBBUTTONUP;
		break;
	case WM_LBUTTONUP:
		m_msg_type = KMSG_LBBUTTONUP;
		break;
	case WM_LBUTTONDBLCLK:
		m_msg_type = KMSG_LBUTTONDBLCLK;
		break;
	case WM_MBUTTONDBLCLK:
		m_msg_type = KMSG_MBUTTONDBLCLK;
		break;
	case WM_RBUTTONDBLCLK:
		m_msg_type = KMSG_RBUTTONDBLCLK;
		break;
	default:
		return;
	}

	m_pos_x = short(LOWORD (lParam)) ;
	m_pos_y = short(HIWORD(lParam)) ;


}



void KMessageMouseWheel::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_MOUSEWHEEL:
		m_key_flag = LOWORD(wParam);   //  key flags
		m_roll     = (short)HIWORD(wParam);     
		break;
	default:
		return;
	}

	m_pos_x = short(LOWORD (lParam)) ;
	m_pos_y = short(HIWORD(lParam)) ;


}

void KMessageTouch::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
 
 
	if(!s_bSupportTouch)
	{
		return;
	}
	  

	unsigned int numInputs = (unsigned int) wParam; 
	TOUCHINPUT* tis = new TOUCHINPUT[numInputs]; 

	LOGI ("===================================================================");
	LOGI ("HandleTouch numInputs = %d", numInputs);

	int TOUCH_EVENT [] = 
	{
		TOUCHEVENTF_MOVE,       
		TOUCHEVENTF_DOWN,       
		TOUCHEVENTF_UP,        
		TOUCHEVENTF_INRANGE,    
		TOUCHEVENTF_PRIMARY,    
		TOUCHEVENTF_NOCOALESCE, 
		TOUCHEVENTF_PEN,        
		TOUCHEVENTF_PALM       
	};

	char TOUCH_STRING [][32]= 
	{
		"TOUCHEVENTF_MOVE",       
		"TOUCHEVENTF_DOWN",       
		"TOUCHEVENTF_UP",        
		"TOUCHEVENTF_INRANGE",    
		"TOUCHEVENTF_PRIMARY",    
		"TOUCHEVENTF_NOCOALESCE", 
		"TOUCHEVENTF_PEN",
		"TOUCHEVENTF_PALM"  
	};

	int TOUCH_EVENT_SIZE = sizeof(TOUCH_EVENT) / sizeof(TOUCH_EVENT[0]); 


	if(s_pfGetTouchInputInfoFunction((HTOUCHINPUT)lParam, numInputs, tis, sizeof(TOUCHINPUT)))
	{

		m_iPointCount = numInputs;
		m_ids = new int [numInputs];
		m_xs = new int [numInputs];
		m_ys = new int [numInputs];


		// Handle each contact point
		for(unsigned int i = 0; i < numInputs; ++i)
		{
			/* handle ti[i]  */

			TOUCHINPUT& ti = tis[i];

			if(ti.dwFlags & TOUCHEVENTF_DOWN)
			{
				m_msg_type = KMSG_TOUCH_DOWN;
			}
			else if(ti.dwFlags & TOUCHEVENTF_MOVE)
			{
				m_msg_type = KMSG_TOUCH_MOVE;
			}
			else if(ti.dwFlags & TOUCHEVENTF_UP)
			{
				m_msg_type = KMSG_TOUCH_UP;
			}
			else
			{
				ASSERT(0);
			}

			m_touch_id = ti.dwID;
						
			POINT point = {ti.x / 100, ti.y / 100};
			//POINT point;
			//point.x = TOUCH_COORD_TO_PIXEL(ti.x);
			//point.y = TOUCH_COORD_TO_PIXEL(ti.y);

			// 屏幕坐标转窗口坐标
			ScreenToClient(m_wnd, &point);
			
			//ScreenToClient(g_hWnd, &point);
			//msg.mousex = point.x;
			//msg.mousey = point.y;

			m_ids[i] = ti.dwID;
			m_xs[i] = point.x;
			m_ys[i] = point.y;


			// dwID 为接触点标识符, x, y 为屏幕像素点的1/100, dwFlags 为DOWN/MOVE/UP
			LOGI ("touchinput[%d]: x(%d), y (%d), hSource(%d),dwID(%d),dwFlags(%d),dwMask(%d), dwTime(%d), dwExtraInfo(%d),cxContact(%d), cyContact(%d)", 
				i, ti.x / 100, ti.y / 100, ti.hSource, ti.dwID, ti.dwFlags, ti.dwMask, ti.dwTime, ti.dwExtraInfo, ti.cxContact, ti.cyContact);
			LOGI ("-------------------------------------------------------------------");
		}
	}
	s_pfCloseTouchInputHandleFunction((HTOUCHINPUT)lParam);
	delete [] tis;


	if(m_iPointCount > 0)
	{
		m_pos_x = m_xs[0];
		m_pos_y = m_ys[0];
	}

	m_touch_id = m_iPointCount;

	
 
}

void KMessageEditTextIME::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{

}
#include "KEditView.h"
void KEditView::showIME(kn_bool b)
{
}
#endif // WIN32
