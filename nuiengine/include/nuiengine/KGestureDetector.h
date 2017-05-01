//  **************************************
//  File:        KGestureDetector.h
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
#ifndef K_KGESTURE_DETECTOR_H
#define K_KGESTURE_DETECTOR_H

#include "message.h"



enum
{
	GESTURE_UNKNOWN,

	GESTURE_ZOOM_BEGIN,
	GESTURE_ZOOM,
	GESTURE_ZOOM_END,

	GESTURE_RORATE_BEGIN,
	GESTURE_RORATE,
	GESTURE_RORATE_END,

	GESTURE_MAX
};



class NUI_API KGesture :  public KObject
{
public:
	KGesture();
	virtual ~KGesture(){};

public:
	// zoom, 
	kn_int m_type;

	// 两指距离
	kn_float m_zoom_span;

	// 中心点
	kn_float m_center_x;
	kn_float m_center_y;
	 
};

 
class NUI_API KTouchPoint:  public KObject
{
 
public:
	KTouchPoint(KMessageTouch* pMsg);
	virtual ~KTouchPoint();


	void calcDistance();

public:

	KMessageTouch* m_p_msgtouch;

	float m_distance;

	kn_float m_center_x;
	kn_float m_center_y;
	 
};


class NUI_API KGestureDetector :  public KObject
{
public:
	KGestureDetector();
	virtual ~KGestureDetector(); 

 
	void addTouchMessge(KMessageTouch* pMsg);

	KGesture recogGesture();

private:

	void Release();

private:

	vector<KTouchPoint*> m_v_touchpoints;

};


#endif
