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
	{//æ‰“å¼€cache åˆ›å»ºç¼“å†²
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

// å±€éƒ¨åˆ·æ–°
void KView::Draw(IRESurface* pDstSurface, LSTRECT& lst_rect, kn_int x, kn_int y)
{
	if (m_b_cache)
	{//æœ‰ç¼“å†²
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
	//ä¿è¯menuåœ¨ç•Œé¢å†…
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
	// å±å¹•åæ ‡è½¬groupviewçš„åæ ‡
	int screenx, screeny;
	GetScreenXY(screenx, screeny);
	KScreen* pScreen = GetScreen(); 
	if (!pScreen)
	{
		return;
	}
	RERect  rectScreen = RERect::MakeXYWH(-screenx, -screeny, pScreen->GetWidth(), pScreen->GetHeight());


	//åŠ å†™é”
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
			// ä¸ç»˜åˆ¶å±å¹•ä»¥å¤–çš„drawable
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


		// è¿˜åŸ
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
{//ç„¦ç‚¹æ§ä»¶æ¶ˆæ¯å¤„ç†,å¤šä¸€ä¸ªåæ ‡è½¬æ¢è¿‡ç¨‹,å› ä¸ºç„¦ç‚¹æ§ä»¶ç›´æ¥åœ¨screenå±‚å“åº”
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
{// æ£€æŸ¥ç‚¹æ˜¯å¦åœ¨viewå†…éƒ¨ï¼Œé™¤äº†viewèŒƒå›´ï¼Œè¿˜æœ‰è£å‰ªåŒºå¤„ç†ï¼Œåº”ç»Ÿä¸€ä½¿ç”¨è¿™ä¸ªå‡½æ•°
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
	{//å¸¦ä½ç½®çš„æ¶ˆæ¯ï¼Œåœ¨viewä¹‹å¤–åŸåˆ™ä¸Šä¸å“åº”ï¼Œå¦‚æœä¹‹å‰æ˜¯ m_b_mouse_picked æˆ–m_b_mouse_in_view çŠ¶æ€ï¼Œåˆ™ä¸ç®¡åœ¨ä¸åœ¨viewä¸Šéƒ½å¤„ç†
		if( !isPointInView( ((KMessagePos*)pMsg)->m_pos_x, ((KMessagePos*)pMsg)->m_pos_y))
		{
			return false;
		}
		else
		{

		}
	}

	//è®¾ç½®å½“å‰å¤„ç†æ¶ˆæ¯çš„view
	pMsg->m_p_view = shared_from_this();

	switch(pMsg->m_msg_type)
	{//è¿™é‡Œçš„å¤„ç†æ˜¯é˜²æ­¢éƒ¨åˆ†ä½ç½®æ¶ˆæ¯è¢«åƒæ‰
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

	// touchæ¶ˆæ¯ä¼šä¼´éšç€LBUTTONï¼Œæœ‰éœ€è¦æ—¶å¯ä»¥å±è”½é¼ æ ‡æ¶ˆæ¯
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
				
				// æŒ‰ KMSG_LBBUTTONDOWN å¤„ç†
				handleMouseDown(pMsg);
			}
			else if(pTouchMsg->m_iPointCount == 2 && m_dw_touctpoint_count == 1)
			{
				// 1 TOUCH_DOWM 1point
				// 2. TOUCH_DOWM 2point   è¿›å…¥å¤šç‚¹æ—¶æ”¾å¼ƒåŸæœ‰å•ç‚¹çš„æ“ä½œ
 
				handleMouseUp(pMsg);
				ret = FALSE;
			}

			if(m_dw_touctpoint_count < pTouchMsg->m_iPointCount)
			{
				m_dw_touctpoint_count = pTouchMsg->m_iPointCount;
			}
			
			
			//ret = TRUE;
			onTouchDownDirect(pTouchMsg->m_pos_x, pTouchMsg->m_pos_y, pTouchMsg); // æŒ‡é’ˆå¼ºåˆ¶è½¬æ¢
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
				// 2. No TOUCH_DOWM 2point [æ²¡æœ‰ç»è¿‡TOUCH_DOWM 2point çš„æƒ…å†µ] 
				// 3 TOUCH_MOVE 2points,   è¿›å…¥å¤šç‚¹æ—¶æ”¾å¼ƒåŸæœ‰å•ç‚¹çš„æ“ä½œ
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
			if(pTouchMsg->m_iPointCount == 1 && m_dw_touctpoint_count == 1)	//	ç¡®ä¿ä¸€ç›´åªæœ‰1ä¸ªæ‰‹æŒ‡
			{
				// æŒ‰ KMSG_LBBUTTONUP å¤„ç†			 
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
		{//åº”ç”¨å±‚è‡ªå®šä¹‰æ¶ˆæ¯
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
	//		p->setViewActiÎöq6ŞƒvXwÆû®ŒZ£‹LÈ5‰[¹¹—ÿkB­°ë†ôj½Z¾ > C „¨³ÉX9ZİS¦ÒBúÏd&¬6ß@"­ıAı¬„·e’E§ôÂ›pªZ¡ª‰Â‚¦‰1¹[ivÏ²Çšº!y†]³}°Š¤9›•Î²¿1¬YsÔï.ÜAÍßÄÆ9ÅÔèl²Ãâ„¾Šyj¡¸)Ú&vì¥*÷b Sã€ù§f¾p¢³¾æ·‰ºà4»'â“¶šœ£-˜ß^1ë§}H¹ÆtkVßêÑ!¹¾…×WBÚäş5YÎŸ=ø—œÙeQó±>§W¨£ia'wz»p¨ØŒEAòI`äQ³ÈÍÚ
©/a©o@•b¹¯uêUJ7qû¤§0ª±™¸\_î\°¸`	¯çØÅrv2Z·şŠCyw`[Ç>}÷°õÒ–$@+ÉöBıDÀ¬ÉsµÜf	;¤ô…F)M‰Êç“
y Œx7WèE¾ğĞÇ.8Ô­h¬RôÓh=šúğÂ<Ğ5âê/qPŒ[S·­İ|fù¹p™¿4xB-÷Tx>à†éŠ¾ô‡qFø¬Ø 2ÂàgÇİ¦¢0²–Ößi)ËŸİÂsòky5ĞßÛ4¨	aäØÓ\°2ùC@‘d¤Iä#ºrÛ¹ˆ‘¨÷õ0c'$¢å4eÊ)]ÂKgÆ±Eˆ\aD­†×s²Ü¨`/"Ğ’A£w,Hwù^M„ÚYìE¿¨/…˜X7µ+ñ`lîÎÉè.î€Ş±Å"€.şt°ø:†ù¥Ş*CN¬BA*ú¤©:´»¿ÊH¡LñÒ¸›NÅÏAª(¥¬İ‡”.1€¸‚ta`5+ë£±ÔhxşN>‘ãßáù»?¾­$Ü„­ßöxvJ±FÎ*`·š‡§`˜Ì"l­iKã–SW¢dŠêÿÖ¥¿b’³Ë3ßt¬%ìÅÂ/L÷;èÁŠÁN	LUr›ª¶¼ŠJç^|\´me¤hG´oE¯r‹swğ*$›;H³k;#s„*É3/ñµ²K,…Ö†µ–^|ï÷&æ_ß[V™ØãÂ©´áOERçUçToò¬Ã…™]¢¾zÜÓx|<âÆ˜|ƒz]D}©<3Ğrtµœìº/÷*ÒwsS¿Ó*Ä¢<¿Ì¼]ó;ÙZCCålâJ™´g‰púË@´¢
Zà¸÷^¼ÄAuÙÅ÷rK±¾õ-§±Ö&9Ş/ ä qƒñïT3(N‹©	ZÄs©_¦òw,V¾£MSxFÎÕfü.¶€Hƒ?C}j˜¯Z¶­Á\îüJb´Ãä 	v”û aâ——P´e¯öÈº+\¯úü‚”å÷ÿ½4s)FÖ‚R(ïš IŠSÌÂ­âÀZÊ¬Èå/ùlJ¬P¿fÛÄ]M¦_ò‚xÑtfmÓ]í Ñs)èEQ×ÃnâÇûøáš¼`ğ4ìëwPoÇH£©$q…«i=ë—”Ç6ÔF/.sc"£ÈG©»Ğ1fAµ<¤-µ®ØÏ‰¨óKÈQ­÷l/Daã?ÇÄˆ&6UĞŸÛ ´ºÃ.ÓY [ÀK6Ë-9ÊM	-Şf%NmÙ_ÿójç’‘4tíšjI”½…¨,V?XxˆÒH 4WÌ¤yág`Õ€îŠ—®„}œ5+…ziQÒ>ÀÍ–`YÈ¸Û m%Üóä+ü”ÔhøcÁ²iXŠEÔ_‹dvû”Øål!Ïµ÷¼B1äXĞZ”öÁèd¬¥?Uˆ9í5Â“[%ãX»g­¯&a¹–»ÚPW£Å^m‘h6^0‡q$ÄÕdE¢E|şXÚ‡cbÚeŒk|HØl1Æ9„p	gÜí‹r[ˆŸÂ.ošÍZµıÖ÷­ò´ÍåzŞ¾ÚšçC¯õ(f:›î%!Ê£èÛÒ]ÆèØÜ5ƒù74jASZ½Ş°íÅƒüDÃ­Ñs®5¨#Ü‡»Õ2M7y¼œ§ß ˆĞU–}ºq#ä"-R–»&ãg9„^M«=Ø8 |q¾ZÄD$ómã³µIÖ¼¾¯ó»¥jŒÙ‡Ïİ×äéµMMµ­ï>Ô±eİëÜÙ¬İ³øŞ—ƒ³~Ú9upömJ&1ÀaqÅqmW´^I‡˜–4„:úS”m´®¼s/êËçS€ˆëÌ¨)#*?û“×ˆÏ„…Ş‘Qs'Q©O-§ÆYÇ=m
ÙT%]òF¢:$Êûò6Ë\Ä²34u‚õ‰hÕ1>Ç‚.zÔÁ‹½ˆã;øÖc¶1ˆ‰‹É5d˜4Ápø˜ Œ”NUz¬¡üÄ8 ë-	‡-/+û×îe2•ë›CÈ-}Ÿ+‚.;QP)Xf`'Ê) jN‡dÚÚÎ(Ûûj#DÍÊÆ=EThÇçj!nÖO¢>Á@~hC$	tÂ‹[ÎL ûÂp}¬oªµ8!%ú¸Ì˜\*œ _óæ º¢ßóS×+³ÎXª)yüqn,¯ÚÊ¬ó¯w¯H®3ÓO¡wSË6YŞ(€‹l¤’§èıüâÊ&æB^oYôÕáä§Êhöğ}öƒmpY©‰_÷Š¹g˜JÌ–OØAN0îÀœ
c J  M9ŞŞ*PQ§ÖÙÉ¬Ù"êÛÕü¼7é¥™ê]9eÒ½®¼>Ø”ÂFŸ„#æ$°yßIV~P â^#[è.o›„q^ò'´I†˜İƒúo8¡BÕîªêÛ.wrÈZ©íJÍ58—´¶†´İÉ£ÅsÓŸ÷ ôR!E¨È¥>Á®~bn/Qtµ¤aÌ±@”ïx0ù,Kdê?%ãJÁûnS6JwİH9)îaØŸƒ ·7Ò",ƒMÜĞı¢cå~<¬+ŠğŞ„bëáÛºêƒJµ–G?èÂù0ÿgB úz¯2n¿&É+PN;ŸA¦ïÔÉœ)ôU›˜Û7ä]ß"C¶8Å¯/vŸóœÃÊ7=J‹i¸1ßİy“ÁŸ>œƒ‰Jö0'˜¹“ñşR>€>­G²ó'Ä/NÚÖ)Æ¬Ôü€¿€d£™mh¢•Û»EóôLºŞ$.ÙŸyãÑÌš¤Ú(ej8`ë_z%Eã|D+Üªw«LÈŸ'R4ïz âõ7Å²™Æ0 5nz®r:¨}zõgÅØæ¶™ o-ËeË‡pwİBø-øµ£cC^›Ïœ.S}F8ÑÓH¸Şé…‚G²ï‹È¡È‰£KG¡¢¼tFô¹KÓ®aÑ@§ÕiÛIk0_ª™ÛîğŸŒúlÎŞûÔOw\*äød«\&Át,ÄyD¡OçÃ:­®p&	$.ËCø/!-°ŒÑCæTëK¯è–ª¤¯)’9ùBó-QY_]x!›"…îĞ‹ÕäßğÈl"fWWú}.Öö_÷è‘ÙÊÒ¿Ó(Q’äéá| éGãÛN>5»-­™Q„öÙ­Näÿ‡_E•=Kò€v¬ê)¾¿úbAİ¸r¦K\Ÿ]·»ÜŸJĞGñf[ÏÖ(“±!€PV*=b²Os…Uª?Vı0ÁöŒ347‰"&§Œ&4f(úÊ
¼‚]@¨7§§o.ºïüıb@ää¦Îj&w²¿ÓUâ[Í™Ö¹·çÂêcÖeXœct‡òÏØU®úøµÜŒ{×F8òÎÁõuªB1fğÂfAw[ìø¨:p^˜5Ì+‰!¹"c QÔN#ÊÕ#o6‚òÌÅg¾ÊÌ3Hi¿CZ¡»ïÿ–œ¡rÍ'&‹u×OzÑ‡İ›«•ùœw‹0YğËìl¼öŠúÃş~S¥
éTéLƒì(h¢ŒF•šfş0oóÛÁºÍÄrÌõƒ(„J¡e|Ÿıô›f³³ÈWZ’–ğ~)Ôì¤°¿-–ËY¾Ê÷5Š±/M=kãXk½|§Úv+ê›Aè‡öÁW3[ëfÆJŒµÎğ:èÂoãƒ„¹ù#ï°I+ø»ÍÎÈvŒô”
Z#Faïn‡êĞ¹Åm¢jùƒ²¢93İ3¬
šG\6(ÃÈÉ6ë´õ£!.+é'”¼ÈÕvAZ/u`ôÖÍê¼FUSmº>y3`(ç%S^™üz¡‡ÂÄ{@T<
uÂâ¯z±eğiö·Ÿ²aÆÜäè@eíª“RØ²HÅ#
êrƒ€Sd2ÄŠj›\)(FÒM²·„ö¶ŸV±ªVvµÜ«f¶L¯?­C3{¼zr-±gc»S`,?Õ‰l2ú;¾2óZ*Q\7ïW+9µi‚ k•g­‚4Õ…%`|“]rù‘zµ]Rñ8íêıâFÅAâíÑG÷©"_şşÙ0ÄÈšÃ5{œœzcèWMQì†Ö¿%°Bw)tMQ8Şwg%à+ôáœÖşqùïƒ gMu‰t-¶ _¥Úµ¨
£êÖ\C÷ÖöP=@@ĞiL­$ÇÀµ…ì]éiBÓµá½*51Q‡„ûÉåÙüÆ`Ô¨¬¥éw2E-?ÙÍ¡2-òĞ	7­dG~çÇ0Îkd6Ğq*Ã c™QÄoîwş%“À¿,#ïpËõ—!0Ë—Æq›(à”¶ÆKàö qg”ş:_¥ÊZ w_ü'V‚k4ÍöÆêÄIR:ø<<ªr,D9İ-gMuWÄÅ^°ÿlòÀŞÏ£œºspvkôB€UåÓfãH[€ıİÿf	ˆ:HØ­jÚ9±ÛÒ-BÎéa¢µÅº£™&jTÜ
ˆborCñ†sx„(X¸÷ÃØãJoÁ×iò@E-†:—^h2”Í¿ª*™Ø‘qS„ğB¸g¼¶5·£]&P0ô6 ">®¢ÆÔtçƒún¸ê@3¸Å’:¡£A— ‘éå®]ØËÑÍšÅÁda&õX]y4æ'å:èØí :_â˜äMõÅ»‘]øLrÒ¤Ø®lR©3ChyQr’k½ÍÔ¸²Æ/I¥GëDù^/¿)îªÃ=u9½¥AT›4œÌlÂ	Æÿ%ú'õ®±gù-aWS®N¯¶G;×=¬Àï{!ã5"ÙÄ¯$GÍl›QLWª­DXNF®O#-çıÃ"’9å…4ëSp¨+­†çÕbæ:Ô¦YKT{a¤(‡ÿc8Sñ†Û[°Ûo>ø2E•š	¼¿wñÔ1»ºHP±Š1(¿Ú—ÿşôB5 Psfsşjg|	âa¯©Œ‹Qª.Ô¼ù±} ¬ĞüªV¼Á3œ,_œ1gq×gª×p èí*WŸ¿<§Ì@‚üaáÕ±²*L-ñûÌ¬¦%TæçVÿ¨Z§ù®Aÿ7‹cm­kJÁ/˜OYUÉô!¦ób»òÿ?YzÏ­,Šv!ŠÔëPyÊ6[¤Á¯ò`:ç¥ 34Sâ3Íî¤ôïì˜a7v"Îdzö]š¦m¡qkŒ»‚gl:Û¹cÈE4´MáyPª«à´]A3á>aõ<·?W:šé­«ì:ãßh"N¨òÀxd%5´°$vÙ×uR¿W•êÃ­
ÁNÚwC³~Î‘·øÜÌ	z† ş5šhŞ†¡›s5Ã,¥[ı~ÇS‰ø5×.ë8İçâ8KLKœ^º”ßC®@QÌÌÜÅ«aÒ³éŒàè…UŞ´¸)ößàJôb1½åİ6`_åïÑuŠşøû‚›h[Opì9:À§ À;v@#ÀƒßşcfÍ¼Zë59æ°­\=u²;;â|Š¬şu¾ÅQx`äx$Ûnl M ¾È±‚ù+±Ì¬à¤ÑØ›¯ÿY"ı°˜Æ†Lq¡FC7çl% ŒÊòM.‚B4÷'h/re7ÜïĞhß9fo¶\² &å5P–~ø
nªK¬‰&İså‘°oÓşš6¶iş¥îšÎ“ÜŠ6-¨¥á©	İOÁ€û÷~0æÈd fqe°G;«Eğ~•¸fÓûŠÄú°Ğš˜Ğ?êP‚(ŞÓb+–h–ZÚBãE=r½ŞY‡/ØMiõ³‰ß˜¹gçSlr… gà¸@ô£ˆÙÏæW‹í¶quJXØÜ³FıÖX5ê„µqP"ôà¡$F_ì¸>­QñÎº1üÖSúT‰U‹Ä6Ï¶°áiãw ”¨¾ªt‹İM)Û[pEĞ¼ÚSˆ°„ƒ°Îk¶I¸^§-ÄÇH¤Şıš”¯qí²1å{±ô}ÓõPÍ
½<¸˜Ò¡Ü’7yÀ1íhÌ‰Â!uådOJ”W‘à[Ûë„òû!s¤½¦H¯vO1¶=F©¸ÜËÉY6ŸyqÊ¨å@T2ÚDƒnIbïŒÓƒPñ¯²Õ'M^bÄ²øE`®d›¡ÙŸùï×ÉÕ»é FÄaÜĞûØ;ôMÖÒ7,LiÓÌòï>«]Aæ¸³‡';[ba Bö£Ô;
‘l¡j`¶õv|
º“µ‘?Á§O%ÄÍGl¨ñŠÿGu'1FAõ¡@\Y2ìVS
ˆ$OTùUµ8¹ğ^ûH[0P[sµ\†;>8â·E”cì+˜hlt¶äû[BÔfŠ#ËPÉòÍçEÃ$VxØŠı6NVı]|$ûàk¬œ¢Dä ¬½JúÙG[„jfÃøõ•T1¶3…0SÄs…f	\èUÓ¬ËáQ÷"£ªOègì«l>Ò‡ÄŞk	·†FåäƒİU¨ª öÒoŸ±	&mÑ·YnueO>«ğÁÎ&/˜à8‰M}éƒ˜ƒ6I-Ú÷T{‰‹èxdˆ¬'ÕíëàvÇå,€’O³P=soP(Ò•2Âéæ¶u6üaKşcÊN*0ù÷’•7¨Xõƒ¦ëÅƒÇXzf!şÁµQËnnP.ñ%–™„²‰{wq{MVğa3:E[OµRafeXùìDy†ö%¶© 	‚†Ô/”süÑê.Â#;ßÉIœ*­|(ÅİÍöÌÄ¬öfd~PXMÊí³‘4e&ıQe.R+:Æ¼d`ª$§rédğâôùãj–Ù»¾$T‹të·©éNG½æIù&ç)Û¹Hûái‡Š
û¹yè‹o™Øä´àÈcšêçcßêa5ˆD©©µmö~IÉÉ•1¸XÅÍ*hÄ~¾S×¹'Pà(²¨—‘«°ìÅº¯§	ìSuF°õ3?Íi~¿µ:Ê,c*|‹ê&UYåÆb-êìkyî+dbµÖé‡9o–÷ËF±ä»:#óQ¯à¹ÇP„oƒ±ºŒŞœ ™:)¶}—&çúaz÷ @V+À(2QÚÙN‘ÕnD)6¸"*¿" Ëî&D…SÜ3\ÔÒ¥Ò^µ¹1NWõ¨^Zw¨á}Vg¹Ï’­bZ(®ø„öÓ²«\‚pgÁtÆ¬lC¶-Ó$StŒÊ­a‰+€DÁã©(š É<-†±²*sı‹šH)6 š|B$¥/næ	¹„@³öÓ<PWx÷±o/6gÃ=Ò0ZÑ¬¸Ä	¹ÆªZbÈ¢šßQøèrÛon°8Oø×Ü	™;õt	3×›*Ö½vÁ‹
£à“aïRPu:×ä£³4*FÙu¿íÊ™Ü`¹–ÚS§WŒ!oÇ\Dó\ÉÏ„ùóõŠB=íÄúrõQ“_	Æ+ÎÔßËx©Äİ»şmÿ<_ÏƒI ¶¤Š(T€Tªçè¡Àÿ±g—™A8Nu>şÚ}7ï¥¸”á+‰.úƒ¬Ê¶º¥@2÷¦ô@«Bæ\¢Å=zH)Ú%@ò,n$ºW!¨}ô‚*àçÊŒÙ×ÇÃ-³8’…“šÎO«7&7ß…i©ùşÏ¬P1Y-Ğ}›ÀîÄûáÒÙ®Ÿz`ïKt ¬Áñ”¢-Ãµ/9(¹Ñ?ÜEaSóÏÊé‡Ôz{cİCÁbáÂJ»É®h	ŠPUğõæğô‡Rúd?TNPÏØa÷p±«r	^‡õğÜ÷¿ç\UOÒÆ@Õ]X‹oğ2Õ›I·­²B-ã£'V·êè¹õæ=Çêë wl”ââÂÚÒ³Ná«ÿo1IÕ]Îº%V©úFtØad¾ï›/¸²¨®Åb8>:«‚İ‚+üÃãì¾çñä?—­¶¿çÔ¢¥N>ÏÆ×u€ƒzÿN7t-²Wñ™Ëgx:u+W^U[@S/ÜX>¼a
õİ;‰³bGÌÈËçG
ˆ»÷À¡ıj¯ÇrÈ/VõECùn·¡Ñ :ïSü(ŠãªœaÆ>2šc—Ÿ¬Šõ¢Àù9·?Ã‡À—¬ÚŒs&¶¾/-ó¨Zç|ğK•ÅÖë@âi‡@¢P-Ç}üm6ªsÃWíï ]ÓB\à˜¨ø›Õş8Ô(Ğ3ÄÖÊL·r.c˜ÉéŸã6ú~pŠdZì¾šä(‰µ2ÉçIÈ2ÕÊÃ…×–°ıß‚C÷—ËÃÿ#ñ*9åÀK2nöæ©mGè©ÓQ€õ’9>ôÁ¾+`’•oÏ)#¿îuWû&ú.€ç1Ç¤aB€ë0xÁ¤Ÿ%)?šªl}jOÖhnr³.Z‹Eé=• a¶BB?\_}ı7á~˜—Ã[!Õd&wPÅ×_ë”‘.‰L&¦F´ô±1HmµÛë@2×¥d¾@-“ºğ™Õ·E3ş‘Ü”Hv„”søItªĞÏ„L3òªXøÿI´¤6X6¸Ø·y¦èH`9“NÂŞ¯Š|Ló¨bå‡M
 QcİyÛÑEZ=Ì*Ã6‹›*?3×‹ğÔ6ç«ºA59	)3zÿ§Õ8x`¥g
îÛí›Iÿ=Æsˆü¹ÜPÜ›¹Ö¤@QÂ{&:œ²b\HtMvšæÏµ‡ÀµÓ¬HÊ‚vÑ}Î!.Ñ´”ÁÑB™\Tû¾ÀËEtz®­óËgà´öÙ_$ƒ’óŠä¹/„ÈÕÑ R^9©‰ûXD¸~íÿâbg`qó%ŒÌ¬c–ñÌçÚC‘zÑ…«{'ŒşŸ¢ÊS7!ğ±a
#)è4±ü÷,œtVOm©9èÄc™EæË­¬'ùzµu7©å¥-“Šô¹z‰BE¼Ò~bM ÊBÊ’ésõWu7Ô¡s`Æê}´ÍıƒÍ½Âcëğ›¸ÿY¿Ğ²ÒÂØ‹÷ƒ¡gTÿs=ú``G"ƒÓ?IHFœ;¦€n©2Ôgêø¼ßMË 4TVghXaõ»ùŒ¥Ä‚,¤Ï«å%Ö˜vÉkèZO¸ëi&VF¬$…v9¶OPm2	s‡š£É]±ÑÅ/1y¬àA5[jä®$¶¤Ò‘tÛQÃjêt5à¼Œ‚„¯˜Î,9™x`ôò¢™šå¹M8Ÿ6&³HôÜeKì^=p%üî~ôÁÒFµ›4ÂŒ\ì)È=a4-U™À÷(
„Ê‰œo™!YÂÁ¬·|ZµĞ²Ş`©#¸-MzŠ2Ù©¤ ó¥~ô§×Qü|Õ]$½‹›:ãT^ÅU18Ÿá Ğ ©ÔJ™0>WqZ Å
€}™Q_ú@÷UŠ÷ìI.jàZH€;á„Ò±q@9uÙæÏuy©mÍÄ'çHõõ’wÀAAâ‰!Z¨Ï&ù;Æ›§dêOú7KÇ­AÈ„ïGÉ:´y¥ÛU]e<;7şêOû·Øcò¤Øèw©ÅIœ,am5¦ú¥Ò!è>•ÍŒ\Úd‚ğ8.õyÑán;‰^—.[|±+?_}ø/şé2ğ'Vlşõ97«ŠûÖ²%;²N‰ıª”ªÑíw¾j·Û>ía±`_’ªÏ_ò:æÚCÂ0Sæ“1‚bÍõi€sA§¦ôÓ˜˜1†ÁØ€÷~ÇäÀ1uâ¢½ò¨E¶8ômh‹Ç·üŒî™uÈÇ™ÖzHñA^Á"€­P<Ä™ñƒãnçÎœDlÑ>UHUˆ>¤¬áèé«p¨gp(Ñ­§”	 «§{Ó;Hš{j#M*–6…ğıöÁ­ÀSXŒõb×…İYí
8Is×oÎÌL¯Ê7íøRè^ÓJQœ9Pâ"Úê·ìïêÆm5Táş“5©ÉÁ‘'˜²B#eæ!	Ñ÷Zñ TÓ±W
»ğˆT’3Wà
EöeLËÓÉ´ÏñÀ°9ˆ)äiåó¼ª
Ù6a0Ûml·²…ø½K}”íÍ!{H1£(”7Û4Î¯Š|–Sï}‚BVıYrI¢€L£×xÛ©ºËaÍtZöğáÀ·H]¡îp7Ô¡˜¥¹Cpóê„½^÷³7Ò³®D÷ùçŒ©ÈU…z8{7 hañ´_\±vløCÍì^¡Øh§œé¬_¦E‰OÇDúè—F]àM‹‹Nö£ †—sªµl ‘8ÍT/6áÅxµºê¥¼2l¢ˆb•ù¿Ü+B$QâtXÙAã iX*P”©9J¿>"”¢4öÕÄúÓQ±ßœ®”óTƒTÃˆ«˜c˜lS°¨@·kON$¤voıwaøL­w&Í»‡eÃÎR#TDñcñY9³è”™NÒTuÃ¢U\»ew[+EşKv\@2!µÙ}~EÃ¿‚rw‡Ç„T¹xÚzTè´÷Šq‰İÅúé¼>C§¡¹äƒm²Òh¡¶[#j.†ûĞÓ¨0äïËŒ±ò’Ğeæ:<\}«3NŸrÃc"èZ{’‰Kh¦ëğŞœ7ğ¢f%Fé›¥?…ä¯Ü5T…<ŸcpÇaP‰tÙ#Á+O¿/îï(®9‘'U³†…ö	z4ƒ„Ë
u˜×÷B—\Ú±‰ËŞŠo#µ9|ÃßzõÀ·Ö¡òÛRuEúCUÉ´v,¤ë†qu®{¶eeÙNfşi, Î~•Hé‚H-o³Â4	Qpô]u²ühøYŒ2fgkĞËºv y›>úşJ«‚¥dêùWî?²J²$ã•…Ñƒ»yÿui*~ù0Îƒ{ÿæïöœí›(È«ÏºR\ôŸ‰È?¥t˜oú@Mdoöbõß7©Â’[8ZÂ1ÀùPB\"¾ÇFg¡dš*Á O¯	´ÇBŞ­œ'~g«³ªéx/ŞÅÃ%Ïä]›ŞAü‘nq…25L@¯»d^g İH“¦|	g‹µjÓZ>bñº‚¨­uAı`¶u¤éhw¬ƒ ¿SÔL’½š¹l¢şgî)×ó"Ä§Bp´ˆÂïNãQğıãélWù“K3îÀ~5äpo™ƒ×1›Ø½;\¬8ìÈ,î*­Ma‘’®¥¼êú…í}ôimÉA¸¥Ö~iª}ìZ3ôç¨9c»Z÷Ä µòë¥&„‘¥êoîÆUÆ>qmU¿p³‡Ä`Û¢j%§n
Sp4@›ˆ«7š\gŞµ
ßßMyvØŒ-`å#×£ğ"ñ¯s]sK=Ğª0ûqDg”€V“»nEŞ:]wµ	ZT–v+úxJA’l?AtÂ*åÔ„R·i¤—Ä„—Ku×ØÂ3·rÜpeºÜxëqıhÕPÌ	Œ¢•~Û¢n>”
ÖSÚØ}ì"dÜí6\\6Í¹˜oÁÅãW¥GaÎ‡eöy_İ	µ¡µx)ô¥:s”Ÿÿ¢5ë>A;ø@È…³è¬jeFx=Îi~}o	•\BáU!ª’îÊq,ÆÖšt÷¿ö„bİñoÜ1ÓCäíÈ·:UÊgòtvÀJğ WøçPÆ'ÿ+ş*T~c½—Ïxİ…¨5¤·Ÿ(àåóWE­ÏÚóß x®ïbì”‚[T½Ke$ƒØ¿³ hÙ«	ˆYò-Uz¨W¯Pİ|XÈÀEñÂÍ¿Pæi€Ş\y‡áåı¤©=P“&à†fI
ƒ:lq(ÊÉy5½¬õ,eòLşñwDíÆ}"Ñ@À¨lî¿ÒÎø#¯,{òG¢–”ÃN7Md²ˆ3áúz¾÷Ğ¸æ
¶ašÚ,‰¯Ÿ~t`À)ôJ¹JW02¨MÙË„fÄş½x|4\2,ÕrESD6È›ë<S}í²ö'©ãë
Ó·^²mÈâ)&‰ÖHdÑáb£ş-ñÔj¿N"„Ÿñ¾¥ôY8`µîŞ°I}ÓS¡?ú<Mjü6­äZĞ•õö¡ÿ¨Xxuè[t†.ÇµÛTpJñĞ;. Íî÷Udrmğ¬°ÿ ´Ò¼ËHŠh \ @ô‰J—4®‹u‰ƒê_…Gˆ.~¥²ìÎzé#"`÷¾Qn°$»sû$b_vNÿg…Ç€Ò7x.TÛÛ¼šM(¹îyg‡7Î]‚&˜âgkí“qª4Ûc´ìuO¨^uCé¯°›DÚ©l¾U±g/Ì#rÁÜ‹Ò]+lÚ÷~k7_ŠKÖ6v:2'°
µÿùÆ6Õ½ène)°;º
}	ÆîòI…Q=_pBMäKŞı“[p¨¡Q³ÿ-áŞR)uÙ9ù¡®°=°*CHB¢ãŒÆ´Tgı^wxÆFz]^'%£…Mq§fÿÌuqØÒ=Ñ‰¶Tªš2Ë{½¤Yÿ[5©÷AgUBÖ¼³J.ú‘^>`÷ªMğkNH‡lÄl2Œœk2ÿ–úg;v1ONá­LÊ|Ç_8-„vN¾ôâh"ú¥S#·óš¯íD-âşEñ¸s(€ˆ`oŒvk(Ù)¹t*`1d›2{$"p–….uÆ²hŞëF@±t»Š9xS*~•I0-'†7oÖLóùmÀ?»V~&†^±3^`d­èâÃS½_¿jjçœ/Ë$ëU<y3›]`Áqí/zõ‹Q·|õy3Yü;àÍ»´Ì°ˆÌCRÙôp¼¬`eİÕÃ=J¢I“~õóbª†(òâ¬ŒÁ·@Ï¦ïŠóÁŒab7}µ‰|mP—µ^	‘|5,-õñ¡÷SìÚH·\i‚Ä“Líú,Ğ&~A>—•à,ˆCŒI iŸ¾À(¼@j`Ğ*õ‰Ú	9£(,éÙßÖN,¤†R®„'"es —Öisv'Ú#FkœKrÛ’ëZ?ù	@ó+©ƒ¨ÛHóüÍ}­°ˆÜÊV²SÄ|¹‘J«’åmâA
½İ§€“vãı•fòşÿÜ.¥;¯{ù4öD0,¼]‹@‚:¿x¿Ø;&ã÷Ãé"g˜ ê¸É˜|ô'^W¢füÄûÇ<ß!N²ƒŸ÷ñÿ–­HÓãñş•¸Kê2]¶´ÚÙ+kNó«)Oø“PÉ®ã¬~²E Şwî°ŸL%¼uï°Í×àßŒÏwò:sƒ×ú…ŞÃ’‹ÊºY!B<srÉ¢‰·@Q¾P˜E"¦Bİ7RC@¤6je¼-SGZ^¬İÚ(—À•:Ï	_ìUvˆÚ	²Á#–Ãºit«SPôG6Òm™#¼švPË±‰óÛGG Åu…g6¸¸¨)-¤	Ä¢&:oáÜÙ6Ø<µ½0ê o(&”„#[LIÏM@±ÉìoÖnÛóÀ,mÇÁ÷}‘d\¾†.µY%+I‡}öæ’b~•à‡Œ’Õ'.Ÿ‘6=Z»˜r£´Ç„à:ª[©½uœ#š¢e°‚9
¸ìØn”R–“ıµh!ù­Ğ«—Ò Nì>ªv¡³œfÜ§Qñ#+jlŸ”]§µ§iOõY˜\
ÄÙdĞıêF[·,s£C~®îU¢Ò°©ö–	DèÓ—u²Ån±Zù•ßG[]}¯U>nIÈš'¾·÷¬:¬a¬­"é‰fâ»¢0„ã¤a7ù%Ìò{çµa0Óvofï›œŒLÁóûY&£kFÃ¤»—>Û½Ö±ÒbäjwIõHW"ŠßX=¹ìœ$Ò$\«B@ˆ.Î³	jöÉu’¬³ğÓjÉOlÒ(°ô³eÑ˜&7I7•¦ÿ´ÆË«»¿¥ &ÁWµ¢–ÃÍ+éîĞª•vÑ.Z(!v$]õüÜ3e»ÖãgA¦›ñ*
—;õPÎƒµCödhJ»ÔX?mÔƒôš³n¥GINÙøXšhÒü†÷òØ(“±°LìcüàCÛÎëÛnÎj¼ñ$/’(‘œÄ$Ô÷åÂO§²a´Ä¦XƒA±¡_—%#¥Pú×5Ëöè-=>Ğ•>‹ÅÉí‰„äro±_‚$üDzÁ•ÚÒ‹SvQ•D«§,ÚDÔw‹£b¦/]J=§)¼u<}o€gåÔR°Aë–÷ ›‡8Ng|Êÿ0Å9=Ù½ˆ¡¸V¹jûÒîüßJ	„P½‡Rò7Æ5XÏIëƒ±ø®^ –ºcl\!iüÓãÊ·Š×yËèlû^/cÓèå³
BšÔÉiáÓ¬@W²cƒ]=X®;§Û‹ò#M\|ÖL;c~ni%Á
!Läš\vVO7Õš¤ò(TZûÇ½XúÛĞQ éSœ¼¢MJ‡ğöt¾y<Ôğÿ†—O:
wzãåêFîPpŠiÓT	}C<ba¶.$™÷W¶¶`ÃÃ&ÛqğÅÉi•]-e&g½ğ/ú¬UÃ
¥¸‘v%
¯»³ÿ|sPF‚­Ùºµ½Sm‰8İ¶•GTÊ½T8eH b•QÅB¯îhãÈ!sÏ$›qlC¢ë7Õ$aÕ¦™|“NõOdı;ƒÒËm0µip÷à3+KV{™Ãº)ıf‡ÏótSŞ®~ÙÌ§4L†Â?[®Æ+0©OéÛ†ûæ³8JõI³ñXûmÊÖ¤Jë…Òª™;â&àÇÓvü$àó÷\ûÅøøW ¸é¹«"ø3Tä½ÓÊŸk–4Wn“‚ÛTÿ@Şú9Çôø•p-ÁHXè‡²4óA³¢Ë{†?ŠfC©Í¥·0f±ë¶²bËy z'µSe·ï%Çj}‹”,4bèv™ß%§ôÙ—P}€c±Í
T¤‰æQ)ÿñ§¼‡á€Ÿ˜s“Æ#Eñ?­è*–¥ª8û È‚!H=&¦í¨[„í×|¢s¿«Í®F|EÊ/>^ğaÏ[µårÉQ?iÂOëykQl»-¨øšÌ-¹+ä©Wç*‡ˆ'ÄNø=¤a¤7üTmiU/CÚŒç=b¥±n]h	1}Ú¹1Ù3İ0ŒçšAÔÕ­^ÅÇáôÙÓ¿qÁn1åıÌ}ó!Ä‡<üºÒúœCzQŸâšë+ cä“ëºò•yFx§,Ğ+ãbÄJ“«ÕV 'q:şDRï¶ó¢ŠøAdH§~I‹î9_ç&~~xïçÅ‰UÆ¯:Åè“{ŞhU[„íkşòMVÎ Yª:ãæ–¶5W^ò:ó¾Ü×O?lA{\DıÁ)A€€_ïğ³…GC”-æZ3%.0õdšR}ÿFÔ¿I«6f6#Å¨,É@Ñ•æàˆÓ7¦—ßx•1%½²‘ BKãp¶¶¨Ø±œ
ã®ß#n_m6™;Í‡îõ9ºe°çŞTÊœ©'aÕvùTÒía¬’=sòÛˆKò=c—Í´ĞLqîK]ÊOTOOZ¯ƒ2A<YÌ"Áh*1õ?Î}4ô@©¥•ÃÓH¶ÊÓÏ.­ş€^ E„9·ëÊÌ:pÚ”l$/üt™GİÓDj#½›yC™ü)÷CŠÃaKF—p(@Çß'Â(ç\ßEWÚ}×-ª7Ğ9–4.‡ÊPT·À89C×â_|ÑØy™~×ÚG,‘euÍâğÛzY]/ØÙµNôµà¬é¾s¬¬Ü;b!‚>6î°'´®&b`Ã"¶~œbäTœıoŒjhËµ`§n°C6}o”şİØë¢»¨aØ¾b½ŸÃ ,œá>BO°¬ğ# GúU&z™KˆğXsìÿ/¬ÅSNÏ‚x³Çù®
:Ea™\•Y´¤óåiÈHY½¯-m©k¶*œV`ş·é^ç×Ø£q ±N”^Œ&qìªe™ˆO‘j$ÚÊ"£n`O{@aÔÀÇREnßä'ìqw³ã”¿Ûƒ ßS™ÑÓ#»f‡ö5”3bˆ«aÅÑ$—|ÜŒ´ºE|³Ù¢×
”}º~ì)v}0À›­ÕD¬t”¦ã¢ßé³T€vaA˜;š~¥šM2Âh~ìDÆ³Nóy˜%üR(¡‰È§±—×ÒZı¾µ%ª´¦9š²%›{‚¤RÔ—û°øm"¢¡×q´<hÑÙÎÕ#ç@*M‘íqòE9ÒÁæS3c™Ó9¡k£üôw²‘J«‹Qã æÔ”œ^àZPÆT®×äÜBœ•ê	8AÜwóÜ]˜‹é;KoAüîP&iy]ëpÛÁŞuŒ†¦‹¡‡Öcdu›	w9H×QJ>
$°ª§«l$ÉÑImQ@Ş«+›xGÅñ&JÀ”Ò¥­Ñ”œŞIü+R²¥LtÙ2Bşa&ø[Yœ‘ª’|ÆÒ¼fÜI—Ç DÁHÖ`Î8ŒMr½ƒ8}á…‚åÉe´4X  á„y©(ç±ÍÂÅRÏ®Ø?Yj%|ËcI²P%XÃs2«>Ùzªü[zK½7+WûÕËujøo÷™È§ÍkP Ä*“ç®‘fØÎ¥|Šå/ƒÓ—ñ)y”ªŸx†ÈùcQ>.ÈÒmUk'Ú’mtoívwÏİ-É¸Ñbì7±ê…nì¢è6´VÚ>:˜ªonøÔ©)£åy/şˆ±1(ËØU© ÀG!ÿ#—ú¨ZİŒw9[R¸´ÕA‘4¦Vş½¡µp{Ësâ'ÅcĞ7‡=¦ô6ºPWÈá¯¨ˆüÌJ“¼ƒ‡WÜ#îRECÏyÍê {Y?rL:ºË:ÜÿÚógt?aDó‘9†KÓƒ3‡#/içîùõ‡‘Q‘Ï˜3i¨õ·˜­ˆ³çL)ëß”¨ ¼MíÇGüĞ•şµØî£†ŞÁ˜‹]PÙáöÑ—@Â|u ó6§ @‰ë<ïå”	¢ÙÅt×7öÖ‚Ï;¥,˜’·t®ù–vL™·%ï:\¨³ÿfùŞX.-Æ]Úúo;ó!gh“©	Ö+¸(6½ÅÖ¹æO9¬ËA“‚W|TèÖ;¶óÉØ»=o÷%2:E¨µïÀxPÒÏ‚T‡Ki®ù¼ŠlÀ˜4i±¥KQ"×fÈÃ³•5ÔLÉœ«rkï|¬º+ÉÈƒ®…o²F•:@»wò%HÒ«  _¹¥÷V©‰´œ“Ÿ¬Êû`#B¢/ZªÌô©¸¨ºù¹P¬UÔ\ˆ;4»d’G·°jxˆQ8Â¨(à ªÈ69½Ä*EæIµ†_…é¸~Ï’ØE;0Õd›]íoC]Š5AÌÍ%%IíËè3•êÇ˜¥+C·+úÜpÄ$NAöíßç"'½î¤Ò7ÅK2¬@0œâX4Ç¯´×Oµú`å<€ëjçp€h°¥şqnYş¨°ÿÓDÒ\^ş#¤SŠ[xæU>rŠš™Ú§.N(’†<Ÿ¿2¬	Psã§†â±ª1pÑÏV¥)64¼=½´ÏAXÁ†v‰÷ß;VÔá¡çµ8P½Ÿ6ğŞ~èë•ıßgï¢ø3x5ó«|×OÈO]ªåa„_(ŞÌß¥Óª*ì†-Ç¸CÍØœ|ı¡Ä‘ßÂË^ƒ¿Tõ·Û™j@üº¿ê‚’B,¥.«¸9ÚşdØŸş~`ÂJûÄ¡o/êßœo¯Ÿ°”à§ÿ–}rb^—œ43Bz&GSöa?½	–]ëÿÖ.ÌÁ š+IÉ`²ICÜ1y(´¯9ĞË†Z…ošù`QŠ/.ı¼FCøI›ıpy9³X”D‚B×„R—r²ß­åÌzQod¹¡/wDaRœhÔ_3‹BÇrínŞM×%dØaºdów$¡¢Å³Ï¢»=÷.!Ğ*§_#a·}õ76IY3{×8şÜi…½€ı0 £›¸!òn@4¹9)'1İÕ<÷Qã£©`g¦¯¤/óñ­åL= V6\5`³§ÉCšÖ»+Á©9áëÏ75ö—©uE„D«|…|!É!·xš©æ¯!úWı`™-ËÛù9¨X§b†J>µ”•É‡EUK¤®sMJÒiL,SĞµ!M¬ûÖEq…@©@‚4BÏ²¥ÆMÕ
c‡7È²¶%w)Êğ	ÿ®õâi@ş©NbUñì¦şù™3¡H
 Îğ6(ğø¤M€£;«³ÌŒ¨Î€5~DôqôÉ·Üİ‘­¡ØÜ¸ Ô¥B»Ëê¼öËğÈ&é/òö¦ú¶&nKù¦¶ÎürSß—98ÊÇ¯˜Ò]L©îªÙ1{õŒ·Æ°0%Ü	Ø®¯:$±ÓãôFÇYç¥T ‡æK&ZÍÿzS¬Å>W™Îà îã^ÙU÷òlÈmş  a¹· å<Ö•ôJ\®rĞÙ²\&çVøş^*Q+A™sÖâ kÏAq‘0$õP*93Ïğ‹p~Ğo':Ã@‰¹'ç-æ}ÎXè¬Ç2)b‡wM…úï"ÏZ3Aß`ücé‚–ŒkÊšÂÕØf‚'©Ùerc#å¼¨x8Ÿ‚²dS{ş­K¨7ïª*ö<’úÉV©XPı#ª¢3›t9ÕÈAÖdŸFZ)-XÍ|Ş5İ&Ø£G7˜ƒ·ĞŒo1d7¥¼÷Àş\€å«jûôß/«Ïıà\å*‹./ğ±—·mB@?r1÷ıFRrî|A%”­V‹Eª8W²1‰|ëÑŒ6[v;Ïıxe|"5TÕôvy·C¼Ó‰†Y°‰fï\ô²ŒÕÈµ8YÛ4††£ûy‹+ÖRw°Ùy>'¨sÏJe•µ¯Òã~L`L¯ıuÎÑ1›à.ŠL¤¡mÎ¼£¤ÁDz,)+I>bo°,mËşö	¢
ƒ®/¾Îß×tÃ¿Æ$åø­|ÑÕä˜‡cæ…£àDls8©[aQW‰š’Ñ¾HAÇf|n­LÒI¶Á.)ğ¸CÄ¶sQ.®éÄ¼ÑK}…ÒFÈbB“t`:dÓ‘C
sÁW/VQã0ÉGgd4nz%ãQdR¥Îa Ì²·–Ûúğ½ƒ®Ëò”"Ò¦a÷;×ş(ÌMXòI®?@¯O¿>½oy\ÀŒı¯Æ«f×TÖ¶…ÖÚ½Ígù|eâ©wlCÙp¤YÅÅäÄ2L“C¤Õ²fµš3âÆ:9µ,ÕÌúY)øüE¨Io]g91DäïKĞœ°wÑ-Qlw±ºJÜw;éuH×;a—1@]™±ì?¦|—É³4¨;xF7HZ}Oº<‘ö	áâ1İƒ|J%­¶#Ú¨É£1üiˆ!gƒyÄBîÿq3¬Çp¾±’¼CQo,¬OåòÈ¥^6eÚƒ®íô›!L‚ z•¦–WW8É NJˆ]ØÅ¨um*S@B}¦cÎgŠie|><¸H(ÉÈ”JkÂENOîFÇZCÒœ€6'=g89ŒñAäÑªtk3F¬‡ĞåË‰´§‚ÄºÚ·5æ‹‰gh1õÏ×PLXÚ*8Šk /ë*:ì6_Úª™&ÖdHÉøÇ[€ÒUzó³¢±a8K@cò ºmËä€İ½HU«ñ¼´ç7YO…·#¶ĞˆÍ)'ëƒÛ¿ 3™E÷ÙªşÍÿ8š‰zÖÿèÏgQÙü^Æ »Ekl®´– v°:šøÙÁp? ¹y u­œÏÆ¦€*æ Œ±[‡œİEà„\ ©Jñq:¤	Ìuçb„¦:ä•{@9ì'tdÔ¹É ƒ®âb¿ŠÈLN^¢Ö‰+„îÀwêGH‡i>Œ+|® ËWÓ’e¤ÍV] c5e—zÉ–‚´°GY¡A°óz´ÕÖ¡çĞkáM@ğF¾(Õæ;‰ĞßÔL‚d÷È¶"nƒ¡Wø¹Ôù/eÄ¹¾úäŠ#U×áÌ€ÂÛ­+0­ÇÅúñˆÄ4ÊÔ15!E^M&g¹»@ZQ¿ÈüdSxäX†“#=ŒV®‘¯'âiîN>%I,%‹N–ßDW·C
‹ECN^©RÇï–ò;ë+Gs†^v3ÒÍj…üFYĞìèìxPO/½>O~™‘®§!u D#¦÷ì¨CÅê¯ÏÛ"™Øœ_ä6}éC~Õ€Ÿ#†l.mÌGÖcµŒÓ%Ó’atÌ¹ÅQ(\gã²÷7µâw+mb9©|Î#T!£õg@/‘¢xıOgİç,Š®»m]¹†ĞÃ÷V#Û\•¿» 
=ï §a¦wÌX¤šøjÒC¸%¶‰•[È&æÉJ&÷¤{’¶D W±_×Ò
ô1ö“’ >€eö>7;{U85Û7­rQ•;h6‘|§<q»¾İ} eşÌLÈXÓYc“™ó$çcÓ@‰:gdd€1üÉûİÀèW§Óï-¿ö¹á6ÚÕQç×ò€›ğªõ(©ôió^c‚äÆ°QùYnªók6ÿñÁw§Z
Ù¼–æTû­ºKÔqq5´5/êÄ<hÈun}B<åIº`Â9”NQ¿Z]½p>VëmïR9‘›–/ƒ’³¹ ¿7-’£÷»Åş*¦ŞÊ}bØÓçÇˆ)“°gÊ_ÿ0üêà6­Z-vy(ÇõŠãd…\¤,ÚîÂÂ[{L+Û©ä-o¸7voß¼„útXÚG;ÎŠÓxE‡¨á.F`O3É$7¯jÌO–êdÁ¤IÉsaOUÉë›(:©zÈ€º¶YÙLŒèn­ïÎî¨Ëlüvf…’r÷ÒÒƒÚ­Õ›êº†×ØÂáòj[“f;Dàß€û“1|A¿(Ò—²ÖPµ_uËIºøĞ\¼Î±½ƒ˜²FU”2‚¸äû%µá¡\2??øº-4¾›»˜û[8>›¡§¯i¼©ÕwšËfœoâ¹¾R»Şa©-İ¾ŞªôŒ¤nRÃú¦æ&£PN±âõÆ¥#ioXã=tuÑÔã›™ÍÒ:H¤KIÍ*\ø?´+Sı×=4Ùm´¾á|1uø«GÖ3?ÊmëeyƒpYb£­l |§€r2:€ÿ ĞgÉ2|M(•@şÑÿk«ªTİ,fıƒº_<æĞÙdãûã(©sÀ€òÊ|Ë¶ŒàçZœKæ+»¿/NâßĞWë;Ö®¶æıj“„5äZë	afMR¯…=‚
[ –šS²;:‰'€ãÜZäè B¥b{ıVÚwš!BV9ƒí™Áa6óêØ†!»–&¾GTp^Á
wü££$rÔÚ²¾	Ôò³ÏÂ}ô&pRBLÀKëAâÜ¢‚!n,
*9Ìå¿=ˆóÌAşÛ¸6²ú@rˆÊmÍÚş-‡NĞ[t>”­MÀ¹AÏ k#9÷ Ş„	š#•ä…dÉÌ–­m2ÅªI±!~Ër¤èM›˜ ×,¹zlNıì2  Z˜Ï¸Q§	·§—@#m±;îE0àf	&gàÏ×Ç{—ºKĞj;äÖîeÍˆ=ušÙ)£°MMŸî¼x0ìå,I²‹*ÓòCK²ñP«?¾‹$`TFõLrç‚º8Åï§‡@mêı$ŸtiZj=Ù·å¶t»ô3vN1)ş1–º*ò †ÿ—vKµ’šA@q;Ù—Z¸¾wL<º§#Úe[8*å7Å©·XfØ/§BøS}vTùò„›+€°?@P K*˜tàÖİˆ…xpó—‡JÂ$¥5hPõ­„\9ÅNÆM]0s}ê–µÔŒñÜ_( N7¡¸~JÏXè_G«g½i¾ÉŸÆåÅæJr-Y›¼'áK×ÎšÃ3…z©OÕX"d,:Ïmtµ«8%‘µÎ&ÀÛÇäå[¡è´ßqKrşNà½ó“†am*Á<V¨|:y”«¥Nf` İ˜ìg­-ßá×‹‹a÷J+ù'Öu3œ¿õãT«b_Cãói~óŒ*èà³è—Ün˜dÙQØ°‰üÜei]AR%½†°+yO&nÉx–G»Ç±BkM{‚}ööõ¯R°]÷ÌÎÊ8Z—g¦RF<5£ÀlÜ¢çDg–ªPÀ¹WÇ‚7‰øz_7Åß»¥¼¦ªè%]AÎ\ªÛë§Æ˜ô²2înÕ›šÛdúê.*Â›×µŞèÒqUFµÜ\[\ƒª¹ÚUäi¨¡Ã·m-ßåKœ“ëf¢K1Sí½œ}PAEYÔì Æä–=!?3Wø ¢ˆÂŸ5í†r·]·J¥Í÷JÖŒl	KáïìàÇICÁ&ÿÎT86İÒÍTÀ'vúÏ»1²rıQOÄø@Õˆ¡êI”›sö4ğuÂlHå5:}Â—Ù]RYŞ3>Q’%ß½Ê·gS~dé§y>»¦±ßM~ñiÔÜš¦ûS+€\¨s8|aäj»üG–Óÿ{s—ŞšÉNmDN¢Ö——6ÑË
©lÌ®0èşëø±Ã%~¹ºmV†Î€æ	‰ªRª€ãäĞq^ü­RÍx†î»¡øNºmm*söcgÀ´Ô“Î™7d¸¹ğ!hµ1+ËIçöŸ? gp›«UÌ$4xØ*èX lî ãJ(t»¯P§ÖIx3…Æè¥úšÇiÜ½¡3®x%áXZQµÃØ*Ş ‰{N²vÁŸ6s’~†A±ËÍv•: ´o}¼n¾— X¹æ¥Ù«²%vÚ²I"s“ş •ªÁÅ´×jóäå’NGF¸1­Šh–‰	i{Ş¨ûÑ}ğ{sWT
WÍ"„HÉ=	êJfÊh`*bÈ¸Å‰q†ÇËâæ¢¦5î‡‹ËÅ—üj?îñÈ°dBÿùéà&­ÎŸúRá˜\¤eìÀ$P^Ö1òGØÌ&uo*ğßØúOcZ¦=é8S‡D‚S´mÓ>_VêSl-Îªå—nH!Pbœ%wn´§(Û>+¡GùÒ.’ªÃ¤'= ²šu¯ÃĞ¡æ^&ô…¦öT,DL´l+Û–‘ô)"ë”Œœ‰a‚\ß„Ña%ã±‰ŸKAôt6œF•èE~XØåq~haíÇ÷|î²kƒnëaƒ¦´-Ü"£Ihü{üGn)›µÇ@ù‡îüO¤»¬»9Ÿh,;pšm*âé-of®u»¦³İ*]®ÅW´/"ëÑf¼ãé¿Ç)+Ã¨x^+ôéì¥ÿ0‡…f¼	—$"*PÂí8'	æ°¼Õ+U·ªÒ^-“}F6s¹{–dÚ\ûÌ¸¿=†b71ÇwCqô‘!9íÖ ã¡üdz¢v7kQ¤7RÖR¤Á™?á^è}©Æ\¤U¯½™ĞIèáñ»·˜
¶aç¨Nïi€¤3fzõR*¡¼ˆ'´Uôá›ŞhşÊwJÚ%rDyÀÔC+Ô½{÷<¼¸2UÀO5å¤®‚ßú}B˜TŒæ.rM»ËJFı_+c(‘P5…ªFÜ‰“I}Œbõg4æ„mÄ÷ñ¬tLÒ¹F
3 ¨»Õ{>‡qè.½Ó,z´2.hpÕë¯×<.vî3ÎÚ¦@XF¦Y*"ì<À llö"šfzÒÈ[Šázşxú1O$\8ç¦l2IB_Q¸q™êJ÷°ß³4¡N÷ÌRv~e8SzÒ)4`6¸í8fWïîÖâ¬>„;ö,Fƒ!ôÁB	:uDwT?
Š:ó‡K*sğÇïĞø±í,`ÿÃÏ“L’Ï‘‹P^= 6:²bl`1b!'ªIÔ,óSTµöøËšØX½"qÙ‘xŒt ªävÃË"r6u<*İ<ˆy†Ñ1>;Ìş+h£¼xËŒ€Õ˜Î†UÚÑ<Êj`zÌ·vêjaóO}ñN‹è”€b5ÆÆÜr´´ÇÚŠı®Ì›à›ÄIn]üºÓ¥ÙÚZTv¶…µZèDÚl¼Ò»ö yµfgğ©Ş—¶@@ö\Ú$o*É”Æ)íH {™æ 8Æ«Îº>²¢·±pĞ?ŒÙbbk”µ
‰ô%¦ÑÀİD°ıˆ‰°'ŸzFÜÏ¦ÔiT_}²ef<ŸFæ´à*& 6ó¿*ÔH3°Æ•S¼Ü>8‘aâ"_:q8N´8Éã Ü/ã*=#ëfØè‡›*£Öş3ÈŞíåç¯”ÙQ&@›ËL5X•0 h¿Œ¦+½Cê7ÿˆ½Ø*M’`işî3¡ ¨X­læ<¦fÅğìÇ-=fÚy"9ÈC¿N}ÓáEø®JŠæ¦—“à™`·µ|7¥l´¥—RŠŞsKfcB½ˆ&]RXa;Cƒ0È£.Ï`vYŒ½ƒÑÎÈ¦ç‡Ig;¥[Jı±ù©–Òi…ÒHabBà{y^¬“;Ô²U« ŞHúyw¡CÊÉ!6Fd/ÓúMÜ¬‹d°ÔˆÚ“ÄœhÌ+³~Üú3Ø#·ª78Ï)K{CFº@ù/:«›´hÿò¬†Õsh;h	=—xäk+ºîOEY™Â§càëÔbÂnòø¸¦Ê”9Şßãñ"cñ`©â½BRwN<)ù“¬Ë¸í} uÒ¥ğÜ‹4GBÑ~®D®‹'ªÄb÷æ
DœU{R£†œ„¨¦|Ş×âÒL¯ÜQ8<D²QÖp*ü:¡Õe;ÏlşÙ`¸Á§*k<’
»tXÄ››ÃAïÂV½çD7¬V3¤¾%©u&1r,‡øtYBà¯9ˆ¥à»{?Á”¾À(0Á?=•ÃöÒpF£53bóäÜØ“Uğ;³ø¬ÌœTN(®«6,ù©‰ı½:­ùû`PØ¹+0ÃÆ“‰ÛÿAßˆØŞ2D½46éö€å°A@«¡–<î¸Ğn„7»'«Ïğ°t~_hSG¼E$"°–àj—f½6Ï½çyUeØK{Xhcå5õığ!\0¨!,k[±ùÓwë=s¶®ßåô;èÅ+à»»Äô½)·1ò]d@kÄD?ú¯KSƒæßÌúR‡†ŸˆOÆ9x}py:@ğ :û4AŸH9 #b­9ƒN9JÈß-g
–3ŸóxÓİô¦¯2ıôÛš¡ÉXË*%/+:Â©–Äõ­£‘‘ZzêŸ}ö»i£4Éù0Jˆyexy+%b7çIwµÂ«$&>2æ
ÛŞO†¢qú­e~¹L¤“âR‚öı¹íƒSÑĞçØGA[ò„™o‡NX‹E>!§Œ"ıÙî”	J¥Ù~YåÔú‘>,Ö2—mƒù$»ê1	¾cÁ™,H¤o	5ö»É­À„]UtgCT Jº0-ƒÜQ¿lrË_†¿ÕP<«v‡lƒÔ=‹ŠB673O]¯èªAŠ½]9hÁ <ÉOÊÚ¡NT€ãM~‘%»
ã>‚‡oZ­ÇN0Ö%`×ó'$!{iï˜1œ¥ J<ugí½?–ltGóB4~üu™¤Zèpi¤Ó€›¯á=‚r¯Ê‹§ÁK)
e 4zIÉ×´Ù¶¨îˆÀ¼ş˜-w¥ÑÎûá¨;Ş‚>8ŞÀãµÃ'ëb«_“†ÄßÇ¶]	æK ¯¹.02ö;Ç.ŸÛÏ$ƒ*`òv†ErDÊ¦¢A€ä£{C¢Eu+æ´4-llD-xÉ"‹Œeša´¥ÙÌ‘#ç½_P%åE‡‚{Û9y£àÆ8n}I<Šb ÌòÅòğ:~†Í¡â(.?¶ö}¿€HF3ˆ“¹ë{¨ğ7…@_¾6±T÷À¿V›ü!¯8õ±ƒğb7w[ğüHtñ
éÈd
†JYÁËß³RU%•·Ñ"š/µm·K¿ò^ÑÛu¸oGªônGÆ­Ãõ…ì3m>zFº:Uy½¯gNûù+I‚Ácÿç-Ò§6wı¤¡í¢mÖòÖ