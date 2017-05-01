//  **************************************
//  File:        KTextView.h
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
#ifndef KTextView_DEFINED
#define KTextView_DEFINED

#include "KView.h"
#include "KTextDrawable.h"
#include "KImageDrawable.h"
#include "KColorDrawable.h"
#include "sigslot.h"
#include "AnimationThread.h" 

class NUI_API KStaticView: public KView
{
public:
	KStaticView();
	virtual ~KStaticView();
	virtual kn_bool Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight);
    virtual kn_bool Create(const RERect& rect);
	virtual void SetRect(const RERect& rect);

	void SetText(kn_string text);
	void SetTextColor(REColor color);
	void SetFontSize(kn_int iFontSize);
	void SetFontBold(kn_bool bBold);
	//renliang added for font
	SkTypeface* GetFont();
	void SetFont(SkTypeface* pTypeFace);

	void setTextAlign(REPaint::Align align);
	void SetTextBound(RERect rect);
	void SetTextBold(kn_bool bBold);

	void setOutBoundDot();

	//Textclip是设置text drawable,注意坐标是View内的相对坐标
	void SetTextClip(RERect rect);
	RERect GetTextBound();
	RERect GetTextClip();

	void setBKGColor(REColor color);        //设置背景颜色
	//设置背景
	void setBKGImage(const kn_string& filepath);
	void setBKDrawable( KDrawable_PTR );
	void setIconImage(const kn_string& resPath);
	void setIconDrawable( KDrawable_PTR );
	void setIconPos(kn_int x, kn_int y);
	void setIconCenter();
	void setTextMargin(kn_int w, kn_int h);
	void setTextDrawable( KTextDrawable_PTR p );

	void sizeBKGToImage();

	void showBK(kn_bool b);
	void showIcon(kn_bool b);
	void showText(kn_bool b);

	KDrawable_PTR getBKDrawable();
	KDrawable_PTR getIconDrawable();
	KDrawable_PTR getTextDrawable();

	kn_string getText();

	void StartTextAnimation();

	//RERect GetTextClipByView();
	//void SetTextClipByView(RERect rect);
	//增加对图片透明度的判断
	virtual kn_bool isPointInView(int x, int y);
	void checkAlpha(kn_bool);
	void setCheckAlphaDrawable( KImageDrawable_PTR b);

	virtual void viewSizeChange();
protected:
	KDrawable_PTR		m_bk_drawable;
	KImageDrawable_PTR	m_icon_drawable;
	KTextDrawable_PTR	m_text_drawable;

protected:
	// 动画线程
	CAnimationThread m_ani_thread_txt;
	//点击区域是否判断图片的alpha通道
	kn_bool m_b_check_alpha;
	//用于检查Alpha的drawable
	KDrawable_PTR m_check_alpha_da;
};

typedef boost::shared_ptr<KStaticView> KStaticView_PTR;

class NUI_API KTextView : public KStaticView
{
public:
	enum ViewState
	{
		BS_NORMAL = 0,
		BS_ACTIVE,
		BS_FOCUS,
		BS_PRESSED,
		BS_DISABLED
	};

	KTextView();
	virtual ~KTextView();


	// Slot
	//virtual void OnMove(kn_int iScreenX, kn_int iScreenY);
	//virtual void OnDown(kn_int iScreenX, kn_int iScreenY);
	//virtual void OnUp(kn_int iScreenX, kn_int iScreenY);

	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
//	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	void onActive(); 
	void onUnActive(); 

	ViewState getState();
	virtual void setState( ViewState state ,kn_bool bRefresh=TRUE);

	void SetTextColor(REColor color);
	void SetTextColor(REColor n_color, REColor f_color, REColor p_color, REColor d_color);

	virtual void shared_ptr_inited();

	void SetTextClipEnable(kn_bool bStatus);
	kn_int GetTextWidth();
	kn_int GetFontSize();
	REPaint* GetTextPaint();
	void onFocus();

	void setCursorChangeEnable(bool);
	void setStateChangeEnable(bool);
protected:
	ViewState	m_state;

	REColor		m_text_normal_color;
	REColor		m_text_focus_color;
	REColor		m_text_pressed_color;
	REColor		m_text_disable_color;

	// 允许更换光标形状: enter 手型，leave 箭头
	kn_bool		m_b_enable_cursor_change;

	// 允许state变化
	kn_bool		m_b_enable_state_change;

};
typedef boost::shared_ptr<KTextView> KTextView_PTR;

#endif

