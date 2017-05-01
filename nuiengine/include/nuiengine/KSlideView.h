//  **************************************
//  File:        KSlideView.h
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
#ifndef K_SLIDE_VIEW_DEFINED
#define K_SLIDE_VIEW_DEFINED

#include "KButtonView.h"
#include "KViewGroup.h"
#include "KTextView.h"
#include "KProcessView.h"

class NUI_API KSlideView : public KTextView
{
public:
	KSlideView() ;
	virtual ~KSlideView();

	//spacing1 滑动条移动到两端的间隔
	void init(PROCESS_DIR e = PROCESS_Horizontal, int spacing1 = 0, int spacing2 = 0);
	void setMaxMin( int max, int min);
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	//b_call_back是否调用值改变回调
	void setValue(float iPos, kn_bool b_call_back = FALSE);    //设定进度的起始位置

	//设定滑块的显示比例
	void setSlideBarSize(int iPos, kn_bool b_call_back = FALSE);    
	int getValue();
	int getValueArea();

	sigslot::signal1<int> m_silder_value_change_sign;

public:
	kn_int m_value;           //当前进度 0~100
	kn_int m_max; 
	kn_int m_min; 
	PROCESS_DIR m_e_dir; //进度条方向 

	kn_bool m_drag_flag;
	int m_spacing1;
	int m_spacing2;

	REPoint m_down_point;
	int m_old_pos;
};

typedef boost::shared_ptr<KSlideView> KSlideView_PTR;

#endif // KPROCESS_VIEW_DEFINED

