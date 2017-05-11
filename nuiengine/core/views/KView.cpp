//  **************************************
//  File:        KView.cpp
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
#include "KView.h"
#include "DataSync.h"
#include "KScreen.h"
#include "KTextView.h"
#include "KViewGroup.h"
#include "boost/lexical_cast.hpp"
#include "KFontManager.h"
using namespace std;
using namespace boost;
using namespace boost::gregorian;

int g_view_count = 0;
KView::KView()
{
	g_view_count++;
//	m_ref = 0;
	m_e_viewtype = KVIEW_BASE;

	m_b_focus = false;
	m_b_active = false;
//	m_p_parent = NULL;
	m_p_screen = NULL;
	m_b_do_msg = TRUE;
	m_shield_msg = TRUE;
	//m_sign_move.connect(this, &KView::OnMove);
	//m_sign_down.connect(this, &KView::OnDown);
	//m_sign_up.connect(this, &KView::OnUp);
	//m_sign_dclick.connect(this, &KView::OnDClick);
	//m_sign_rdown.connect(this, &KView::OnRDown);
	//m_sign_rup.connect(this, &KView::OnRUp);
	//m_sign_rdclick.connect(this, &KView::OnRDClick);
	//m_sign_wheel_down.connect(this, &KView::OnWheel);


	//m_sign_touchmove.connect(this, &KView::OnTouchMove);
	//m_sign_touchdown.connect(this, &KView::OnTouchDown);
	//m_sign_touchup.connect(this, &KView::OnTouchUp);

	//m_sign_gesture.connect(this, &KView::OnGesture);
	m_p_gesture_detector = new KGestureDetector;

	m_b_enable_gesture = false;

	m_pSurface = NULL;
	m_b_cache = FALSE;
	m_b_update_cache = TRUE;

	m_b_mouse_picked = false;
	m_b_rmouse_picked= false;
	m_dw_touctpoint_count = 0;

	m_b_mouse_in_view = false; 
	m_i_modal_result = 0;
	m_rotate_angle = 0;
	m_rotate_point.set(0,0);

	m_rect.setXYWH(0,0,100,50);

	m_b_modal_exit = false;
	m_i_modal_result = 0;

	m_transform_flag = 0;

	m_scale_size.fX = 1;
	m_scale_size.fY = 1;

	m_LRMethod_left = KVIEW_LRMethod_Parent_LRNone;
	m_LRMethod_right = KVIEW_LRMethod_Parent_LRNone;
	m_BTMethod_top = KVIEW_BTMethod_Parent_BTNone;
	m_BTMethod_bottom = KVIEW_BTMethod_Parent_BTNone;

	m_b_show_tip = FALSE;
	m_tip_id = NO_TIMER;

	m_name = lexical_cast<kn_string>(g_view_count);
	m_id = g_view_count;

	m_tip_delay_time = NO_TIMER;
	m_tip_hide_time = NO_TIMER; 
}
 
KView::~KView()
{
	g_view_count--;
	Release();
}

void KView::shared_ptr_inited()
{
	m_sign_move.connect(this, &KView::OnMove);
	m_sign_down.connect(this, &KView::OnDown);
	m_sign_up.connect(this, &KView::OnUp);
	m_sign_dclick.connect(this, &KView::OnDClick);
	m_sign_rdown.connect(this, &KView::OnRDown);
	m_sign_rup.connect(this, &KView::OnRUp);
	m_sign_rdclick.connect(this, &KView::OnRDClick);
	m_sign_wheel_down.connect(this, &KView::OnWheel);


	m_sign_touchmove.connect(this, &KView::OnTouchMove);
	m_sign_touchdown.connect(this, &KView::OnTouchDown);
	m_sign_touchup.connect(this, &KView::OnTouchUp);

	m_sign_gesture.connect(this, &KView::OnGesture);
}
 
void KView::setName(const kn_string& name)
{
	m_name = name;
}

kn_string& KView::getName()
{
	return m_name;
}

void KView::setID(kn_int id)
{
	m_id = id;
}

kn_int KView::getID()
{
	return m_id;
}

KVIEWTYPE KView::getType()
{
	return m_e_viewtype;
}

void KView::Release()
{
	
	KScreen* p =  GetScreen(); 
	KView_PTR v_null;
	if (!p)
	{
		p=p;
	}
	//if (m_b_active)
	//{
	//	if (p) 
	//	{
	//			p->setViewActive(v_null, FALSE);
	//	}
	//}
	if (m_b_focus)
	{
		if (p)
		{
			p->setFocusView(v_null, FALSE);
		}
	}


	writeLock lock(m_lst_drawable_mutex);
	m_lst_drawable.clear();	
	m_sign_move.disconnect_all();
	m_sign_down.disconnect_all();
	m_sign_up.disconnect_all();
	m_sign_dclick.disconnect_all();
	m_sign_rdown.disconnect_all();
	m_sign_rup.disconnect_all();
	m_sign_rdclick.disconnect_all();
	m_sign_wheel_down.disconnect_all();


	m_sign_touchmove.disconnect_all();
	m_sign_touchdown.disconnect_all();
	m_sign_touchup.disconnect_all();

	m_sign_gesture.disconnect_all();


	SAFE_DELETE(m_p_gesture_detector);

//	m_p_parent = NULL;
	m_p_screen = NULL;
	SAFE_DELETE(m_pSurface);


}

bool KView::Create(const RERect& rect)
{
	return Create(rect.left(), rect.top(), rect.width(), rect.height());

}

kn_bool KView::Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight)
{
	m_rect.setXYWH(iX, iY, iWidth, iHeight);
	m_transform_flag = 0;
	RERect rect; 
	rect.set(0,0, iWidth,iHeight);
	InvalidateRect(rect);
	//SaveOriginalViewRect(m_rect);
	return true;
}

