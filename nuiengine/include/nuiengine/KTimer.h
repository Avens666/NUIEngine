//  **************************************
//  File:        KTimer.h
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
#ifndef KTimer_DEFINED
#define KTimer_DEFINED

#include "NE_type.h"
#include <boost/enable_shared_from_this.hpp>  
#include <boost/timer/timer.hpp>
#include <boost/thread.hpp>
#include "sigslot.h"
//#include "KView.h"
class KScreen;
typedef kn_int TimerID;
using boost::timer::cpu_times;
using boost::timer::nanosecond_type;

class KView;
typedef boost::weak_ptr<KView> KView_WEAK_PTR;

class NUI_API KTimer:public boost::enable_shared_from_this<KTimer>
{
public:
	KTimer(); 
	KTimer(int interval, int times = -1); //间隔时间 千分之一秒 和触发次数 -1无限次  
	~KTimer();

	bool isActive() const; //是否活动 start开始活动 stop则不活动

	void setInterval(int v);
	kn_int getInterval() const;

	void setTimes(int value);
	int getTimes();

	bool start(); //如果不传入screen，使用m_p_view的screen，如果m_p_view也为空，则启动无效
	void restart(); 
	void stop();
	bool check(); //定时检查函数

	kn_int64 elapsedTime();

	//设置view，更安全，不设置，需要上层保证定时器调用时对应的回调对象是有效的
	void setView(KView_WEAK_PTR);
	typedef sigslot::signal1<kn_int> TimeOutSignal;	
	TimeOutSignal m_timeout_signal;

private:
	KView_WEAK_PTR m_p_view;
	boost::timer::cpu_timer m_timer;

	kn_int m_interval;
	int m_times;
	int m_time; // 运行次数
	kn_bool m_b_run;
	nanosecond_type m_last_time;
};
typedef boost::shared_ptr<KTimer> KTimer_PTR;
typedef std::list<KTimer_PTR> LSTTIMER;

//全局的插入函数
void AddNUITimer(KTimer_PTR);
void CheckNUITimer();
/////////////////////////////////////////////

class NUI_API KThreadTimer:public boost::noncopyable
{
public:
	KThreadTimer();
	~KThreadTimer();



	// 该函数不宜内联
	bool IsActive() const;
	TimerID GetTimerID() const;
	kn_int Interval() const;

	void SetSingleShot(bool value);
	bool IsSingleShot() const;

	bool Start(TimerID id, kn_int msec);
	void Stop();
	kn_int64 ElapsedTime();

	typedef sigslot::signal1<kn_int> TimeOutSignal;	
	TimeOutSignal m_timeoutSignal;

private:
	static void ThreadProc(KThreadTimer* p);

	boost::timer::cpu_timer m_timer;
	boost::thread* m_pThread;
	TimerID m_id;
	kn_int m_interval;
	bool m_bSingleShot;
};


#endif
