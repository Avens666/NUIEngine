//  **************************************
//  File:        KScreen.cpp
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
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "KScreen.h"
#include "RERegion.h"
#include "KFontManager.h"
#ifdef WIN32
#include "glenv.h"
#endif
#include "REWindow.h"
#include "KMessage.h"
using namespace boost;
KNUIInitPara::KNUIInitPara()
{
	m_w = 1024;
	m_h = 768;
	m_format = REBitmap::kARGB_8888_Config;
	m_render_mode = SCREEN_RENDER_MAIN_THREAD;
	m_device_type = WIN_DEVICE;
	m_wnd = NULL;

}

KNUIInitPara::~KNUIInitPara()
{

}

KScreen::KScreen()
{
	m_p_screen_surface = NULL; 
	m_hwnd = 0;
	m_hdc = 0;
	m_width = 800;
	m_height = 480;
	m_p_thread = NULL;

	m_render_run_flag = FALSE;

	m_b_draw_clip = FALSE;

	m_render_flag = FALSE;

	m_render_mode = SCREEN_RENDER_MULTI_THREAD;

	m_render_device_type = WIN_DEVICE;
	m_view_group = KViewGroup_PTR(new KViewGroup);

	m_p_nui_instance = NULL;
}
 
KScreen::~KScreen()
{

	Release();
}


kn_bool KScreen::Create(KNUIInitPara& para)
{
	m_width  = para.m_w;
	m_height = para.m_h;
	m_hwnd = para.m_wnd;
	m_render_device_type = para.m_device_type;
    m_init_para = para;

#if defined(WIN32)
	m_hdc = GetDC(m_hwnd);

	switch (para.m_device_type)
	{
	case WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateDeviceReSurface(m_hdc, m_width,m_height,para.m_format);
		break;
	case GL_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLReSurface(m_hdc, m_width,m_height,para.m_format);
		break;
	case SKIAGL_DEVICE: // add by boy
		m_p_screen_surface = RESurfaceFactory::CreateSkiaGLReSurface(m_hwnd, m_width,m_height,para.m_format);
		break;
	case DDRAW_DEVICE: //DDraw上屏， cpu占用无优势
		m_p_screen_surface = RESurfaceFactory::CreateDDrawReSurface(m_hwnd, m_width,m_height,para.m_format);
		break;
	case LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,para.m_format);
		break;
	case GL_LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,para.m_format);
		break;
	}


	bool istouchsupport = CheckTouchSupport(m_hwnd);
#elif defined(ANDROID_NDK)
	if (para.m_device_type == ANDROID_DEVICE)
	{
 		m_p_screen_surface = RESurfaceFactory::CreateAndroidReSurface((char*)m_hwnd, m_width,m_height, para.m_format);

	}
#elif defined(IOS_DEV)
	if (para.m_device_type == IOS_DEVICE)
	{
        void* pWnd = 0;
 		m_p_screen_surface = RESurfaceFactory::CreateIOSReSurface(pWnd, m_width,m_height, para.m_format);
 
        m_hwnd = pWnd;
        para.m_wnd = pWnd;
	}

#elif defined(QT_HMI)
    if(para.m_device_type == QT_SOFT_DEVICE)
    {
        m_p_screen_surface = RESurfaceFactory::CreateQtSoftReSurface(m_hwnd, m_width,m_height, para.m_format);
    }
#endif


	m_view_group->Create(0,0,m_width,m_height);
	m_view_group->SetScreen(this);

	m_render_mode = para.m_render_mode;
	if (m_render_mode == SCREEN_RENDER_MULTI_THREAD)
	{
		RunRenderThread(); 
	}
	return true;
}

void  KScreen::resize(int w, int h)
{
	//先停止渲染
	stopRenderThread();

	SAFE_DELETE(m_p_screen_surface);

	m_width  = w;
	m_height = h;

#ifdef WIN32
	switch (m_render_device_type)
	{
	case WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateDeviceReSurface(m_hdc, m_width,m_height,m_init_para.m_format);
		break;
	case GL_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLReSurface(m_hdc, m_width,m_height,m_init_para.m_format);
		break;
	case DDRAW_DEVICE: //DDraw上屏， cpu占用无优势
		m_p_screen_surface = RESurfaceFactory::CreateDDrawReSurface(m_hwnd, m_width,m_height,m_init_para.m_format);
		break;
	case LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,m_init_para.m_format);
		break;
	case GL_LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,m_init_para.m_format);
		break;
	case SKIAGL_DEVICE: // add by boy
		m_p_screen_surface = RESurfaceFactory::CreateSkiaGLReSurface(m_hwnd, m_width,m_height,m_init_para.m_format);
		break;

	}

