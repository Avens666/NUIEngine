//  **************************************
//  File:        KSwitchButtonView.h
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
#ifndef KSwitchButtonView_DEFINED
#define KSwitchButtonView_DEFINED

#include "KTextView.h"
#include "KTextDrawable.h"
#include "KImageDrawable.h"
#include "KColorDrawable.h"
#include "KButtonView.h"
#include "sigslot.h"

class NUI_API KSwitchButtonView : public KImgButtonView
{
public:
	KSwitchButtonView();
	virtual ~KSwitchButtonView();
	virtual void shared_ptr_inited();
	/**设置OFF状态、ON状态资源，以及初始状态 :
	true表示当前为ON状态，false表示当前为OFF状态
	ON状态时显示OFF状态资源表示点击后ON->OFF
	OFF状态时显示ON状态资源表示点击后OFF->ON
	*/
	void setResourse(IRESurface* on, IRESurface* off, kn_bool bON = TRUE);
	void setResourse(const kn_string& strOnResPath, const kn_string& strOffResPath, kn_bool bON = TRUE);
	void setResourseDrawable( KDrawable_PTR onDrawable, KDrawable_PTR offDrawable, kn_bool bON = TRUE);
	void showSwitchStatus();
	void onClick(kn_int x, kn_int y, KMessageMouse* pMsg);
	void setStatus(kn_bool b, kn_bool callback=FALSE); //callback 是否激活回调
	BOOL getStatus();
	sigslot::signal1<kn_bool> m_sign_state_changed;

protected:
	KDrawable_PTR m_off_drawable;
	KDrawable_PTR m_on_drawable;
	kn_bool m_b_on;
};

typedef boost::shared_ptr<KSwitchButtonView> KSwitchButtonView_PTR;
#endif

