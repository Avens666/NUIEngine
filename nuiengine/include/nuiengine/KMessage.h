//  **************************************
//  File:        KMessage.h
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

#ifndef __KKOTEI_MESSAGE_H__
#define __KKOTEI_MESSAGE_H__

#include "NE_pubhead.h"
#include "message.h"

//#define WM_NAVI_BASE (WM_USER + 1000)	//	消息起始
//enum KKoteiMessage
//{
//	WM_NAVI_HMI_SETSPECBOOK_FINISHED = 1,		//	通知HMI设置特殊收藏点完成
//    WM_NAVI_GET_POSITION,
//    WM_NAVI_SET_POSITION
//};


kn_dword SendKMessage(KMessage* pMsg);
kn_dword PostKMessage(KMessage* pMsg);
KMessage* GetKMessage();



// 自定义消息队列
class NUI_API KMsgQueue
{
public:

	// 加入用户消息
    void AddUserMessage(KMessage* pMsg);

	// 获取用户消息
    KMessage* GetUserMessage();

 

protected:

    // 自定义消息列表
    deque<KMessage*> m_dqUserMsg;


	// 操作消息的互斥
    //KMutex m_mutex;
    boost::shared_mutex m_mutex;
 

};


#endif // __KKOTEI_MESSAGE_H__