#elif defined(ANDROID_NDK)
	if (m_render_device_type == ANDROID_DEVICE)
	{
 		m_p_screen_surface = RESurfaceFactory::CreateAndroidReSurface((char*)m_hwnd, m_width,m_height, m_init_para.m_format);

	}
    
#elif defined(IOS_DEV)
	if (m_render_device_type == IOS_DEVICE)
	{
        
        void* pWnd = 0;  // GL 不需要该参数
 		m_p_screen_surface = RESurfaceFactory::CreateIOSReSurface(pWnd, m_width,m_height, m_init_para.m_format);
        m_hwnd = pWnd;
        
	}

#elif defined(QT_HMI)
    if(m_render_device_type == QT_SOFT_DEVICE)
    {
        m_p_screen_surface = RESurfaceFactory::CreateQtSoftReSurface(m_hwnd, m_width,m_height, m_init_para.m_format);
    }
#endif

	m_view_group->resize( RERect::MakeWH(w, h));
	if (m_render_mode == SCREEN_RENDER_MULTI_THREAD)
	{
		RunRenderThread(); 
	}

	SetRenderFlag(TRUE);
}

///废弃，仅为兼容保留
kn_bool KScreen::Create(kn_int w, kn_int h, REBitmap::Config format, HWND hWnd,  kn_int render_mode, kn_int device_type )
{

	m_width  = w;
	m_height = h;
	m_hwnd = hWnd;

#ifdef WIN32
	m_hdc = GetDC(hWnd);

	m_render_device_type = device_type;
	switch (device_type)
	{
	case WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateDeviceReSurface(m_hdc, m_width,m_height,format);
		break;
	case GL_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLReSurface(m_hdc, m_width,m_height,format);
		break;
	case LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,format);
		break;
	case GL_LAYER_WIN_DEVICE:
		m_p_screen_surface = RESurfaceFactory::CreateGLLayerDeviceReSurface(m_hwnd, m_hdc, m_width,m_height,format);
		break;
	}

#endif
	
	//DDraw上屏， cpu占用无优势
	//	m_p_screen_surface = RESurfaceFactory::CreateDDrawReSurface(m_hwnd, m_width,m_height,format);
	m_view_group->Create(0,0,m_width,m_height);
	m_view_group->SetScreen(this);

	m_render_mode = render_mode;
	if (render_mode == SCREEN_RENDER_MULTI_THREAD)
	{
		RunRenderThread(); 
	}
	return true;
}
 
void KScreen::Release()
{
	releaseAnimationThreadRun();//释放统一动画线程 zhic
	stopRenderThread();

	//此处手动释放，要保证所有动画线程在m_data_sync析构前退出
	m_view_group->Release();

	m_data_sync.release();

	SAFE_DELETE(m_p_screen_surface);

	DeleteFontManagerSingleton();
}

void KScreen::stopRenderThread()
{
	if (m_p_thread)
	{
		if (m_render_run_flag)
		{
			m_p_thread->interrupt();
		}

		//while(m_render_run_flag)
		//{
		//	Sleep(2);
		//}
		//修改为有限等待，否则底层出错就无法退出
		m_p_thread->timed_join(posix_time::seconds(5));

		SAFE_DELETE(m_p_thread);
	}
}

 int g_tt = FALSE;
