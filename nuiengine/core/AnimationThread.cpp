//  **************************************
//  File:        AnimationThread.cpp
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
#include "AnimationThread.h"
#include "KView.h"
#include "KScreen.h"
#include "message.h"

////////////////动画线程单例/////////////
CAnimationThreadRun* g_AnimationThreadRun = NULL;
CAnimationThreadRun* getAnimationThreadRun()
{
	if (!g_AnimationThreadRun)
	{
		g_AnimationThreadRun = new CAnimationThreadRun();
	}

	return g_AnimationThreadRun;
}

void releaseAnimationThreadRun()
{
	SAFE_DELETE(g_AnimationThreadRun);
}

////////////////////////// CAnimationThreadRun ///////////////////
CAnimationThreadRun::CAnimationThreadRun(void)
{
	m_frame_time = 30;
	m_b_run = FALSE;

	m_pThread = new boost::thread(AnimationThreadProc, this);
}

CAnimationThreadRun::~CAnimationThreadRun(void)
{
	if (m_pThread)
	{
		if (m_b_run)
		{
			m_pThread->interrupt();
		}

		while(m_b_run)
		{
			Sleep(2);
		}

		SAFE_DELETE(m_pThread);
	}

}

void  CAnimationThreadRun::addAnimationThread(CAnimationThread* p)
{
	writeLock lock(m_ani_data_mutex);
	p->runInit();
	m_ani_data_lst.push_back(p);
}

void CAnimationThreadRun::removeAnimationThread(CAnimationThread* p)
{
	readLock lock_read(m_ani_data_mutex);

	for(CNUIAnimationThreadList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();)
	{
		//在循环中要清除已stop的动画请求
		if ((*ite) == p)
		{
			lock_read.unlock();
			writeLock lock(m_ani_data_mutex);
			m_ani_data_lst.erase(ite++);
			lock.unlock();

			lock_read.lock();
		}
		else
		{
			++ite;
		}
	}
	lock_read.unlock();
}

void CAnimationThreadRun::AnimationThreadProc(CAnimationThreadRun* p)
{
	p->m_b_run = TRUE;
	writeLock lock(p->m_ani_data_mutex);
	lock.unlock();
	try
	{
		while(1)
		{
			lock.lock();
			for(CNUIAnimationThreadList::iterator ite = p->m_ani_data_lst.begin();ite!=p->m_ani_data_lst.end();)
			{
				(*ite)->run();

				//在循环中要清除已stop的动画请求
				if ((*ite)->getRunState() == CAnimationThread::STOP)
				{
					p->m_ani_data_lst.erase(ite++);
				}
				else
				{
					++ite;
				}
				boost::this_thread::interruption_point();
			}
			lock.unlock();

			boost::this_thread::sleep_for(boost::chrono::milliseconds(p->m_frame_time));
		}
	}
	catch (boost::thread_interrupted &e)
	{
		p->m_b_run = FALSE;

	}

	p->m_b_run = FALSE;
}

///////////////////////////CAnimationThread///////////////////////////////////////////////////

CAnimationThread::CAnimationThread(void)
{
	init();

}

void CAnimationThread::init()
{
	m_e_runing = STOP;
	m_auto_clear_ani = TRUE;
	m_stop_msg = -1;

	//每帧间隔时间 毫秒单位
	m_frame_time = 40;

	m_p_screen = NULL;
	m_last = 0;
	m_p_timer = NULL;
	m_ani_id = 0;
	m_b_update = FALSE;
	m_b_pause = FALSE;
}


CAnimationThread::~CAnimationThread(void)
{
	Stop();
	clearAnimation();
}