kn_uint  KView::addDrawable(KDrawable_PTR  p )
{
	writeLock lock(m_lst_drawable_mutex);
	m_lst_drawable.push_back(p);
	return m_lst_drawable.size()-1;
}

void KView::Draw(IRESurface*  pDstSurface, kn_int x, kn_int y)
{
	//RERect  rect = GetBoundRect();
	//rect.offset(x,y);

	RefreshSurface(pDstSurface, x, y);//rect.left(), rect.top());

#ifndef _WIN32_WCE 
	clearInvalidRect();
#endif
}


void KView::enableCache(kn_bool b)
{
	if (m_b_cache == b)
	{
		return;
	}
	if (b)
	{//打开cache 创建缓冲
		if (m_pSurface == NULL)
		{
			RERect rct = GetBoundRect();
			m_pSurface = RESurfaceFactory::CreateRESurface(rct.width(), rct.height(), REBitmap::kARGB_8888_Config);
		}

	}
	else
	{
		SAFE_DELETE(m_pSurface);
	}
	m_b_cache = b;
}

// 局部刷新
void KView::Draw(IRESurface* pDstSurface, LSTRECT& lst_rect, kn_int x, kn_int y)
{
	if (m_b_cache)
	{//有缓冲
		if (m_b_update_cache)
		{
			RefreshSurface(m_pSurface, 0, 0);
			m_b_update_cache = FALSE;
		}
		if (m_transform_flag == 0)
		{
			pDstSurface->DrawBitmap(m_pSurface, x, y);
		}
		else
		{
			REMatrix m ;
			getMatrix( x, y, m);
			pDstSurface->DrawBitmapMatrix(m_pSurface, m);
		}
	}
	else
	{
		RefreshSurface(pDstSurface, x, y);
	}

	clearInvalidRect();
}


void  KView::showMenu(KMenuBase_PTR p_menu, int ix, int iy)
{
	int x,y, mx, my;
	GetScreenXY(x,y);
	mx = x + ix;
	my = y + iy;
	//保证menu在界面内
	if (mx + p_menu->GetRect().width() > GetScreen()->GetWidth() )
	{
		mx -= p_menu->GetRect().width();
	}
	if (my + p_menu->GetRect().height() > GetScreen()->GetHeight() )
	{
		my -= p_menu->GetRect().height();
	}

	p_menu->SetPosition(mx, my);
	GetScreen()->showMenu(p_menu);
	
}

void KView::RefreshSurface(IRESurface* pDstSurface, kn_int x, kn_int y)
{
	// 屏幕坐标转groupview的坐标
	int screenx, screeny;
	GetScreenXY(screenx, screeny);
	KScreen* pScreen = GetScreen(); 
	if (!pScreen)
	{
		return;
	}
	RERect  rectScreen = RERect::MakeXYWH(-screenx, -screeny, pScreen->GetWidth(), pScreen->GetHeight());


	//加写锁
	readLock lock(m_lst_drawable_mutex);

	if(m_b_clip)
	{
		pDstSurface->Save();
		pDstSurface->ClipRect(GetScreenRect(m_rect_clip) );
	}

	REMatrix matrix;
	getMatrix(x, y, matrix);

	VEC_DRAWABLE::iterator itEnd = m_lst_drawable.end();
	for(VEC_DRAWABLE::iterator ite = m_lst_drawable.begin();ite != itEnd; ++ite)
	{
		KDrawable_PTR p = (*ite);

		if( !p->isShow() || !RERect::Intersects(rectScreen, p->GetRect()) )
		{
			// 不绘制屏幕以外的drawable
			continue;
		}

		int i_ds_alpha = p->GetPaint()->getAlpha();
		int i_alpha =  i_ds_alpha * m_i_opacity *(p->getOpacity()) / 65025; //(255*255)
		p->GetPaint()->setAlpha(i_alpha);
		if (p->getType() == KDrawable::KDRAWABLE_COLOR)
		{
			((KColorDrawable*)p.get() )->setColorAlpha( i_alpha );
		}
 
		if (m_transform_flag == 0)
		{
			p->Draw(pDstSurface, NULL, m_rect.left()+ x, m_rect.top() + y);
		}
		else
		{
			p->Draw(pDstSurface, &matrix, m_rect.left()+ x, m_rect.top() + y);
		}


		// 还原
		p->GetPaint()->setAlpha(i_ds_alpha);
	}

	if(m_b_clip)
	{
		pDstSurface->Restore();
	}
}

void KView::RefreshCache()
{
	m_b_update_cache = TRUE;
}


kn_bool KView::IsMousePicked()
{
	return m_dw_touctpoint_count > 0;
}

kn_bool KView::DoFocusMessage(KMessage* pMsg)
{//焦点控件消息处理,多一个坐标转换过程,因为焦点控件直接在screen层响应
	int x = 0;
	int y = 0;

	if ( !m_p_parent.expired())
	{
		m_p_parent.lock()->GetScreenXY(x, y);
	}

	if (pMsg->HasPos())
	{
		((KMessagePos*)pMsg)->m_pos_x -=x;
		((KMessagePos*)pMsg)->m_pos_y -= y;
	}
	kn_bool ret = DoMessage(pMsg);

	if (pMsg->HasPos())
	{	
		((KMessagePos*)pMsg)->m_pos_x += x;
		((KMessagePos*)pMsg)->m_pos_y += y;
	}

	return ret;
}

