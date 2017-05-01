//  **************************************
//  File:        KButtonView.cpp
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
#include "KButtonView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"

/////////////////// KImgButtonView //////////////////////////
KImgButtonView::KImgButtonView()
{
	m_e_viewtype = KVIEW_BUTTON;

	m_b_check_alpha = FALSE;


}

KImgButtonView::~KImgButtonView()
{
}

void KImgButtonView::setState( ViewState state, kn_bool bRefresh )
{
	//调用基类函数设置文本状态，传入False不激活刷新
	if (m_state != state)
	{
		//m_state = state;

		writeLock lock(m_lst_drawable_mutex);
		switch(state)
		{
		case BS_FOCUS:
			m_lst_drawable[0] = m_focus_bk_drawable;
			break;
		case BS_NORMAL:
			m_lst_drawable[0] = m_bk_drawable;
			break;
		case BS_PRESSED:
		case BS_ACTIVE:
			m_lst_drawable[0] = m_selected_bk_drawable;
			break;
		case BS_DISABLED:
			m_lst_drawable[0] = m_disable_bk_drawable;
			break;
		default:
			break;
		}
		lock.unlock();

		KTextView::setState(state, FALSE);

		if (bRefresh)
		{
			InvalidateView();
		}

	}
}

void  KImgButtonView::setBKGImage(const kn_string& normalResPath, 
	const kn_string& focusResPath, 
	const kn_string& selectedResPath, 
	const kn_string& disabledResPath)
{
	KDrawable_PTR normal = KImageDrawable_PTR(new KImageDrawable(normalResPath));
	KDrawable_PTR focus = KImageDrawable_PTR(new KImageDrawable(focusResPath));
	KDrawable_PTR selected = KImageDrawable_PTR(new KImageDrawable(selectedResPath));
	KDrawable_PTR disable = KImageDrawable_PTR(new KImageDrawable(disabledResPath));

	setBKG(normal,focus,selected,disable);
}

void  KImgButtonView::setBKG( KDrawable_PTR normal, KDrawable_PTR focus, KDrawable_PTR selected, KDrawable_PTR disabled)
{
	writeLock lock(m_lst_drawable_mutex);
	m_bk_drawable = normal;
	m_focus_bk_drawable = focus;
	m_selected_bk_drawable = selected;
	m_disable_bk_drawable = disabled;
	m_lst_drawable[0] = m_bk_drawable;
	//不要忘了释放锁
	lock.unlock();
	showBK(TRUE);
}

