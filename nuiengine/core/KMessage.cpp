//  **************************************
//  File:        KMessage.cpp
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

#include "KMessage.h"
#include "DataSync.h"



// 添加用户消息
void KMsgQueue::AddUserMessage(KMessage* pMsg)
{
    writeLock lock(m_mutex);
    m_dqUserMsg.push_back(pMsg);

}

// 获取用户消息
KMessage* KMsgQueue::GetUserMessage()
{
	//	快速判断
	if(m_dqUserMsg.empty())
	{
		return false;
	}



    //KAutoMutex amLock(m_mutex);

    writeLock lock(m_mutex);

    KMessage* pMsg = NULL;

	if(!m_dqUserMsg.empty())
	{
        pMsg = m_dqUserMsg.front();
        m_dqUserMsg.pop_front();

	}

    return pMsg;

}
 
// 自定义消息队列
KMsgQueue g_UserMsgQueue;


//extern kn_hwnd g_hWndNavi;
kn_dword SendKMessage(KMessage* pMsg)
{
    return PostKMessage(pMsg);


}

kn_dword PostKMessage(KMessage* pMsg)
{

    g_UserMsgQueue.AddUserMessage(pMsg);

	return 0;
}


KMessage* GetKMessage()
{

    return g_UserMsgQueue.GetUserMessage();
}
 