kn_bool KView::isPointInView(int x, int y)
{// 检查点是否在view内部，除了view范围，还有裁剪区处理，应统一使用这个函数
	if(GetBoundRect().contains( x, y) )
	{
		if( m_b_clip )
		{
			if (m_rect_clip.contains( x,y))
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}
kn_bool KView::DoMessage(KMessage* pMsg)
{
	if (pMsg->m_msg_type == KMSG_3DX) {
		int i = 0;
	}
	if (!m_b_do_msg || !m_b_show)
	{
		return false;
	}

	if(pMsg->HasPos() && !(m_b_mouse_picked || m_b_mouse_in_view ) )
	{//带位置的消息，在view之外原则上不响应，如果之前是 m_b_mouse_picked 或m_b_mouse_in_view 状态，则不管在不在view上都处理
		if( !isPointInView( ((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y))
		{
			return false;
		}
		else
		{

		}
	}

	//设置当前处理消息的view
	pMsg->m_p_view = shared_from_this();

	switch(pMsg->m_msg_type)
	{//这里的处理是防止部分位置消息被吃掉
	case KMSG_LBBUTTONDOWN:
	case KMSG_TOUCH_DOWN:
	case KMSG_LBUTTONDBLCLK:
	case KMSG_RBUTTONDBLCLK:
	case KMSG_RBBUTTONDOWN:
		if ( !isPointInView(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y) )
		{
			return false;
		}

	}

// 	if((pMsg->m_msg_class_type == KMSG_TYPE_TOUCH ) && ((KMessageTouch*)pMsg)->m_touch_id != m_dw_touch_id)
// 	{
// 		return false;
// 	}



	kn_bool ret = FALSE;

	// touch消息会伴随着LBUTTON，有需要时可以屏蔽鼠标消息
	switch(pMsg->m_msg_type)
	{
	case KMSG_MOUSEMOVE:
 
		handleMouseMove(pMsg);
		ret = FALSE;
		break;
	case KMSG_LBBUTTONDOWN:
		handleMouseDown(pMsg);
		ret = m_shield_msg;
		break;
	case KMSG_LBUTTONDBLCLK:
 
		m_sign_dclick.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
		ret = m_shield_msg;
		break;
	case KMSG_LBBUTTONUP:
		//MyTraceA("KMoveableView::DoMessage KMSG_LBBUTTONUP ID = %d", m_dw_touch_id) ;
		handleMouseUp(pMsg);
		 
		break;
	case KMSG_RBBUTTONDOWN:
		m_b_rmouse_picked = TRUE;
		m_sign_rdown.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y,(KMessageMouse*) pMsg);
		ret = m_shield_msg;
		break;
	case KMSG_RBUTTONDBLCLK:
		m_sign_rdclick.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
		ret = m_shield_msg;
		break;
	case KMSG_RBBUTTONUP:
		if (m_b_rmouse_picked)
		{
			
			m_sign_rup.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
			m_b_rmouse_picked = FALSE;
		}
		break;
	case KMSG_MOUSEWHEEL:
		onWheelDirect((KMessageMouseWheel*)pMsg);
		m_sign_wheel_down.emit( (KMessageMouseWheel*)pMsg);
		break;
	case KMSG_KEYDOWN:
		m_sign_key_down.emit((KMessageKey*)pMsg);
		break;
	case KMSG_TYPE_INPUT_ING:
	case KMSG_TYPE_INPUT_END:
	case KMSG_TYPE_CHAR:
	case KMSG_TYPE_IME_UPDATE:
		m_sign_keyboard_input.emit((KMessageInput*)pMsg);
		break;
	case KMSG_KEYUP:
		m_sign_key_up.emit((KMessageKey*)pMsg);
		break;
	case KMSG_3DX:
		m_sign_3dx.emit((KMessage3Dx*)pMsg);
		break;
	case KMSG_TOUCH_DOWN:
		{
			ret = m_shield_msg;
			KMessageTouch* pTouchMsg = (KMessageTouch*)pMsg;
			if(pTouchMsg->m_iPointCount == 1)
			{
				m_dw_touctpoint_count = 1;
				
				// 按 KMSG_LBBUTTONDOWN 处理
				handleMouseDown(pMsg);
			}
			else if(pTouchMsg->m_iPointCount == 2 && m_dw_touctpoint_count == 1)
			{
				// 1 TOUCH_DOWM 1point
				// 2. TOUCH_DOWM 2point   进入多点时放弃原有单点的操作
 
				handleMouseUp(pMsg);
				ret = FALSE;
			}

			if(m_dw_touctpoint_count < pTouchMsg->m_iPointCount)
			{
				m_dw_touctpoint_count = pTouchMsg->m_iPointCount;
			}
			
			
			//ret = TRUE;
			onTouchDownDirect(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg); // 指针强制转换
			m_sign_touchdown.emit(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg);
			
		}
		
		break;
	case KMSG_TOUCH_MOVE:
		{

			KMessageTouch* pTouchMsg = (KMessageTouch*)pMsg;
			
			if(pTouchMsg->m_iPointCount == 1 && m_dw_touctpoint_count == 1)
			{
				handleMouseMove(pMsg);				 
			}
			else if(pTouchMsg->m_iPointCount == 2 && m_dw_touctpoint_count == 1)
			{
				// 1 TOUCH_DOWM 1point
				// 2. No TOUCH_DOWM 2point [没有经过TOUCH_DOWM 2point 的情况] 
				// 3 TOUCH_MOVE 2points,   进入多点时放弃原有单点的操作
				handleMouseUp(pMsg);	
				 
			}

			if(m_dw_touctpoint_count < pTouchMsg->m_iPointCount)
			{
				m_dw_touctpoint_count = pTouchMsg->m_iPointCount;
			}
	 
			onTouchMoveDirect(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg);
			m_sign_touchmove.emit(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg);
			ret = FALSE;
		}
	
		break;
	case KMSG_TOUCH_UP:
		{
			KMessageTouch* pTouchMsg = (KMessageTouch*)pMsg;
			if(pTouchMsg->m_iPointCount == 1 && m_dw_touctpoint_count == 1)	//	确保一直只有1个手指
			{
				// 按 KMSG_LBBUTTONUP 处理			 
				handleMouseUp(pMsg);
				 
			}
	 
			onTouchUpDirect(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg);
			m_sign_touchup.emit(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg);
			//ret = m_shield_msg;
			m_dw_touctpoint_count = 0;
		}
		
		 
		break;
	case KMSG_DRAG:
	case KMSG_DRAG_UP:
		 onDragDirect((KMessageDrag*)pMsg);
		 break;
	default:
		ret = false;
		if (pMsg->m_msg_type > KNUIMSG_USER)
		{//应用层自定义消息
			ret = OnUserMsg(pMsg);
		}
		break;
	}
	return ret;
}


void KView::handleMouseDown(KMessage* pMsg)
{
	m_b_mouse_picked = true;
	m_dw_touctpoint_count = 1;
	//	g_b_has_picked = true;
	onDownDirect(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y,(KMessageMouse*)pMsg);
	m_sign_down.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y,(KMessageMouse*)pMsg);
}

void KView::handleMouseMove(KMessage* pMsg)
{
	onMoveDirect(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
	m_sign_move.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
}

void KView::handleMouseUp(KMessage* pMsg)
{
	if (m_b_mouse_picked)
	{
		onUpDirect(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y,(KMessageMouse*)pMsg);
		m_sign_up.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);

		kn_bool b_in = isPointInView( ((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y ) ;
		if (b_in)
		{
			m_clicked_pos_signal.emit(((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y, (KMessageMouse*)pMsg);
			m_clicked_signal.emit(shared_from_this());
		}
		m_b_mouse_picked = false;
		m_dw_touctpoint_count = 0; 
	}
}


void KView::onDragDirect(KMessageDrag* mag)
{

}

kn_bool KView::OnUserMsg(KMessage* pMsg)
{
	return FALSE;
}

void KView::setViewFocus()
{
	KScreen* p= GetScreen();
	if (p)
	{
		p->setFocusView(shared_from_this());
		m_b_focus = TRUE;
	}

	m_sign_focus.emit();
}

void KView::unSetViewFocus()
{
	KScreen* p= GetScreen();
	if (p && p->getFocusView() == shared_from_this())
	{
		KView_PTR v_null;
		p->setFocusView(v_null, FALSE);
	}

	m_b_focus = FALSE;
	m_sign_unfocus.emit();
}

kn_bool KView::bViewFocus()
{
	return m_b_focus;
}

void KView::setViewActive()
{
	//KScreen* p= GetScreen();
	//if (p)
	//{
	//	p->setViewActive(shared_from_this());
	//	m_b_active = TRUE;
	//}
	m_sign_active.emit();
}

void KView::unSetViewActive(kn_bool unsetScreen)
{
	//if (unsetScreen)
	//{
	//	KScreen* p= GetScreen();
	//	if (p)
	//	{
	//		KView_PTR v_null;
	//		p->setViewActive(v_null, FALSE);
	//	}	
	//	
	//}

	m_sign_unactive.emit();
//	m_b_active = FALSE;
}

// DoMessage时直接调用,不经信号槽传递,供控件内部使用
void KView::onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void KView::onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if (m_b_mouse_picked || m_b_rmouse_picked)
	{//点击按下状态，可不处理聚焦消息
		if (m_b_mouse_in_view && !isPointInView(iScreenX, iScreenY))   //鼠标离开按钮 
		{
			m_b_mouse_in_view = false;   //清除进入标志
			//			m_mouse_leave_signal.emit();
		}
		return;
	}

	if(!m_b_mouse_in_view && isPointInView(iScreenX, iScreenY))    //鼠标进入按钮
	{
		m_b_mouse_in_view = true;    //设置进入标志
		setViewActive();
		m_mouse_enter_signal.emit();
	//	MyTrace(_T("%s enter \n"), m_name.c_str() );
		if (m_shield_msg)
		{
			pMsg->setIdle(KMSG_RETURN_DILE);
		}

		//tip处理
		if (m_b_show_tip)
		{
			if (!m_b_mouse_picked && !m_b_rmouse_picked)
			{
				showTip(m_tip_delay_time, m_tip_hide_time);
			}
		}

	}
	else if (m_b_mouse_in_view )   
	{
		if(isPointInView(iScreenX, iScreenY) )
		{//吃掉消息
			if (m_shield_msg)
			{
				pMsg->setIdle(KMSG_RETURN_DILE);
			}
		}
		else
		{//鼠标离开按钮 
			m_b_mouse_in_view = false;   //清除进入标志
			unSetViewActive();
			m_mouse_leave_signal.emit();
	//		MyTrace(_T("%s leave \n"), m_name.c_str());
			if (m_b_show_tip)
			{
				hideTip();
			}
		}

	}
}

void KView::onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void KView::onWheelDirect(KMessageMouseWheel* pMsg)
{

}

void KView::enableGesture(kn_bool b)
{
	m_b_enable_gesture = b;
}

void KView::recogGesture(KMessageTouch* pMsg)
{

	if(!m_b_enable_gesture)
	{
		return;
	}

	char szName [][32] = {"KMSG_TOUCH_UP",
		"KMSG_TOUCH_MOVE",
		"KMSG_TOUCH_DOWN"};

//	MyTraceA("onTouchUpDirect ..%d, type(%d), %s", pMsg->m_iPointCount, pMsg->m_msg_type, szName[pMsg->m_msg_type - KMSG_TOUCH_UP]);

	if(pMsg->m_iPointCount == 1)
	{
	//	MyTraceA("onTouchUpDirect ..%d, type(%d), %s, touchid(%d), (%d,%d)", pMsg->m_iPointCount, pMsg->m_msg_type, szName[pMsg->m_msg_type - KMSG_TOUCH_UP],
	//		pMsg->m_ids[0], pMsg->m_xs[0], pMsg->m_ys[0]);

	}
	else 
	{
//		MyTraceA("onTouchUpDirect ..%d, type(%d), %s, [touchid(%d), (%d,%d)], [touchid(%d), (%d,%d)]", pMsg->m_iPointCount, pMsg->m_msg_type, szName[pMsg->m_msg_type - KMSG_TOUCH_UP],
//			pMsg->m_ids[0], pMsg->m_xs[0], pMsg->m_ys[0],
//			pMsg->m_ids[1], pMsg->m_xs[1], pMsg->m_ys[1]		);
	}


	m_p_gesture_detector->addTouchMessge(pMsg);
	KGesture gesture = m_p_gesture_detector->recogGesture();
	
	if(gesture.m_type != GESTURE_UNKNOWN)
	{
		m_sign_gesture.emit(&gesture);
	}
}

void KView::onTouchDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg)
{
	recogGesture(pMsg);
}

void KView::onTouchMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg)
{

	recogGesture(pMsg);
}

void KView::onTouchUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg)
{
	
	recogGesture(pMsg);
}


void KView::enableMessage(kn_bool b)
{
	m_b_do_msg = b;
}


//void KView::SetOpacity(kn_byte value)
//{
//	if(m_i_opacity != value)
//	{
//		InvalidateForParent(GetBoundRect());
//		m_i_opacity = value;
//	}
//}
//
//kn_int KView::GetOpacity()
//{
//	return m_i_opacity;
//}

void KView::GetPosition(kn_int& iX, kn_int& iY)
{
	RERect rct = GetBoundRect();
	iX = rct.left();
	iY = rct.top();
}

REPoint KView::GetPosition()
{
	RERect rct = GetBoundRect();
	return REPoint::Make(rct.left(), rct.top());
}

void KView::GetScreenXY(kn_int &x, kn_int &y)
{
	kn_int xw, yw;

// 	x = m_rect.left();
// 	y = m_rect.top();


	RERect rectBound = GetBoundRect();

	x = rectBound.left();
	y = rectBound.top();

	if (!m_p_parent.expired()) 
	{
		m_p_parent.lock()->GetScreenXY(xw,yw);
		x += xw;
		y += yw;
	}
}

void KView::SetScreenPosition(kn_int iX, kn_int iY)
{
//	ASSERT(m_p_parent);
	int iScreenLeft,iScreenTop;

    RERect rect = GetBoundRect();
    InvalidateForParent(rect);

	GetScreenXY(iScreenLeft, iScreenTop);

	int iOffsetX = iX - iScreenLeft;
	int iOffsetY = iY - iScreenTop;

	m_rect.offset(iOffsetX, iOffsetY);
    rect = GetBoundRect();
    InvalidateForParent(rect);
}

void KView::SetPosition(kn_int iX, kn_int iY)
{
    RERect rect = GetBoundRect();
    InvalidateForParent(rect);
//	int iOffsetX = -iX + m_rect.left();
//	int iOffsetY = -iY + m_rect.top();
//	m_rect.offset(iOffsetX, iOffsetY);

	writeLock lock(m_lst_drawable_mutex);

	if (m_transform_flag & VIEW_TRANSFORM_BOUND)
	{//变形矩阵也要移动，要不变形情况下移不动
		int x = m_rect_bound.left() - m_rect.left() +iX;
		int y = m_rect_bound.top() - m_rect.top() +iY;
		m_rect_bound.offsetTo(x,y);
	}

	m_rect.offsetTo(iX, iY);
    rect = GetBoundRect();
    InvalidateForParent(rect);
}

void KView::SetPositionX(kn_int iX)
{
	SetPosition(iX, m_rect.top() );
}

void KView::SetPositionY(kn_int iY)
{
	SetPosition(m_rect.left(), iY);
}


kn_bool KView::HasInvalidRect()
{
	if (m_lst_rect_invalid.empty() )
	{
		return FALSE;
	}
	return TRUE;
}

LSTRECT KView::GetInvalidRect()
{
	return m_lst_rect_invalid;
}

void KView::SetParent(KViewGroup_WEAK_PTR p)
{
	m_p_parent = p; 
}

//注意这里是返回强引用
KViewGroup_PTR KView::GetParent()
{
	return m_p_parent.lock();
}

void KView::SetScreen(KScreen * p)
{
	m_p_screen = p; 
}

KScreen* KView::GetScreen()
{
	if (m_p_screen)
	{
		return m_p_screen;
	}

	if (!m_p_parent.expired())
	{
		return m_p_parent.lock()->GetScreen();
	}

	//不应该运行到这里
//	ASSERT(FALSE);
	return NULL;
}


void KView::SetRect(const RERect& rect)
{
	KShowObject::SetRect(rect);
	m_transform_flag = 0;
	InvalidateForParent(m_rect);
}

void KView::UpdateUI ()
{
	KScreen* p = GetScreen();
	if (p)
	{
		p->SetRenderFlag(TRUE);
	}
	
}

// 设置局部刷新区域
void KView::InvalidateRect(RERect rect)
{
	//暂时关闭局部刷新 zhic  现阶段意义不大
	return;

	kn_int xw, yw;
	GetScreenXY(xw, yw);
	rect.offset(xw - rect.left(),yw - rect.top());

	writeLock lock(m_rectInvalid_mutex);
	m_lst_rect_invalid.push_back(rect);
}

//刷新控件自身,可控制是否立即要求重绘，
void KView::InvalidateView(kn_bool bRefresh)
{
	RERect r = RERect::MakeWH(m_rect.width(), m_rect.height() );
	InvalidateRect(r);

	if (bRefresh)
	{//申请立即重绘
		UpdateUI();
	}
}

void KView::InvalidateForParent(RERect& r)
{
	RERect rect = r;
	rect.offset(m_rect.left(),m_rect.top());
	InvalidateRect(rect);
}


void KView::clearInvalidRect()
{
#ifndef _WIN32_WCE
	// wince 产生断言，原因待查
	writeLock  lock(m_rectInvalid_mutex);
#endif
	m_lst_rect_invalid.clear();
}

//先不实现相对变换
//void KView::Rotate( REScalar degree, REScalar px, REScalar py )
//{
//	m_b_transformed = true;
//	m_matrix.postRotate(degree, px, py);
//}
//
//void KView::Skew( REScalar x, REScalar y, REScalar px, REScalar py )
//{
//	m_b_transformed = true;
//	m_matrix.postSkew(x, y, px, py);
//}
//
//void KView::Translate( REScalar dx, REScalar dy )
//{
//	m_b_transformed = true;
//	m_matrix.postTranslate(dx, dy);
//}
//
//void KView::Scale( REScalar sx, REScalar sy, REScalar px /*= 0*/, REScalar py /*= 0*/ )
//{
//	m_b_transformed = true;
//	m_matrix.postScale(sx, sy, px, py);
//}

RERect KView::GetBoundRect()
{
	if (m_transform_flag != 0)
	{
		RERect ret = m_rect;
		REMatrix m;
		getMatrix(0,0,m);
		m.mapRect(&ret);
		return ret;
	}
	else
	{
		return m_rect;
	}
}

void  KView::getMatrix( REScalar x, REScalar y, REMatrix& mat)
{
	mat.setIdentity();
	if (m_transform_flag == 0)
	{
		return;
	}
	
	if (m_transform_flag & VIEW_TRANSFORM_BOUND)
	{
		RERect rect1 = m_rect;
		RERect rect2 = m_rect_bound;
		rect1.offset( x, y );
		rect2.offset( x, y);
		mat.setRectToRect(rect1, rect2, REMatrix::kFill_ScaleToFit);
	}

	if (m_transform_flag & VIEW_TRANSFORM_TRANSLATE)
	{
		mat.setTranslate(m_translate_point.x(), m_translate_point.y() );
	}

	if (m_transform_flag & VIEW_TRANSFORM_SCALE)
	{
		mat.setScale(m_scale_size.x(), m_scale_size.y(), m_scale_xy.x() + m_rect.left()+ x, m_scale_xy.y() + m_rect.top()+ y);
	}

	if (m_transform_flag & VIEW_TRANSFORM_ROTATE)
	{
		mat.setRotate(m_rotate_angle, m_rotate_point.x() + m_rect.left()+ x, m_rotate_point.y() + m_rect.top()+ y);
	}

	if (m_transform_flag & VIEW_TRANSFORM_SKEW)
	{
		mat.setSkew(m_skew_para.fLeft, m_skew_para.fRight, m_skew_para.fTop+ x, m_skew_para.fBottom +y);
	}
}

void KView::SetBoundRect(const RERect& rect)
{
	writeLock lock(m_lst_drawable_mutex);

	if(rect == m_rect)
	{//清除标志位
		m_transform_flag &=(VIEW_TRANSFORM_ALL - VIEW_TRANSFORM_BOUND);
	}
	else
	{
		m_rect_bound = rect;
		m_transform_flag |=VIEW_TRANSFORM_BOUND;
	}
}

void KView::SetTranslate( REScalar dx, REScalar dy )
{
	if (dx == 0 && dy == 0)
	{
		m_transform_flag &=(VIEW_TRANSFORM_ALL - VIEW_TRANSFORM_TRANSLATE);
	}
	else
	{
		m_transform_flag |=VIEW_TRANSFORM_TRANSLATE;
		m_translate_point.set(dx, dy);
	}
}

void KView::SetScale( REScalar sx, REScalar sy, REScalar px /*= 0*/, REScalar py /*= 0*/ )
{
	if (sx == 1 && sy == 1)
	{
		m_transform_flag &=(VIEW_TRANSFORM_ALL - VIEW_TRANSFORM_SCALE);
		m_scale_size.fX = sx;
		m_scale_size.fY = sy;
		m_scale_xy.fX = px;
		m_scale_xy.fY = py;
	}
	else
	{
		m_transform_flag |=VIEW_TRANSFORM_SCALE;
		m_scale_size.fX = sx;
		m_scale_size.fY = sy;
		m_scale_xy.fX = px;
		m_scale_xy.fY = py;
	}

}

void KView::SetScaleX( REScalar sx )
{
	SetScale(sx,m_scale_size.fY , m_scale_xy.fX, m_scale_xy.fY );
}

void KView::SetScaleY( REScalar sy )
{
	SetScale(m_scale_size.fX, sy , m_scale_xy.fX, m_scale_xy.fY );
}

void KView::SetScalePoint( REScalar px , REScalar py)
{
	m_scale_xy.fX = px;
	m_scale_xy.fY = py;
}

void KView::SetRotate( kn_float degrees, REScalar px /*= 0*/, REScalar py /*= 0*/ )
{
	if (degrees == 0)
	{
		m_transform_flag &=(VIEW_TRANSFORM_ALL - VIEW_TRANSFORM_ROTATE);
	}
	else
	{
		m_rotate_angle = degrees;
		m_rotate_point.set(px,py);
		m_transform_flag |=VIEW_TRANSFORM_ROTATE;
	}
}

void KView::SetRotatePoint( REScalar px , REScalar py)
{
	SetRotate(m_rotate_angle,  px, py);
}

void KView::SetRotateAngle( kn_float degrees)
{
	SetRotate(degrees, m_rotate_point.x(), m_rotate_point.y());

}


void KView::SetSkew( kn_float sx, kn_float sy, REScalar px /*= 0*/, REScalar py /*= 0*/ )
{
	if (sx == 1.0 && sy == 1.0)
	{
		m_transform_flag &= (VIEW_TRANSFORM_ALL - VIEW_TRANSFORM_SKEW);
	}
	else
	{
		m_transform_flag &= VIEW_TRANSFORM_SKEW;
		m_skew_para.fLeft = sx;
		m_skew_para.fRight = sy;
		m_skew_para.fTop = px;
		m_skew_para.fBottom = px;
	}

	
}


REScalar KView::getRotateAngle()
{
	return m_rotate_angle;
}

void KView::getRotatePoint(REScalar& x, REScalar& y)
{
	x = m_rotate_point.x();
	y = m_rotate_point.y();

}


void KView::OnMove(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	// 设置区域

}
void KView::OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void KView::OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}
void KView::OnDClick( kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg )
{

}

void KView::OnRDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}

