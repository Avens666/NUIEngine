//  **************************************
//  File:        KTimer.cpp
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
#include "KTimer.h"
#include "KScreen.h"
#include "KView.h"
#define INVALID_TIMER_ID (-1)
#define ONE_MILLISECOND (1000000LL)

//使用全局计时器链表管理
LSTTIMER	g_lst_timer;

void CheckNUITimer()
{
	LSTTIMER::iterator it;

	for (it = g_lst_timer.begin(); it != g_lst_timer.end();)
	{
		(*it)->check();

		if ( !(*it)->isActive() )
		{//不激活的删除
			it = g_lst_timer.erase(it);
		}
		else
		{
			it++;
		}
	}

}

void AddNUITimer(KTimer_PTR v)
{
	g_lst_timer.push_back(v);
}

KTimer::KTimer()
{
	m_interval = 1000;
	m_times = -1;
	m_b_run = FALSE;
	nanosecond_type last(0);
	m_last_time = last;
	m_time = 0;
}

KTimer::KTimer(int interval, int times)
{
	m_interval = interval;
	m_times = times;
	m_b_run = FALSE;
	nanosecond_type last(0);
	m_last_time = last;
	m_time = 0;
}

KTimer::~KTimer()
{
	if (isActive())
	{
	}
}

void KTimer::setView(KView_WEAK_PTR v)
{
	m_p_view = v;
}

bool KTimer::start()
{
	if (isActive())
	{
		return FALSE;
	}

	if (m_times != 0)
	{

		nanosecond_type last(0);
		m_last_time = last;
		m_time = 0;
		m_timer.start();
		AddNUITimer(shared_from_this() );

		m_b_run = TRUE;

	}
	return m_b_run;
}

void KTimer::restart()
{
	if (!isActive())
	{
		start();
		return;
	}

	m_timer.stop();
	m_time = 0;
	m_last_time = 0;
	m_timer.start();
	m_b_run = TRUE;
}


bool KTimer::isActive() const 
{
	return m_b_run; 
}

kn_int KTimer::getInterval() const 
{ 
	return m_interval; 
}

void KTimer::setInterval(int v)
{
	m_interval = v;
}


void KTimer::setTimes(int value) 
{
	m_times = value;
}

int KTimer::getTimes() 
{ 
	return m_times; 
}


void KTimer::stop()
{
	m_b_run = FALSE;
}

bool KTimer::check() 
{
	if (!isActive())
	{
		return FALSE;
	}
	cpu_times const elapsed_times(m_timer.elapsed());
	nanosecond_type const elapsed(elapsed_times.wall);
	if (elapsed - m_last_time >= nanosecond_type(m_interval) * ONE_MILLISECOND)
	{
		if ( !m_p_view._empty() &&  m_p_view.expired())
		{//非空但是指针无效，不能触发
			stop();
		}
		else
		{//仅仅_empty()还是允许触发
			m_timeout_signal.emit(m_time+1);
			m_last_time = elapsed;
			m_time++;
			if (m_times >0 && m_time >= m_times)
			{
				stop();
			}
		}

		return TRUE;
	}
	return FALSE;
}

kn_int64 KTimer::elapsedTime()
{
	if (isActive())
	{
		return m_timer.elapsed().wall / ONE_MILLISECOND;
	}
	return 0;
}


/////////////////////////////////////////////


KThreadTimer::KThreadTimer()
	:m_pThread(0),
	m_id(INVALID_TIMER_ID),
	m_interval(0),
	m_bSingleShot(false)
{

}

KThreadTimer::~KThreadTimer()
{
	if (IsActive())
	{
		Stop();
	}
}

bool KThreadTimer::Start( TimerID id, kn_int msec )
{
	boost::this_thread::get_id();
	if (IsActive())
	{
		return false;
	}
	if (msec >= 0)
	{
		m_id = id;
		m_interval = msec;
		m_pThread = new boost::thread(ThreadProc, this);
		return true;
	}
	return false;
}


bool KThreadTimer::IsActive() const 
{
	return m_id >= 0; 
}

TimerID KThreadTimer::GetTimerID() const 
{
	return m_id; 
}
kn_int KThreadTimer::Interval() const 
{ 
	return m_interval; 
}

void KThreadTimer::SetSingleShot(bool value) 
{
	m_bSingleShot = value;
}

bool KThreadTimer::IsSingleShot() const 
{ 
	return m_bSingleShot; 
}


void KThreadTimer::Stop()
{
	if (IsActive())
	{
		m_pThread->interrupt();
		delete m_pThread;
		m_pThread = 0;
		m_interval = 0;
		m_id = INVALID_TIMER_ID;
	}
}

void KThreadTimer::ThreadProc( KThreadTimer* p )
{
	using boost::timer::cpu_times;
	using boost::timer::nanosecond_type;
	static const nanosecond_type one_milisecond(1000000LL);

	p->m_timer.start();
	nanosecond_type last(0);
	while (1)
	{
		cpu_times const elapsed_times(p->m_timer.elapsed());
		nanosecond_type const elapsed(elapsed_times.wall);
		if (elapsed - last >= nanosecond_type(p->m_interval) * ONE_MILLISECOND)
		{
			p->m_timeoutSignal.emit(p->m_id);
			if (p->m_bSingleShot)
			{
				p->m_id = INVALID_TIMER_ID;
				break;
			}
			last = elapsed;
		}
		boost::this_thread::sleep_for(boost::chrono::milliseconds(p->m_interval / 10));
	}
	delete p->m_pThread;
	p->m_pThread = 0;
	p->m_interval = 0;
	p->m_id = INVALID_TIMER_ID;
}

kn_int64 KThreadTimer::ElapsedTime()
{
	if (IsActive())
	{
		return m_timer.elapsed().wall / ONE_MILLISECOND;
	}
	return 0;
}
