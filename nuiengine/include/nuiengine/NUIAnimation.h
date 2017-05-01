//  **************************************
//  File:        NUIAnimation.h
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
#pragma once
#include "NE_type.h"
#include <vector>
#include "KEasingCurve.h"
#include "DataSync.h"
#include <boost/timer/timer.hpp>
class KScreen;
class NUI_API CPropertyAnimationData:public KObject
{
public:
	CPropertyAnimationData(CPropertyBase* tar, KEasingCurve::Type= KEasingCurve::InOutCirc); //只指定target，src自动从pview当前值拷贝
	CPropertyAnimationData(CPropertyBase* src, CPropertyBase* tar, KEasingCurve::Type = KEasingCurve::InOutCirc);
	virtual ~CPropertyAnimationData(void);	
	CPropertyBase* getCurrent(kn_double v);
	KScreen* getScreen();
private:
	CPropertyBase* m_p_source;
	CPropertyBase* m_p_target;
	//运动曲线
	KEasingCurve m_easing;
};
typedef vector<CPropertyAnimationData*> CPropertyAnimationDataList;

#define LOOP_ALWAYS 9999999
class NUI_API CNUIAnimation :public KObject
{
public:
	CNUIAnimation(void);
	CNUIAnimation(kn_int duration, kn_int wait_time, kn_int loop_wait_time=0);
	virtual ~CNUIAnimation(void);

	enum RunState
	{
		PLAYING,
		PAUSE,
		STOP,
		NO_START
	};

	enum LoopType
	{
		LOOP_TIME,
		LOOP_PINGPONG
	};
	void init();
	//加入动画数据，动画播放过程中包括pause是不能加入的
	kn_bool addAnimationData(CPropertyAnimationData*);
	void Play();
	void Stop();
	void Pause();

	KScreen* getScreen();

	void setRunState(RunState e);
	RunState getRunState();
	void setLoopType(LoopType e);	//只有STOP状态才能改变这些值
	void setLoopTimes(kn_int);
	void setLoopWaitTime(kn_int);
	//每帧间隔时间，影响执行速度
	void setFrameTime(kn_uint);

	void setDuration(kn_int);

	kn_bool onPlay(kn_int);
private:

	boost::shared_mutex m_ani_data_mutex;
	CPropertyAnimationDataList m_ani_data_lst;

	kn_bool updateFrame( kn_double percent, kn_bool b_draw = FALSE);

	//根据循环方式返回
	kn_double getPercent(kn_int current);

//	CPropertyBase* m_p_current;
	boost::mutex m_runing_mutex;
	RunState m_e_runing;

	//播放持续时间 毫秒单位
	kn_int m_duration;

	//每帧持续时间 毫秒单位
	kn_uint m_frame_time;

	//暂停的流逝时间
	kn_int m_pause_time;

	//线程传入的最后时间 用于stop后play矫正时间
	kn_int m_last_elapsed;

	boost::timer::cpu_timer m_pause_timer;

	//播放延迟时间
	kn_int m_wait_time;

	//循环类别
	LoopType m_loop_type;
	//循环次数
	kn_int	m_loop_times;
	//当前循环次数
	kn_int m_current_loop;
	//循环间隔等待时间 微秒单位
	kn_int	m_loop_wait_time;
};
typedef vector<CNUIAnimation*> CNUIAnimationDataList;