void KView::OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{

}
void KView::OnRDClick( kn_int iScreenX, kn_int iScreenY , KMessageMouse* pMsg)
{

}

void KView::OnWheel( KMessageMouseWheel* pMsg )
{

}

void KView::OnTouchMove(kn_int x, kn_int y, KMessageTouch* pMsg)
{



}
void KView::OnTouchDown(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg)
{

}

void KView::OnTouchUp(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg)
{

}

void KView::OnGesture(KGesture* pGesture)
{

}

 

kn_int KView::doModal()
{
	// note: 未考虑多层doModal的情况
	setViewFocus();
	m_b_modal_exit = false;
	GetScreen()->addModalView( shared_from_this() );
	//	writeLock lock(m_lst_drawable_mutex);

	//doModal的时候添加变暗的全屏半透明背景
	kn_int i_x = 0,i_y = 0;
	i_x -= m_rect.left();
	i_y -= m_rect.top();

	//获取当前View相对于顶级的坐标
	if (!m_p_parent.expired())
	{
		i_x -= m_p_parent.lock()->GetRect().left();
		i_y -= m_p_parent.lock()->GetRect().top();
	}
	//添加背景
	KDrawable_PTR p_drawable (new KColorDrawable(0x70000000));
	p_drawable->setRect(i_x,i_y,GetScreen()->GetWidth(),GetScreen()->GetHeight());
	m_lst_drawable.insert(m_lst_drawable.begin(),p_drawable);
	InvalidateView();
#ifndef ANDROID_NDK
	int ret = runModalLoop();

	if (!m_p_parent.expired())
	{
		m_p_parent.lock()->AddViewToDel( shared_from_this() );
	}

	if (ret != -1) //返回-1是收到系统退出消息，可能view已被释放
	{
		GetScreen()->delModalView(shared_from_this());
//		GetScreen()->SetRenderFlag(FALSE);
		InvalidateView();
		return m_i_modal_result;
	}

	return 0;
#else
	return 1;
#endif
}

