//  **************************************
//  File:        KCheckboxView.cpp
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
#include "KCheckboxView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"
#include "KFontManager.h"


KCheckboxView::KCheckboxView() 
{
	m_e_viewtype = KVIEW_CHECKBOX;
	m_b_check = false;

}
void KCheckboxView::shared_ptr_inited()
{
	KTextView::shared_ptr_inited();
	m_clicked_pos_signal.connect(this, &KCheckboxView::onClick);
}

void KCheckboxView::setResourse(IRESurface* check, IRESurface* unckect, const kn_string& strMessage)
{
	KImageDrawable_PTR daChecked = KImageDrawable_PTR(new KImageDrawable(check, true));
	KImageDrawable_PTR daUnchecked = KImageDrawable_PTR(new KImageDrawable(unckect, true));

	setResourseDrawable(daChecked, daUnchecked, strMessage);
}

void KCheckboxView::setResourse(const kn_string& strClickResPath, const kn_string& strUnclickResPath, const kn_string& strMessage)
{
	KImageDrawable_PTR daChecked = KImageDrawable_PTR(new KImageDrawable(strClickResPath));
	KImageDrawable_PTR daUnchecked = KImageDrawable_PTR(new KImageDrawable(strUnclickResPath));

	// 取后半段
	//RERect rectMessage = RERect::MakeXYWH(m_rect.width() / 4, 0, m_rect.width() * 3 /4 , m_rect.height());
	//KTextDrawable* pTextDrable =  new KTextDrawable(strMessage.c_str(), RE_ColorBLACK, 30, REPaint::kCenter_Align);
	//pTextDrable->SetRect(rectMessage);	
	//pTextDrable->setFont(GetFontManagerSingleton()->GetFontFromName("Calibri"));
	//KDrawable_PTR daMessage = KTextDrawable_PTR(pTextDrable);
	//setResourseDrawable(daChecked, daUnchecked, daMessage);

	setResourseDrawable(daChecked, daUnchecked, strMessage);
	 
}
//void KCheckboxView::setResourseDrawable( KDrawable_PTR daChecked,  KDrawable_PTR daUnchecked, KDrawable_PTR daMessage)
void KCheckboxView::setResourseDrawable( KDrawable_PTR daChecked,  KDrawable_PTR daUnchecked, const kn_string& strMessage)
{
	//setBKDrawable(daChecked);
	//setIconDrawable(daUnchecked);
	m_drawable_checked = daChecked;
	m_drawable_unchecked = daUnchecked;

	addDrawable(m_drawable_checked);
	addDrawable(m_drawable_unchecked);
	int x = m_drawable_checked->GetRect().width()+m_drawable_checked->GetRect().x();
	RERect rectMessage = RERect::MakeXYWH(x, 0, m_rect.width()- x, m_rect.height());
	SetTextBound(rectMessage);
	SetText(strMessage.c_str());
	SetFont(GetFontManagerSingleton()->GetFontFromName("Microsoft YaHei"));
	showCheck();

}
void KCheckboxView::showCheck()
{
	if (m_b_check)
	{
		//showIcon(false);
		//showBK(true);
		m_drawable_checked->SetShow(TRUE);
		m_drawable_unchecked->SetShow(FALSE);
	}
	else
	{
		//showIcon(true);
		//showBK(false);
		m_drawable_checked->SetShow(FALSE);
		m_drawable_unchecked->SetShow(TRUE);
	}

}


KCheckboxView::~KCheckboxView()
{

}

void KCheckboxView::onClick(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	setCheck(!m_b_check);
}


void KCheckboxView::setCheck(kn_bool b)
{
	if(m_b_check != b)
	{
		m_b_check = b;
		showCheck();
		InvalidateView();

		m_sign_state_changed.emit(b);
	}
	
}

kn_bool KCheckboxView::getCheck()
{
	return m_b_check;
}



//////////////////////////////////////////////////////////////////////////
// KRadioboxView
KRadioboxView::KRadioboxView()
{
	m_e_viewtype = KVIEW_RADIOBOX;
 
 
}

