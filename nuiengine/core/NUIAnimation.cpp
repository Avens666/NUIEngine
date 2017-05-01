//  **************************************
//  File:        NUIAnimation.cpp
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
#include "NUIAnimation.h"
#include "KView.h"
#include "KScreen.h"


//////////////////////////CPropertyAnimationData/////////////////
CPropertyAnimationData::CPropertyAnimationData(CPropertyBase* tar, KEasingCurve::Type type)
{
	m_p_source = tar->copyProperty();
	m_p_target = tar;
	m_easing.SetType(type);
}

CPropertyAnimationData::CPropertyAnimationData(CPropertyBase* src, CPropertyBase* tar, KEasingCurve::Type type)
{
	m_p_source = src;
	m_p_target = tar;
	m_easing.SetType(type);
}

CPropertyAnimationData::~CPropertyAnimationData()
{
	SAFE_DELETE( m_p_source);
	SAFE_DELETE( m_p_target);

}

//根据百分比获取中间帧属性
CPropertyBase* CPropertyAnimationData::getCurrent(kn_double v)
{
	 return m_p_source->getCurrent(m_p_target, v, &m_easing);
}

KScreen* CPropertyAnimationData::getScreen()
{
	return m_p_source->getView()->GetScreen();
}

///////////////////////////CNUIAnimation///////////////////////////////////////////////////

CNUIAnimation::CNUIAnimation(void)
{
	init();
}

void CNUIAnimation::init()
{
	m_e_runing = STOP;

	//播放持续时间 毫秒单位
	m_duration = 0;

	//每帧持续时间 毫秒单位
	m_frame_time = 30;

	m_pause_time = 0;

	//播放延迟时间
	m_wait_time = 0;
	m_last_elapsed = 0;

	m_loop_times = 1;
	m_current_loop = 0;
	m_loop_wait_time = 0;
	m_loop_type = LOOP_TIME;
}

CNUIAnimation::CNUIAnimation(kn_int duration, kn_int wait_time, kn_int loop_wait_time)
{
	init();

	//播放持续时间 毫秒单位
	m_duration = duration;

	//播放延迟时间
	m_wait_time = wait_time;
	//两次循环间间隔时间
	m_loop_wait_time = loop_wait_time;
}