void KView::endModal(kn_int iResult,  kn_bool b_del_view)
{
	m_i_modal_result = iResult;
	m_b_modal_exit = true;
	if (b_del_view)
	{
		if (!m_p_parent.expired())
		{
			m_p_parent.lock()->AddViewToDel(shared_from_this());
		}
	}
#ifdef ANDROID_NDK
	GetScreen()->delModalView(shared_from_this());
#endif
}


RERect  KView::GetScreenRect(RERect rect)
{
	//将矩形
	int screenx, screeny;
	GetScreenXY(screenx, screeny);

	rect.offset(screenx - m_rect.left(), screeny - m_rect.top());
	return rect;
}



//把view内部的内容渲染到一个image上,构造成TextView返回
IRESurface* KView::renderToSurface()
{
	IRESurface*  p_surface = RESurfaceFactory::CreateRESurface(m_rect.width(), m_rect.height(), REBitmap::kARGB_8888_Config );
	return NULL;
}

KTextView* KView::renderToView()
{
	return NULL;
}
///////////简化的属性参数设置
kn_bool  KView::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PropertyPARA_ID_POS_X)
	{
		kn_int i_v = v;
		return  (m_rect.left() != i_v) ;
	}
	if (para_id == PropertyPARA_ID_POS_Y)
	{
		kn_int i_v = v;
		return  (GetBoundRect().top() != i_v) ;
	}
	if (para_id == PropertyPARA_ID_Opacity)
	{
		kn_int i_v = v;
		return  (getOpacity() != (int)i_v) ;
	}

	if (para_id == PropertyPARA_ID_RotateAngle)
	{
		kn_int i_v = v;
		return  (m_rotate_angle != i_v) ;
	}

	if (para_id == PropertyPARA_ID_ScaleX)
	{
		return  (m_scale_size.fX != v) ;
	}
	if (para_id == PropertyPARA_ID_ScaleY)
	{
		return  (m_scale_size.fY != v) ;
	}
	return FALSE;
}

