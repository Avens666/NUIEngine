//  **************************************
//  File:        KSwitchButtonView.cpp
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
#include "KSwitchButtonView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"

/////////////////// KSwitchButtonView //////////////////////////
KSwitchButtonView::KSwitchButtonView()
{
	m_b_on = true;

}
void KSwitchButtonView::shared_ptr_inited()
{
	KImgButtonView::shared_ptr_inited();
	m_clicked_pos_signal.connect(this, &KSwitchButtonView::onClick);
}
KSwitchButtonView::~KSwitchButtonView()
{
}
void KSwitchButtonView::setResourse(IRESurface* on, IRESurface* off, kn_bool bON)
{
	KImageDrawable_PTR onDrawable = KImageDrawable_PTR(new KImageDrawable(on, true));
	KImageDrawable_PTR offDrawable = KImageDrawable_PTR(new KImageDrawable(off, true));

	setResourseDrawable(onDrawable, offDrawable, bON);
}

void KSwitchButtonView::setResourse(const kn_string& strOnResPath, const kn_string& strOffResPath, kn_bool bON)
{
	KImageDrawable_PTR onDrawable = KImageDrawable_PTR(new KImageDrawable(strOnResPath));
	KImageDrawable_PTR offDrawable = KImageDrawable_PTR(new KImageDrawable(strOffResPath));

	setResourseDrawable(onDrawable, offDrawable, bON);

}
void KSwitchButtonView::setResourseDrawable( KDrawable_PTR onDrawable, KDrawable_PTR offDrawable,  kn_bool bON)
{
	m_on_drawable = onDrawable;
	m_off_drawable = offDrawable;
	
	addDrawable(m_on_drawable);
	addDrawable(m_off_drawable);
	setStatus(bON);
}
void KSwitchButtonView::showSwitchStatus()
{
	if (m_b_on)
	{
		m_on_drawable->SetShow(TRUE);
		m_off_drawable->SetShow(FALSE);
	}
	else
	{
		m_on_drawable->SetShow(FALSE);
		m_off_drawable->SetShow(TRUE);
	}

}
void KSwitchButtonView::onClick(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	setStatus(!m_b_on);
	InvalidateView();
	m_sign_state_changed.emit(m_b_on);
}
void KSwitchButtonView::setStatus(kn_bool b, kn_bool callback)
{
	if (b == m_b_on)
	{
		return;
	}

	m_b_on = b;
	showSwitchStatus();

	if (callback)
	{
		m_sign_state_changed.emit(m_b_on);
	}
}

BOOL KSwitchButtonView::getStatus()
{
	return m_b_on;
}
