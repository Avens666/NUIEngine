//  **************************************
//  File:        messageforlinux.cpp
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

#include "message.h"
#include "KLog.h"
#include "KBufferReader.h"
#include "CharSet.h"

void KMessageKey::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{

}

void KMessageTouch::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
	unsigned int numInputs = (unsigned int) wParam;
	POINT* points = (POINT*)lParam;
    m_iPointCount = numInputs;
    m_ids = new int [numInputs];
    m_xs = new int [numInputs];
    m_ys = new int [numInputs];
    
    for(unsigned int i = 0; i < numInputs; ++i)
    {
        m_msg_type = (KMESSAGETYPE)message;
        m_ids[i] = i+1;
        m_xs[i] = points[i].x;
        m_ys[i] = points[i].y;
	}
	delete [] points;
    
    
	if(m_iPointCount > 0)
	{
		m_pos_x = m_xs[0];
		m_pos_y = m_ys[0];
	}
    
	m_touch_id = numInputs;
}

void KMessageMouse::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{

}

void KMessageMouseWheel::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{

}
void KMessageEditTextIME::transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam)
{
	int iBufferSize = wParam;
	kn_byte* pBuf = (kn_byte*)lParam;
	KBufferReader reader (pBuf, iBufferSize);
	int iStart = 0;
	int iBefore = 0;
	int iCount = 0;
	int iWordSize = 0;
	reader.Read(iStart);
	reader.Read(iBefore);
	reader.Read(iCount);
	reader.Read(iWordSize);
	kn_char* pszUpdateText = new kn_char[iWordSize + 1];
	memset(pszUpdateText, 0, sizeof(kn_char) * (iWordSize + 1));
	for(int i = 0; i < iWordSize; i++)
	{
		kn_word wWord = 0;
		reader.Read(wWord);
		pszUpdateText[i] = wWord;
	}
	m_InputStr = pszUpdateText;
	m_i_start = iStart;
	m_i_before = iBefore;
	m_i_count = iCount;
	delete [] pszUpdateText;
}


KMessage* KMessage::getMsgForWin32(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	KMessage* msg = NULL;
	switch(message)
	{
	case KMSG_MOUSEMOVE:
	case KMSG_LBBUTTONDOWN:
	case KMSG_MBBUTTONDOWN:
	case KMSG_RBBUTTONDOWN:
	case KMSG_MBBUTTONUP:
	case KMSG_RBBUTTONUP:
	case KMSG_LBBUTTONUP:
	case KMSG_LBUTTONDBLCLK:
	case KMSG_MBUTTONDBLCLK:
	case KMSG_RBUTTONDBLCLK:
		{
			msg = new KMessageMouse;
			msg->m_msg_type = (KMESSAGETYPE)message;
			((KMessageMouse*)msg)->m_pos_x = wParam;
			((KMessageMouse*)msg)->m_pos_y = lParam;
		}
		break;
	case KMSG_MOUSEWHEEL:
		{
			msg = new KMessageMouseWheel;
			msg->m_msg_type = (KMESSAGETYPE)message;
			((KMessageMouseWheel*)msg)->m_pos_x = LOWORD(wParam);
			((KMessageMouseWheel*)msg)->m_pos_y = HIWORD(wParam);
			((KMessageMouseWheel*)msg)->m_roll = lParam;
		}
		break;
	case KMSG_KEYDOWN:
	case KMSG_KEYUP:
		msg = new KMessageKey;
		msg->m_msg_type = (KMESSAGETYPE)message;
		((KMessageKey*)msg)->m_key_id = wParam;
		((KMessageKey*)msg)->m_repeat_count = lParam;
		break;

	case KMSG_TIMER:

		break;
	case KMSG_USER:
		//        msg = new KMessage;
		//        msg->m_msg_type = wParam;
		//        msg->m_msg_class_type = KMSG_TYPE_USER;

		msg = (KMessage*)wParam;
            break;
    case KMSG_TOUCH_UP:
    case KMSG_TOUCH_DOWN:
    case KMSG_TOUCH_MOVE:
        msg = new KMessageTouch(hWnd);
        msg->transMsgForWin32(message,wParam,lParam);
        break;
	case KMSG_TYPE_IME_UPDATE:
		msg = new KMessageEditTextIME;
		msg->transMsgForWin32(message,wParam,lParam);
		msg->m_msg_class_type = KMSG_TYPE_INPUT;
		msg->m_msg_type = KMSG_TYPE_IME_UPDATE;
		break;
	default:
		break;
	}



	if (msg)
	{
		//        if (KNUIMSG_USER != message)
		//        { 
		//            msg->m_wParam = wParam;
		//            msg->m_lParam = lParam;
		//            msg->transMsgForWin32(message,wParam,lParam);
		//        }
	}

	return msg;
}


#endif // KOTEI_LINUX
