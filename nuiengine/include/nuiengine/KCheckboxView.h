//  **************************************
//  File:        KCheckboxView.h
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
#ifndef KCheckboxView_DEFINED
#define KCheckboxView_DEFINED

#include "KButtonView.h"
#include "KViewGroup.h"
 
class NUI_API KCheckboxView : public KTextView
{
public:
	KCheckboxView();
	virtual ~KCheckboxView();
	virtual void shared_ptr_inited();
	virtual void setCheck(kn_bool b);
	kn_bool getCheck();

	virtual void onClick(kn_int x, kn_int y, KMessageMouse* pMsg);

	// 设置资源: 选中图标 、未选中图标、文字
	void setResourse(const kn_string& strClickResPath, const kn_string& strUnclickResPath, const kn_string& strMessage);
	void setResourse(IRESurface* , IRESurface* , const kn_string& strMessage);
	//void setResourseDrawable( KDrawable_PTR daChecked,  KDrawable_PTR daUnchecked, KDrawable_PTR daMessage);
	void setResourseDrawable( KDrawable_PTR daChecked,  KDrawable_PTR daUnchecked, const kn_string& strMessage);//huw
	// 选中状态改变,参数为改变后的状态
	sigslot::signal1<kn_bool> m_sign_state_changed;

	void SetCenter();
 
protected:

	void showCheck();
 
protected:

	kn_bool m_b_check;

	KDrawable_PTR m_drawable_checked;
	KDrawable_PTR m_drawable_unchecked;
	//KDrawable_PTR m_drawable_message;


};
typedef boost::shared_ptr<KCheckboxView> KCheckboxView_PTR;

class NUI_API KRadioboxView : public KCheckboxView
{
public:
	KRadioboxView() ;
	virtual ~KRadioboxView();

	virtual void onClick(kn_int x, kn_int y, KMessageMouse* pMsg);
};
typedef boost::shared_ptr<KRadioboxView> KRadioboxView_PTR;

class NUI_API KRadioboxGroup : public KViewGroup
{
public:

	KRadioboxGroup();
	virtual ~KRadioboxGroup();

	void SetGroupCheck(KRadioboxView_PTR pCheckedView);
 
	void SetSelectedIndex(kn_int index);

	kn_int GetSelectedIndex ();
	KRadioboxView_PTR GetSelectedView ();


	// 选中项改变，参数为改变后的状态
	sigslot::signal1<kn_int> m_sign_index_changed;
	sigslot::signal1<KRadioboxView_PTR> m_sign_selectedview_changed;

protected:

	kn_int m_i_selected_index;
	
	KRadioboxView_PTR m_p_selected_view; 
 
}; 

typedef boost::shared_ptr<KRadioboxGroup> KRadioboxGroup_PTR;
#endif