void  KView::doChange(kn_int para_id, kn_double v)
{
	if (para_id == PropertyPARA_ID_POS_X)
	{
		SetPositionX( v );
	}
	if (para_id == PropertyPARA_ID_POS_Y)
	{
		SetPositionY( v );
	}
	if (para_id == PropertyPARA_ID_Opacity)
	{
		setOpacity(v);
	}
	if (para_id == PropertyPARA_ID_RotateAngle)
	{
		SetRotateAngle(v);
	}
	if (para_id == PropertyPARA_ID_ScaleX)
	{
		SetScaleX(v);
	}
	if (para_id == PropertyPARA_ID_ScaleY)
	{
		SetScaleY(v);
	}
}

kn_double  KView::getParaValue(kn_int para_id)
{
	if (para_id == PropertyPARA_ID_POS_X)
	{
		return m_rect.left();
	}
	if (para_id == PropertyPARA_ID_POS_Y)
	{
		return  m_rect.y();
	}
	if (para_id == PropertyPARA_ID_Opacity)
	{
		return  getOpacity();
	}

	if (para_id == PropertyPARA_ID_RotateAngle)
	{
		return  m_rotate_angle;
	}

	if (para_id == PropertyPARA_ID_ScaleX)
	{
		return  m_scale_size.fX;
	}
	if (para_id == PropertyPARA_ID_ScaleY)
	{
		return  m_scale_size.fY;
	}
	return 0;
}

