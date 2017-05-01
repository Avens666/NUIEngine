//  **************************************
//  File:        KGestureDetector.cpp
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
#include "KGestureDetector.h"
 
//////////////////////////////////////////////////////////////////////////
KGesture::KGesture()
{
	m_type = GESTURE_UNKNOWN;

	// 两指距离
	m_zoom_span = 0;

	// 中心点
	m_center_x = 0;
	m_center_y = 0;
}

//////////////////////////////////////////////////////////////////////////

KTouchPoint::KTouchPoint(KMessageTouch* pMsg)
{
	m_p_msgtouch = pMsg;

	m_distance = 0.0f;

	m_center_x = 0.0f;
	m_center_y = 0.0f;

	calcDistance();

}

void KTouchPoint::calcDistance()
{
	int m_iPointCount;	//	触摸点个数
	int* m_ids;	// 标志point id
	int* m_xs;	// 多点坐标
	int* m_ys;


	if(m_p_msgtouch != NULL && m_p_msgtouch->m_iPointCount >=2)
	{
		int dx = m_p_msgtouch->m_xs[0] - m_p_msgtouch->m_xs[1];
		int dy = m_p_msgtouch->m_ys[0] - m_p_msgtouch->m_ys[1];

		m_distance = sqrt ((float)(dx * dx + dy * dy));

		m_center_x = (m_p_msgtouch->m_xs[0] + m_p_msgtouch->m_xs[1]) / 2.0f;
		m_center_y = (m_p_msgtouch->m_ys[0] + m_p_msgtouch->m_ys[1]) / 2.0f;
 

	}
}

KTouchPoint::~KTouchPoint()
{
	SAFE_DELETE (m_p_msgtouch);
	m_distance = 0.0f;

	m_center_x = 0.0f;
	m_center_y = 0.0f;

}


//////////////////////////////////////////////////////////////////////////
KGestureDetector::KGestureDetector()
{

}


KGestureDetector::~KGestureDetector()
{
	Release();
}


void KGestureDetector::addTouchMessge(KMessageTouch* pMsg)
{
 
	KMessageTouch* pTouchMsg = pMsg->deepCopy();

	if(pTouchMsg->m_msg_type == KMSG_TOUCH_DOWN && pTouchMsg->m_iPointCount == 1)
	{
		Release();
	}

	KTouchPoint* pTouchPoint = new KTouchPoint(pTouchMsg);

	m_v_touchpoints.push_back(pTouchPoint);
}

KGesture KGestureDetector::recogGesture()
{
	KGesture gesture;

	if(!m_v_touchpoints.empty())
	{
		KTouchPoint* pTouchPoint = m_v_touchpoints.back();
		KMessageTouch* pMsgTouch = pTouchPoint->m_p_msgtouch;
 
		if(pTouchPoint->m_p_msgtouch->m_iPointCount == 2)
		{
			gesture.m_zoom_span = pTouchPoint->m_distance;
			gesture.m_center_x = pTouchPoint->m_center_x;
			gesture.m_center_y = pTouchPoint->m_center_y;

			if(pMsgTouch->m_msg_type == KMSG_TOUCH_DOWN)
			{
				gesture.m_type = GESTURE_ZOOM_BEGIN;
			}
			else if(pMsgTouch->m_msg_type == KMSG_TOUCH_MOVE)
			{
				gesture.m_type = GESTURE_ZOOM;
			}
			else if(pMsgTouch->m_msg_type == KMSG_TOUCH_UP)
			{
				gesture.m_type = GESTURE_ZOOM_END;
			}


			
		}
		else
		{
			if(m_v_touchpoints.size() > 2)
			{
				KTouchPoint* pTouchPoint = m_v_touchpoints.back();
				KTouchPoint* pLastTouchPoint = m_v_touchpoints[m_v_touchpoints.size() - 2];

				// 2点变1点
				if(pTouchPoint->m_p_msgtouch->m_iPointCount == 1 && pLastTouchPoint->m_p_msgtouch->m_iPointCount == 2
					&& pLastTouchPoint->m_p_msgtouch->m_msg_type != KMSG_TOUCH_UP)
				{
					
					gesture.m_zoom_span = pLastTouchPoint->m_distance;
					gesture.m_center_x = pLastTouchPoint->m_center_x;
					gesture.m_center_y = pLastTouchPoint->m_center_y;
					gesture.m_type = GESTURE_ZOOM_END;

				}
			}
		}

	}

	return  gesture;
}

void KGestureDetector::Release()
{
	for(vector<KTouchPoint*>::iterator it = m_v_touchpoints.begin(); it != m_v_touchpoints.end(); it++)
	{
		KTouchPoint* p = *it;
		delete p;
	}

	m_v_touchpoints.clear();
}