void KScreen::Draw(kn_bool bFullRefresh)
{
	//MyTraceA("KScreen::Draw ....");
	LSTRECT rectScreen;
	//bFullRefresh=1;
	//预处理，gl渲染时需要，软渲染不处理
	m_p_screen_surface->preDraw();
	if(!m_b_draw_clip)
	{
		m_p_screen_surface->DrawColor(SkColorSetARGB(0,0,0,0), SkXfermode::kSrc_Mode);
		m_view_group->Draw(m_p_screen_surface, 0, 0);
		if( m_view_menu && m_view_menu->isShow() ) m_view_menu->Draw(m_p_screen_surface, 0, 0);
		if( m_view_tip && m_view_tip->isShow() ) m_view_tip->Draw(m_p_screen_surface, 0, 0);
	}
	else
	{
		rectScreen = m_view_group->GetInvalidRect();
		RERectHelper::Merge(rectScreen);
		if (rectScreen.size() > 0)
		{
			m_p_screen_surface->Save();
			m_p_screen_surface->ClipRect(*(rectScreen.begin()));
			for(LSTRECT::iterator ite = rectScreen.begin();ite!=rectScreen.end();ite++)
			{
				m_p_screen_surface->ClipRect((*ite), RERegion::kUnion_Op);
			}
			m_p_screen_surface->DrawColor(RE_ColorGRAY);
			m_view_group->Draw(m_p_screen_surface, rectScreen, 0, 0);

			m_p_screen_surface->Restore();
		}
	}

	if(g_tt)
	{
        m_p_screen_surface->WriteToFile(_T("d:/test.png"));
        g_tt = FALSE;
	}
	Flip(rectScreen);
}
 
void KScreen::Flip(LSTRECT &lst_rect)
{

	m_p_screen_surface->Flip(lst_rect);
}

//是否局部刷新
void KScreen::setClipDraw(kn_bool b)
{
	m_b_draw_clip = b;
}
 