////增加引用计数
//void  KView::addRef()
//{
//	m_ref++;
//}
//
////降低引用计数
//void  KView::decRef()
//{
//	m_ref--;
//}
//
////是否存在引用计数
//kn_bool  KView::isRef()
//{
//	return m_ref > 0;
//	 
//}

void KView::viewSizeChange()
{
	if (m_p_parent.expired())
	{
		return;
	}

    RERect rectParent = m_p_parent.lock()->GetRect();
    RERect rectOrigin = m_p_parent.lock()->getRectOrigin();

    sizeChange(rectParent, rectOrigin);

	for(VEC_DRAWABLE::iterator ite = m_lst_drawable.begin();ite != m_lst_drawable.end(); ++ite)
	{
		if ( (*ite)->getAutoSize() )
		{
			(*ite)->sizeChange(m_rect, m_rect_origin);
		}
		
	}
}

void  KView::SaveOriginalViewRect()
{
	KShowObject::SaveOriginalViewRect();

	for(VEC_DRAWABLE::iterator ite = m_lst_drawable.begin();ite != m_lst_drawable.end(); ++ite)
	{
		(*ite)->SaveOriginalViewRect();
	}
}

void	KView::SetShieldMsg(kn_bool b)
{
	m_shield_msg = b;
}

void KView::setTip(const kn_string& str, kn_int delay, kn_int hide)
{
	m_tip_txt = str;
	m_tip_delay_time = delay;
	m_tip_hide_time = hide;
	enableTip(TRUE);
}

