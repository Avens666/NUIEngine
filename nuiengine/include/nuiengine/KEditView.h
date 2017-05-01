//  **************************************
//  File:        KEditView.h
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
#ifndef KEditView_DEFINED
#define KEditView_DEFINED

#include "KTextView.h"
#include "KTextDrawable.h"

class NUI_API EditWordAtom
{
public:
	EditWordAtom()
	{
		m_Type = 0;
		m_Length = 0;
		m_TextAtom = _T("");
	}
	kn_string m_TextAtom;  //单个字符
	int m_Length;   // 字符的像素宽
	int m_Type;		// 字符的类型
};



class NUI_API KEditView : public KTextView
{
public:

	KEditView();
	virtual ~KEditView();
	virtual void shared_ptr_inited();
	virtual kn_bool Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight);
	virtual kn_bool Create(RERect& rect);

	void Init(int b_w = 5, int b_h = 5,REColor cross_clr = RE_ColorBLACK );
	void SetText( kn_string text );
	virtual void UpdateTextDrawable();
	void ShowCross(bool bshow);
	virtual void SetCrossIndex(int index);
	/// @brief 获取游标位置值 add by junz at 2014-12-27
	kn_int GetCrossIndex();
	void SetAllSelect(bool bSel);
	/// @brief 获取当前是否全选 add by junz at 2014-12-27
	kn_bool GetAllSelect();

	int GetLinecount();

	virtual void OnDClick(kn_int iScreenX, kn_int iScreenY,KMessageMouse* pMsg);
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 

	virtual void setState( ViewState state, kn_bool bRefresh );
	virtual void  onKeydown(KMessageKey* pMsg);
	virtual void  onKeyInput(KMessageInput* pMsg);
	virtual void  onFocus();
	virtual void  onUnFocus();

	virtual void InsertChar(int index,kn_string& str );
	virtual void RemoveChar(int index);
	void RemoveAllChar();
	void Copy();
	void Paste();
	void Cut();

	// 是否显示输入法keyboard,  实现放在 messageforwindows.cpp or messageforlinux.cpp 
	void showIME(kn_bool b);
	void setReadonly(bool bReadOnly){m_bReadOnly = bReadOnly;};
	bool getReadonly(){return m_bReadOnly;};
	void setMultiLine(bool bMultiLine);
	bool getMultiLine(){return m_bMultiLine;};

	void setAlignType(int type);
	int getAlignType();
	void setTextOff(kn_int ix,kn_int iy);
	void setCrossOff(kn_int i);
	void setRowInterval(int v);
	void setFocusDrawable( KDrawable_PTR p );
    // 设置可输入文字的最大长度 add by weix 
    void setInputMaxLength(kn_int max_length);

	sigslot::signal0<> m_sign_end_input;
	sigslot::signal0<> m_sign_change_input;
protected:
	KDrawable_PTR m_Cross_drawable;
	KDrawable_PTR m_AllSelect_drawable;

	bool m_bReadOnly;  //只读标示

	std::vector<EditWordAtom> m_Text;   //字符串
	int m_Index;  // 游标的位置

	bool m_bAllSelect;

	int m_bk_border_w, m_bk_border_h;  // 边缘预留的宽度,

	bool m_bFocus;

	bool m_bMultiLine;
	int m_i_x_offset;
	int m_i_y_offset;

	int m_i_cross_offset;
	KDrawable_PTR m_focus_drawable;
    // 可输入最大字数  add by weix
    kn_int m_input_max_length;
    // 是否为backspace操作 add by weix at 2015-12-01
    kn_bool m_b_is_backspace_oper; 
    // backspace操作删除的文字长度 add by weix at 2015-12-01
    kn_int  m_backspace_char_length;

};
typedef boost::shared_ptr<KEditView> KEditView_PTR;
#endif