kn_bool CAnimationThread::clearAnimation()
{
	if ( m_e_runing == STOP )
	{
		writeLock lock(m_ani_data_mutex);
		for(CNUIAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
		{
			SAFE_DELETE(*ite);
		}
		m_ani_data_lst.clear();
		//		MyTrace(_T("clearAnimation ") );
	}

	return FALSE;
}

kn_bool  CAnimationThread::addAnimation(CNUIAnimation* p)
{
	if ( m_e_runing == STOP )
	{
		writeLock lock(m_ani_data_mutex);
		p->Play();
		m_ani_data_lst.push_back(p);
		return TRUE;
	}

	return FALSE;
}

void CAnimationThread::setStopMsg(kn_int id)
{
	m_stop_msg = id;
}

void CAnimationThread::Stop()
{
	if ( m_e_runing != STOP )
	{
		setRunState(STOP);
		getAnimationThreadRun()->removeAnimationThread(this);
	}
	SAFE_DELETE(m_p_timer);
}


void CAnimationThread::Start(RunState e)
{
	if ( m_e_runing == STOP )
	{
		if (m_ani_data_lst.size()>0)
		{
			getAnimationThreadRun()->addAnimationThread(this);
		}
	}
}

void CAnimationThread::setRunState(RunState s)
{
	boost::mutex::scoped_lock lock(m_runing_mutex);
	m_e_runing = s;
}

void CAnimationThread::Play()
{
	setRunState(PLAYING);
}

void CAnimationThread::Pause()
{
	setRunState(PAUSE);
}



void CAnimationThread::setAutoClearAni(kn_bool b)
{
	m_auto_clear_ani = b;
}

CAnimationThread::RunState CAnimationThread::getRunState()
{
	return m_e_runing;
}

KScreen* CAnimationThread::getScreen()
{
	KScreen* screen = NULL;
	readLock lock(m_ani_data_mutex);
	for(CNUIAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
	{
		CNUIAnimation*  p_a_data = (*ite);
		screen = p_a_data->getScreen();
		break;
	}

	return screen;
}

kn_bool CAnimationThread::isPlaying()
{//返回播放状态
	return  ( getRunState() == CAnimationThread::PLAYING );
}

void  CAnimationThread::waitAnimationStop()
{
	while(getRunState() == CAnimationThread::PLAYING)
	{
		Sleep(50);
	}
}

CNUIAnimation* CAnimationThread::addAnimationHelper(KView_PTR view, kn_int PARA_ID, double v, kn_int duration, kn_int wait_time, kn_int loop_wait_time, KEasingCurve::Type type)
{
	CNUIAnimation* pNUIAnimation = new CNUIAnimation (duration, wait_time, loop_wait_time);
	CPropertyPara* para = new CPropertyPara(view,  PARA_ID, v);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(para, type);
	pNUIAnimation->addAnimationData(pAnimationData);
	addAnimation(pNUIAnimation);
	return pNUIAnimation;
}

CNUIAnimation* CAnimationThread::addAnimationHelper(KView_PTR view, kn_int PARA_ID, double v1,double v2, kn_int duration, kn_int wait_time, kn_int loop_wait_time, KEasingCurve::Type type)
{
	CNUIAnimation* pNUIAnimation = new CNUIAnimation (duration, wait_time, loop_wait_time);
	CPropertyPara* para1 = new CPropertyPara(view,  PARA_ID, v1);
	CPropertyPara* para2 = new CPropertyPara(view,  PARA_ID, v2);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(para1, para2, type);
	pNUIAnimation->addAnimationData(pAnimationData);
	addAnimation(pNUIAnimation);
	return pNUIAnimation;
}

CNUIAnimation* CAnimationThread::addRectAnimationHelper(KView_PTR view, RERect rect, kn_int duration, kn_int wait_time, kn_int loop_wait_time, KEasingCurve::Type type)
{

	CNUIAnimation* pNUIAnimation = new CNUIAnimation (duration, wait_time, loop_wait_time);
	CPropertyRect* para1 = new CPropertyRect(view, rect);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(para1, type);
	pNUIAnimation->addAnimationData(pAnimationData);
	addAnimation(pNUIAnimation);
	return pNUIAnimation;
}

//run的预处理
void CAnimationThread::runInit()
{
	m_p_screen = getScreen();
	setRunState(PLAYING);

	m_last = 0;

	if (m_p_screen == NULL)
	{
		return;
	}
	m_ani_id = m_p_screen->getNewID();

	//动画启动时间
	if (m_p_timer)
	{
		m_p_timer->start();
	}
	else
	{
		m_p_timer = new boost::timer::cpu_timer();
	}

	m_b_update = FALSE;
	m_b_pause = FALSE;

}

//运行一帧
void CAnimationThread::run()
{
	if (m_e_runing == STOP || m_p_screen == NULL || m_p_timer == NULL)
	{
		return;
	}
	//处理暂停
	if (m_e_runing == PAUSE)
	{
		if (m_b_pause == FALSE)
		{//第一次暂停
			m_p_timer->stop();
			m_b_pause = TRUE;
		}
		return;
	}

	if (m_b_pause && m_e_runing == PLAYING)
	{//从暂停恢复
		m_b_pause = FALSE;
		m_p_timer->resume();
	}


	if ( m_p_screen->checkID(m_ani_id) )
	{//如果前一帧id还存在，代表渲染线程没有处理完，则不进行动画处理，等待上次请求处理完再处理
		return;
	}

	cpu_times const elapsed_times(m_p_timer->elapsed());
	kn_uint elapsed = (elapsed_times.wall)/(1000000LL) ;

	BOOL b_all_stop = FALSE;
	if (elapsed - m_last >= m_frame_time ) //(1000000LL) 1微秒
	{
		m_b_update = FALSE;

		b_all_stop = TRUE;
		readLock lock(m_ani_data_mutex);
		for(CNUIAnimationDataList::iterator ite = m_ani_data_lst.begin();ite!=m_ani_data_lst.end();ite++)
		{
			boost::this_thread::interruption_point();
			CNUIAnimation*  p_a_data = (*ite);
			m_b_update |= p_a_data->onPlay(elapsed);
			if ( p_a_data->getRunState() != CNUIAnimation::STOP )
			{
				b_all_stop = FALSE; 
			}
		}

		lock.unlock();

		if (m_b_update)
		{//有更新需求才提交
			//加入当前帧动画标识，
			m_p_screen->addID(m_ani_id);
			m_p_screen->SetRenderFlag(TRUE);
		}
		m_last = elapsed;
	}

	if ( b_all_stop)
	{//全部动画stop后线程退出
		setRunState(STOP);
		if (m_auto_clear_ani)
		{
			clearAnimation();
		}

		//发送动画完成消息
		if (m_stop_msg > KNUIMSG_USER )
		{
			m_p_screen->sendNUIMessage(m_stop_msg);
		}
	}
}

void CAnimationThread::setFrameTime(kn_uint v )
{
	m_frame_time = v;
}


///////////////////// CAnimationOwnThread ///////////////////////////



CAnimationOwnThread::CAnimationOwnThread(void)
{


	m_pThread = NULL;
}


CAnimationOwnThread::~CAnimationOwnThread(void)
{
	Stop();
	clearAnimation();
}

void CAnimationOwnThread::releaseThread()
{
	SAFE_DELETE(m_pThread);
}

void CAnimationOwnThread::Stop()
{
	if (m_pThread)
	{
		if (m_e_runing != STOP)
		{
			m_pThread->interrupt();
		}

		while(m_e_runing != STOP)
		{
			Sleep(2);
		}

		//SAFE_DELETE(m_pThread);
	}
}

void CAnimationOwnThread::Start(RunState e)
{
	if ( m_e_runing == STOP )
	{
		//boost::thread thrd1(boost::bind(&AnimationThreadProc, this));
		m_pThread = new boost::thread(AnimationThreadProc, this, e );
	}
}


void CAnimationOwnThread::AnimationThreadProc(CAnimationOwnThread* p, CAnimationThread::RunState e)
{
	ASSERT(p);
	if (p==NULL) return;
	kn_int elapsed;
	kn_bool b_all_stop = FALSE;
	KScreen* screen = p->getScreen();
	kn_int ani_id = 0;
	try
	{
		p->setRunState(e);

		using boost::timer::cpu_times;
		using boost::timer::nanosecond_type;

		nanosecond_type last(0);
		//动画启动时间

		boost::timer::cpu_timer timer;
		timer.start();


		if (screen == NULL)
		{
			ASSERT(screen);
			return;
		}
		ani_id = screen->getNewID();
		kn_bool b_update = FALSE;
		kn_bool b_pause = FALSE;

		readLock lock(p->m_ani_data_mutex);
		lock.unlock();

		while (1)
		{
			//处理暂停
			if (p->m_e_runing == PAUSE)
			{
				if (b_pause == FALSE)
				{//第一次暂停
					timer.stop();
					b_pause = TRUE;
				}
				boost::this_thread::sleep_for(boost::chrono::milliseconds(p->m_frame_time/5));
				continue;
			}

			if (b_pause && p->m_e_runing == PLAYING)
			{//从暂停恢复
				b_pause = FALSE;
				timer.resume();
			}


			if ( screen->checkID(ani_id) )
			{//如果前一帧id还存在，代表渲染线程没有处理完，则不进行动画处理，等待上次请求处理完再处理
				boost::this_thread::sleep_for(boost::chrono::milliseconds(p->m_frame_time/5));
				continue;
			}

			cpu_times const elapsed_times(timer.elapsed());
			elapsed = (elapsed_times.wall)/(1000000LL) ;
			if (elapsed - last >= p->m_frame_time ) //(1000000LL) 1微秒
			{
				b_update = FALSE;
				b_all_stop = TRUE;
				lock.lock();
				for(CNUIAnimationDataList::iterator ite = p->m_ani_data_lst.begin();ite!=p->m_ani_data_lst.end();ite++)
				{
					boost::this_thread::interruption_point();
					CNUIAnimation*  p_a_data = (*ite);
					b_update |= p_a_data->onPlay(elapsed);
					if ( p_a_data->getRunState() != CNUIAnimation::STOP )
					{
						b_all_stop = FALSE; 
					}
				}

				lock.unlock();

				if (b_update)
				{//有更新需求才提交
					//加入当前帧动画标识，
					screen->addID(ani_id);
					screen->SetRenderFlag(TRUE);
				}
				last = elapsed;
			}

			if (p->m_auto_kill_thread && b_all_stop)
			{//全部动画stop后线程退出
				break;
			}

			boost::this_thread::sleep_for(boost::chrono::milliseconds(p->m_frame_time));
		}
	}
	catch (boost::thread_interrupted &ex)
	{
		//	 p->setRunState(STOP);
	}
	p->releaseThread();

	p->setRunState(STOP);
	if (p->m_auto_clear_ani)
	{
		p->clearAnimation();
	}

	// p->Stop();
	//发送动画完成消息
	if (p->m_stop_msg > KNUIMSG_USER )
	{
		//zhic 20151213 处理动画播放结束消息，由动画线程发送修改为在渲染线程中发送此消息 保证获得消息后所有的动画渲染完成
		//screen->sendNUIMessage(p->m_stop_msg);
		screen->addStopId(ani_id, p->m_stop_msg);
	}
}

void CAnimationOwnThread::setAutoKillThread(kn_bool b)
{
	//boost::mutex::scoped_lock lock(m_runing_mutex);
	m_auto_kill_thread = b;
}