KRadioboxView::~KRadioboxView()
{

}

void KRadioboxView::onClick(kn_int x, kn_int y, KMessageMouse* pMsg)
{
	if(!m_b_check)
	{

		ASSERT((m_p_parent.lock())->getType() == KVIEW_RADIOBOXGROUP);

		// 由KRadioboxGroup 处理
		(VIEW_PTR(KRadioboxGroup)(m_p_parent.lock()) )->SetGroupCheck( VIEW_PTR(KRadioboxView)(shared_from_this()) );
	}


}


//////////////////////////////////////////////////////////////////////////
// KRadioGroup
KRadioboxGroup::KRadioboxGroup()
{
	m_e_viewtype = KVIEW_RADIOBOXGROUP;
	m_i_selected_index = -1;
}

KRadioboxGroup::~KRadioboxGroup()
{

}

void KRadioboxGroup::SetGroupCheck(KRadioboxView_PTR pRadioboxView)
{
	if(pRadioboxView == NULL || pRadioboxView == m_p_selected_view)
	{
		return;
	}

	writeLock lock(m_lst_view_mutex);
  
	if(find(m_lst_view.begin(), m_lst_view.end(), pRadioboxView) == m_lst_view.end())
	{
		// 不在列表中
		MyTraceA("SetGroupCheck pRadioboxView is not exist!");
		return;
	}

 	// 设置group中选中项,其他项都不选中
	for(UINT i = 0; i < m_lst_view.size(); i++)
	{
		( VIEW_PTR( KRadioboxView)(m_lst_view[i]))->setCheck(m_lst_view[i] == pRadioboxView);

		if(m_lst_view[i] == pRadioboxView)
		{
			m_i_selected_index = i;
			m_p_selected_view = pRadioboxView;
		}
	}

	// 先释放,避免死锁
	lock.unlock();

	// 选中的view放在最上面显示
	// changeViewLayerTop(m_p_selected_view);


 
	m_sign_index_changed.emit(m_i_selected_index);
	m_sign_selectedview_changed.emit(m_p_selected_view);
}

// 设置选中的索引
void KRadioboxGroup::SetSelectedIndex(kn_int index)
{

	writeLock lock(m_lst_view_mutex);

	if(index >= m_lst_view.size() || index < 0)
	{
		MyTraceA("SetSelectedIndex err index = %d", index);
		return;
	}

	if(index == m_i_selected_index)
	{
		// 无需改变
		return;
	}


	for(int i = 0; i < m_lst_view.size(); i++)
	{
		( VIEW_PTR(KRadioboxView)(m_lst_view[i]) )->setCheck(i == index);
	}

	m_i_selected_index = index;
	m_p_selected_view = VIEW_PTR(KRadioboxView)(m_lst_view[index]);


	lock.unlock();

	//changeViewLayerTop(m_p_selected_view);


	m_sign_index_changed.emit(index);
	m_sign_selectedview_changed.emit(m_p_selected_view);

}



kn_int KRadioboxGroup::GetSelectedIndex ()
{
	return m_i_selected_index;
}

KRadioboxView_PTR KRadioboxGroup::GetSelectedView ()
{
	return m_p_selected_view;
}

void KCheckboxView::SetCenter()
{
	RERect rect = m_drawable_checked->GetRect();
	if (m_drawable_checked)
	{
		m_drawable_checked->SetRect(RERect::MakeXYWH((m_rect.width() / 4 - rect.width()) / 2,m_rect.centerY() - rect.height()/2
			,rect.width(),rect.height()));
	}
	if (m_drawable_unchecked)
	{
		m_drawable_unchecked->SetRect(RERect::MakeXYWH((m_rect.width() / 4 - rect.width()) / 2,m_rect.centerY() - rect.height()/2
			,rect.width(),rect.height()));
	}
	RERect rectMessage = RERect::MakeXYWH(m_rect.width() / 4, 0, m_rect.width() * 3 /4 , m_rect.height());
	SetTextBound(rectMessage);
    //setTextAlign(REPaint::Align::kCenter_Align);
    setTextAlign(REPaint::kCenter_Align);
}