void CNUIAnimation::setDuration(kn_int v)
{
	m_duration = v;
}
CNUIAnimation::~CNUIAnimation(void)
{
		for(CPropertyAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
		{
			SAFE_DELETE(*ite);
		}
		m_ani_data_lst.clear();
}


kn_bool  CNUIAnimation::addAnimationData(CPropertyAnimationData* p)
{
	if ( m_e_runing == STOP )
	{
		writeLock lock(m_ani_data_mutex);
		m_ani_data_lst.push_back(p);
		return TRUE;
	}

	return FALSE;
}



void CNUIAnimation::Stop()
{
	setRunState(STOP);
	m_pause_time = 0;
}

void CNUIAnimation::Play()
{
	using boost::timer::cpu_times;
	switch (m_e_runing)
	{
	case PAUSE:
		{
			cpu_times const elapsed_times(m_pause_timer.elapsed());
			m_pause_time += (elapsed_times.wall)/(1000000LL) ;
			m_pause_timer.stop();
		}
		break;
	case STOP:

		break;
	case PLAYING:
		return;
		break;
	}

	setRunState(PLAYING);
}

void CNUIAnimation::Pause()
{
	setRunState(PAUSE);
	m_pause_timer.start();
}

void CNUIAnimation::setRunState(RunState s)
{
	boost::mutex::scoped_lock lock(m_runing_mutex);
	m_e_runing = s;
}
CNUIAnimation::RunState CNUIAnimation::getRunState()
{
	return m_e_runing;
}

void CNUIAnimation::setLoopType(LoopType e)
{
	if ( m_e_runing == STOP )
	{
		m_loop_type = e;
	}
	
}

//返回值表示是否进行了绘制
kn_bool CNUIAnimation::onPlay(kn_int elapsed)
{
	if (m_e_runing == PAUSE)
	{
		return FALSE;
	}
	if (m_e_runing == STOP)
	{
		m_last_elapsed = elapsed;
		return FALSE;
	}
	kn_int time = elapsed - m_wait_time - m_pause_time - m_last_elapsed;
	if (time >= 0)
	{//超过等待时间
		kn_double  dv = 0; //运行百分比
		kn_bool ret;
		dv = getPercent(time);
		ret = updateFrame(dv);
		if (m_loop_times != LOOP_ALWAYS)
		{
			if(time > m_loop_times * (m_duration + m_loop_wait_time) )
			{//播放完了
				Stop();
			}
		}
		return ret;
	}
	return FALSE;
}

kn_bool CNUIAnimation::updateFrame(kn_double dv, kn_bool b_draw)
{
	CPropertyBase* p_pb = NULL;
	KScreen* screen = NULL;

	readLock lock(m_ani_data_mutex);
	for(CPropertyAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
	{
		CPropertyAnimationData*  p_a_data = (*ite);
		p_pb = p_a_data->getCurrent(dv);
		if (p_pb==NULL)
		{
			return FALSE;
		}
		ASSERT(p_pb->getView() ) ;
		
		if (screen==NULL)
		{//只取一次
			screen = p_pb->getView()->GetScreen();
		}
		
		if (screen ) screen->addProperty(p_pb);
	}

	if(b_draw && screen != NULL)
	{//动画外部线程管理的就不能在这里绘制
		screen->SetRenderFlag(TRUE);	
	}

	return TRUE;
}

kn_double CNUIAnimation::getPercent(kn_int current)
{//传入的current从动画开始时间算起，
	kn_double dv = 0;
	//当前循环次数,注意第一次循环该值为0
	m_current_loop = current/(m_duration+m_loop_wait_time);
	//当前周期
	kn_int current_duration = (m_duration+m_loop_wait_time)*(m_current_loop) + m_duration;
	switch (m_loop_type)
	{
	case LOOP_TIME:
		if ( m_current_loop >= m_loop_times) // current >= current_duration )
		{//播完
			if (m_loop_times != LOOP_ALWAYS)
			{
				//超出播放次数
				dv = 1;
			} 
			else
			{ //从头开始
				dv = 0;
			}
		}
		else 
		{//没播完
			//播放次数以内
			if (m_loop_times != LOOP_ALWAYS)
			{
				dv = (kn_double)(current % (m_duration + m_loop_wait_time) )/(kn_double)m_duration;
			}
			else
			{
				dv = (kn_double)(current % (m_duration) )/(kn_double)m_duration;
			}
		}
		break;
	case LOOP_PINGPONG:
		if (m_loop_times != LOOP_ALWAYS && m_current_loop >= m_loop_times) //current >= current_duration )
		{//播完
			if (m_loop_times%2 == 0 )
			{//结束点在开始
				dv = 0;
			}
			else
			{//结束点在end
				dv = 1;
			}
		}
		else
		{//没播完
			if ( m_current_loop%2 == 0 )
			{//正向播放
				dv = (kn_double)(current % (m_duration + m_loop_wait_time) )/(kn_double)m_duration;
			}
			else
			{//反向
				dv = 1 - (kn_double)(current % (m_duration + m_loop_wait_time) )/(kn_double)m_duration;
			}
		}
		break;
	}

	return dv;
}

void CNUIAnimation::setLoopTimes(kn_int n)
{
	m_loop_times = n;
}

KScreen* CNUIAnimation::getScreen()
{
	KScreen* screen = NULL;
	readLock lock(m_ani_data_mutex);
	for(CPropertyAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
	{
		CPropertyAnimationData*  p_a_data = (*ite);
		screen = p_a_data->getScreen();
		break;
	}

	return screen;
}

void CNUIAnimation::setFrameTime(kn_uint v)
{
	m_frame_time = v;
}