KView_PTR KScreen::getModalView()
{
	if (m_lst_modal_view.size() > 0)
	{
		return *(m_lst_modal_view.rbegin() ); 
	}
	KView_PTR p;
	return p;
}
kn_bool KScreen::DoMessage(KMessage* pMsg)
{
	bool ret = FALSE;
	if (m_render_mode == SCREEN_RENDER_MAIN_THREAD && pMsg->m_msg_type == KMSG_SCREEN_DRAW)
	{
		if ( GetRenderRunFlag() == FALSE) 
		{//非异步刷新机制，可依靠消息驱动
			OnDraw();
		}
		return TRUE;
	}

	//有菜单弹出,只处理菜单，不是最优解决方案，以后需要优化
	if (m_view_menu && pMsg->m_msg_class_type != KMSG_TYPE_USER)
	{
		ret = m_view_menu->DoMessage(pMsg);
		if ( pMsg->HasPos() 
			&& (pMsg->m_msg_type == KMSG_LBBUTTONUP ||  pMsg->m_msg_type == KMSG_RBBUTTONUP) 
			&& pMsg->m_wParam != NO_HIDE_MENU ) //设置为NO_HIDE_MENU 不隐藏菜单，有时候菜单有内部迁移
		{
			if( m_view_menu->isAutoHide() || !(m_view_menu->isPointInView( ((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y )) )
			{
				m_view_menu.reset();
				SetRenderFlag(TRUE);
			}
		}
	}
	else
	{

		// 避免m_p_modal_view DoMessage时 被置NULL
		KView_PTR p_modal_view = getModalView();
		bool b_temp = FALSE;
		//聚焦控件全局处理
		if (m_focus_view)
		{
			m_temp_focus_view.push_back(m_focus_view); //使用链表防止domodal的消息嵌套
			//if (m_temp_focus_view == NULL)
			//{
			//	m_temp_focus_view = m_focus_view;
				b_temp = TRUE;
			//}
			
			ret |=  m_focus_view->DoFocusMessage(pMsg);
			
			//聚焦控件消息不屏蔽后续控件处理 zhic
			//m_focus_view->DoFocusMessage(pMsg);
		}

		if(p_modal_view != NULL ) 
		{// 仅模式对话框接收消息
			if (p_modal_view != m_focus_view)
			{//防止和聚焦view重复				
				ret |= p_modal_view->DoFocusMessage(pMsg);
			}   
			
			if (pMsg->m_msg_class_type == KMSG_TYPE_USER)
			{// 暂不屏蔽自定义消息
				ret |= m_view_group->DoMessage(pMsg);
			}
		}
		else if( !(ret && pMsg->HasPos()) ) //聚焦控件处理 就不再传递
		{
			ret |= m_view_group->DoMessage(pMsg);
		}

		if (b_temp)
		{
			m_temp_focus_view.pop_back();
	//		m_temp_focus_view = NULL;
		}
	}

//	delViews();
	return ret;
}
 
kn_int KScreen::GetWidth()
{
	return m_width;
}
 
kn_int KScreen::GetHeight()
{
	return m_height;
}

kn_bool KScreen::GetRenderFlag()
{
	return m_render_flag;
}


void	KScreen::SetRenderFlag(kn_bool b)
{
	if (m_render_flag == b )
	{
		return;
	}


	writeLock lock(m_render_flag_mutex);
	m_render_flag = b;	


	if ( !GetRenderRunFlag() && m_render_mode == SCREEN_RENDER_MAIN_THREAD)
	{//多线程未启动，通过发送消息通知主线程刷新
		if(m_render_flag) 
		{

			sendNUIMessage(KMSG_SCREEN_DRAW);
		}
	}

}

void KScreen::OnDraw()
{
	SetRenderFlag(FALSE);
	UI_DoSyncDataList();
	Draw(TRUE);
}

kn_bool KScreen::GetRenderRunFlag()
{
	return m_render_run_flag;
}

boost::mutex render_run_flag_mutex;
void KScreen::SetRenderRunFlag(kn_bool b)
{
	boost::mutex::scoped_lock lock(render_run_flag_mutex);
	m_render_run_flag = b;
}

void KScreen::UI_DoSyncDataList()
{
	CPropertyList* pl = m_data_sync.getDataList();
	if (pl)
	{
		pl->DoChange();
		SAFE_DELETE(pl);	
	}

}
#ifdef WIN32
extern CGLEvn g_nui_gl_env;
#endif
kn_int KScreen::renderThread(LPVOID lpPara)
{
	KScreen* p = (KScreen*)lpPara;
	p->SetRenderRunFlag(TRUE);

#ifdef WIN32

	if (p->m_render_device_type ==GL_DEVICE )
	{
		g_nui_gl_env.glenvInit(p->m_hdc, 0,0, p->m_width, p->m_height);
	}

	if (p->m_render_device_type ==GL_LAYER_WIN_DEVICE )
	{
		g_nui_gl_env.glenvInit(p->m_hdc, 0,0, p->m_width, p->m_height);
		//g_nui_gl_env.glenvInit( ((REGLLayerWinDeviceSurface*)(p->m_p_screen_surface))->getMemDC(), 0,0, p->m_width, p->m_height, TRUE);
	}

	if( p->m_render_device_type == SKIAGL_DEVICE )
	{
		p->m_p_screen_surface->Initialize(p->m_width, p->m_height, SkBitmap::Config(0));
	}
#endif

	try
	{
		while(p->GetRenderRunFlag())
		{
			if (p->GetRenderFlag())
			{
				// 要最开始就设置flag，后面的处理耗时，不及时设置会冲掉后续的申请
				p->SetRenderFlag(FALSE);

				DWORD t0= GetTickCount();
				boost::this_thread::interruption_point();
				p->UI_DoSyncDataList();
				boost::this_thread::interruption_point();

				DWORD t1 = GetTickCount();

				 p->Draw(TRUE);
 
				DWORD t2 = GetTickCount();

				p->dealAniStopMsg();
				//MyTraceA("renderThread syncdatalist %d, draw %d", t1 - t0, t2 - t1);
			}
			boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
		}
	}
	catch (boost::thread_interrupted &e)
	{
		p->SetRenderRunFlag(FALSE);
	}
		p->SetRenderRunFlag(FALSE);
	return 1;
}

void KScreen::addStopId(kn_int& id, kn_int& msg_id)
{
	writeLock lock(m_stop_id_mutex);
	m_stop_id_map.insert(make_pair(id, msg_id));
}

//在渲染线程中执行
void KScreen::dealAniStopMsg()
{
	writeLock lock(m_stop_id_mutex);
	map<kn_int, kn_int>::iterator iter = m_stop_id_map.begin();
	for (; iter != m_stop_id_map.end(); )
	{
		kn_int id = iter->first;
		if (!checkID(id))
		{//渲染队列没有动画线程对应的id
			kn_int msg = iter->second;
			m_stop_id_map.erase(iter++);
			sendNUIMessage(msg);
		}
		else
		{
			iter++;
		}
	}

}

void	KScreen::RunRenderThread()
{
	if ( GetRenderRunFlag() == FALSE )
	{
		//boost::thread thrd1(boost::bind(&RenderThread, this));
		m_p_thread = new boost::thread(renderThread, this);
	}
}

void	KScreen::AddView(KView_PTR p)
{
	m_view_group->AddView(p);
}

void	KScreen::DeleteView(KView_PTR p)
{
	m_view_group->AddViewToDel(p);
}

kn_bool KScreen::addProperty(CPropertyBase* p, kn_bool b_check_change)
{
	kn_bool b = FALSE;
	if (!b_check_change || p->isChange()) //b_check_change是否检查值是否相等，某些异步情况下设置要使用
	{
		m_data_sync.addProperty(p);
		b= TRUE;
	}
	else
	{//不变别忘了删除
		SAFE_DELETE(p);
	}
	return b;
}


//获得一个新id
kn_int KScreen::getNewID()
{
	return m_data_sync.getNewID();
}
//检查ID是否存在
kn_bool KScreen::checkID(kn_int& n)
{
	return m_data_sync.checkID(n);
}

//加入ID标识
void KScreen::addID(kn_int& n)
{
	m_data_sync.addID(n);
}

//发送自定义消息，不同的操作系统实现不同
void KScreen::sendNUIMessage(kn_int msgid, KMessage* msg)
{
//#ifdef WIN32
//	if (msg== NULL)
//	{
//		msg = new KMessage;
//	}
//	msg->m_msg_type = (KMESSAGETYPE)msgid;
//	msg->m_msg_class_type = KMSG_TYPE_USER;
//
//	PostMessage(m_hwnd, KNUIMSG_USER, (WPARAM)msg, NULL);
//#endif

//#if defined(QT_HMI) || defined(ANDROID_NDK) || defined(IOS_DEV)

    if (msg== NULL)
    {
        msg = new KMessage;
    }
    msg->m_msg_type = (KMESSAGETYPE)msgid;
    msg->m_msg_class_type = KMSG_TYPE_USER;

    SendKMessage(msg);

 
//#endif
}

void  KScreen::sendSystemMessage(kn_int msgid,  WPARAM w, LPARAM l )
{
	PostMessage(m_hwnd, msgid, w, l);
}

void KScreen::addModalView(KView_PTR pView)
{
	m_lst_modal_view.push_back( pView );
}

void KScreen::delModalView(KView_PTR pView)
{
	if (pView == NULL)
	{
		return;
	}
	for(LSTVIEW::iterator  it = m_lst_modal_view.begin(); it != m_lst_modal_view.end(); it++)
	{
		if ( *it == pView)
		{
			m_lst_modal_view.erase(it);
			break;
		}
	}
}

void KScreen::clearModalView()
{
	//for(LSTVIEW::iterator  it = m_lst_modal_view.begin(); it != m_lst_modal_view.end(); it++)
	//{
	//	(*it)->decRef();
	//}
	m_lst_modal_view.clear();
}

HWND  KScreen::getWnd()
{
	return m_hwnd;
}

void KScreen::setWnd(HWND wnd)
{
	m_hwnd = wnd;
}

void KScreen::setNUIInstance(CNUIInstance* p)
{
	m_p_nui_instance = p;
}

CNUIInstance* KScreen::getNUIInstance()
{
	return m_p_nui_instance;
}


void KScreen::setFocusView(KView_PTR v, kn_bool b)
{
	if (b && m_focus_view && m_focus_view != v)
	{//切换新控件
		m_focus_view->unSetViewFocus();
	}

	m_focus_view = v;

}

void KScreen::setViewActive(KView_PTR v, kn_bool b)
{
	//active 的处理有些问题 现在也没用到
	//if ( b && m_active_view && m_active_view != v)
	//{//取消老的,传入false 内部不进行循环设置
	//	m_active_view->unSetViewActive(FALSE);
	//}

	//if (v)
	//{
	//	m_active_view = v;
	//}
	//else
	//{
	//	m_active_view.reset();

	//}
}

KView_PTR KScreen::getFocusView()
{
	return m_focus_view;
}

KView_PTR KScreen::getTempFocusView()
{
	if (m_temp_focus_view.size() > 0)
	{
		return m_temp_focus_view.back();
	}
	KView_PTR p;
	return p;
}


KView_PTR KScreen::getActiveView()
{
	return m_active_view;
}

void KScreen::showMenu(KMenuBase_PTR p)
{
	m_view_menu = p;
	if (m_view_menu)
	{
		m_view_menu->SetScreen(this);
	}

}

void KScreen::showTip(KTipBase_PTR p)
{
	m_view_tip = p;
	m_view_tip->SetScreen(this);
}

void KScreen::hideTip(int id)
{
	if (m_view_tip)
	{
		if (id == NO_TIMER || m_view_tip->getTipID() == id)
		{//防止A的tip被B的消息影响
			m_view_tip.reset();
		}	
	}

}
