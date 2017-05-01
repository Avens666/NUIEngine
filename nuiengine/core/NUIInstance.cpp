//  **************************************
//  File:        NUIInstance.cpp
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

#include "KScreen.h"
#include "mainforwindows.h"
#include "KView.h"
#include "KTimer.h"
#include "KMessage.h"

CNUIInstance::CNUIInstance()
{
	m_hWnd = 0;
}

CNUIInstance::~CNUIInstance()
{

}

void CNUIInstance::release()
{
	m_screen.Release();
}

KScreen* CNUIInstance::getScreen()
{
	return &m_screen;
}


void  CNUIInstance::create(KNUIInitPara& para)
{
	m_screen.Create(para);
	m_screen.setNUIInstance(this);
	m_hWnd = para.m_wnd;

}


void CNUIInstance::SendTimerEvent(int num)
{
	KMessage* pMsg = GetKMessage();
	while(pMsg)
	{
	  
		LOGI("CNUIInstance::SendTimerEvent 2222 CORE_INITIAL_PROGRESS m_msg_type (%d)", pMsg->m_msg_type);
		NUIWndProc(m_hWnd, KMSG_USER, (WPARAM)pMsg, 0 );
		pMsg = GetKMessage();

	}
//	NUIWndProc(m_hWnd, KMSG_TIMER, GetTickCount() / 10, 0 );

	CheckNUITimer();

}


int CNUIInstance::RunEventLoop(bool& bExit)
{
	return this->RunModalEventLoop(bExit);
}

// for android & ios
void CNUIInstance::SendOSMessage(int message, int wParam, int lParam)
{
	if(message == KMSG_TIMER)
	{
		SendTimerEvent(0);

	}
	else
	{
		NUIWndProc(0, message, wParam, lParam);

	}

}

void CNUIInstance::ResizeWindow(void* dst, int w, int h)
{
	m_screen.setWnd((HWND)dst);
	m_screen.resize(w , h);
 
}



kn_int KView::runModalLoop()
{
  
	KScreen* pScreen = GetScreen();

	if(pScreen == NULL)
	{
		ASSERT(0);
		return -1;
	}

	CNUIInstance* pNUIInstance = pScreen->getNUIInstance();
	if(pNUIInstance == NULL)
	{
		ASSERT(0);
		return -1;
	}


	pNUIInstance->RunModalEventLoop(m_b_modal_exit);
	return m_i_modal_result;


	//return -1;
}

 