void KView::enableTip(kn_bool b)
{
	m_b_show_tip = b;
}

void KView::showTip(int delay_time, int hide_time)
{

	if (delay_time > 0)
	{
		m_delay_timer = KTimer_PTR(new KTimer(delay_time, 1));
		m_delay_timer->setView(shared_from_this());
		m_delay_timer->m_timeout_signal.connect(this, &KView::showTipCallback);
		m_delay_timer->start();
	}
	else
	{//直接显示
		showTipCallback(1);
	}

	if (hide_time>0)
	{
		m_hide_timer = KTimer_PTR(new KTimer(hide_time+delay_time, 1));
		m_hide_timer->setView(shared_from_this());
		m_hide_timer->m_timeout_signal.connect(this, &KView::hideTipCallback);
		m_hide_timer->start();
	}

}

void KView::showTipCallback(int n)
{
	KScreen* screen = GetScreen();
	if (!screen)
	{
		return;
	}

	KTipBase_PTR tip = KTipBase_PTR(new KTipBase);
	tip->init(screen, GetScreenRect(m_rect), m_tip_txt);
	screen->showTip( tip );
	m_tip_id = tip->getTipID();
	InvalidateView();
}

void KView::hideTipCallback(int n)
{
	hideTip();
}

void KView::hideTip()
{
	KScreen* screen = GetScreen();
	if (!screen)
	{
		return;
	}
	if (m_delay_timer)
	{
		m_delay_timer->stop();
		m_delay_timer.reset();
	}

	if (m_hide_timer)
	{
		m_hide_timer->stop();
		m_hide_timer.reset();
	}
	screen->hideTip(m_tip_id );

	InvalidateView();
}

void  KView::createTextDrawableHelper(KTextDrawable_PTR* txt,kn_string str, int fontsize, kn_int x,kn_int y, kn_int w, kn_int h, REColor clr, REPaint::Align align )
{
	*txt = KTextDrawable_PTR(new KTextDrawable(str.c_str(), clr, fontsize, align));
	(*txt)->setFont( GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei") );
	(*txt)->setRect(x,y, w, h);
	addDrawable( *txt );
}

void KView::createColorDrawableHelper(KColorDrawable_PTR* color_da, kn_int x,kn_int y, kn_int w, kn_int h, REColor r )
{
	*color_da = KColorDrawable_PTR(new KColorDrawable(r) );
	(*color_da)->SetRect(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()) );
	addDrawable(*color_da);
}

RERect KView::getDrawRectBaseOnChild()
{
	if (!isShow())
	{
		return RERect::MakeXYWH(0, 0, 0, 0);
	}

	// 访问m_lst_drawable需要加锁 add by junz 2015-12-26
	readLock lock(m_lst_drawable_mutex);
	RERect view_rect = RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height());
	for (kn_int i = 0; i < m_lst_drawable.size(); ++i)
	{
		view_rect.join(m_lst_drawable[i]->GetRect());
	}
	RERect draw_rect = RERect::MakeXYWH(view_rect.left() + m_rect.left(), view_rect.top() + m_rect.top(), view_rect.width(), view_rect.height());
	return draw_rect;
}

void KView::requestSetPosition(kn_int iX, kn_int iY)
{
	CPropertyPos* pPos = new CPropertyPos(shared_from_this(), iX, iY);
	GetScreen()->addProperty(pPos, FALSE);
}