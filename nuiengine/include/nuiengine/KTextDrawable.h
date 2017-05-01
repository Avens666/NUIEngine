//  **************************************
//  File:        KTextDrawable.h
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
#ifndef K_TEXT_DRAWABLE_H
#define K_TEXT_DRAWABLE_H

#include "KDrawable.h"


class NUI_API KTextDrawable : public KDrawable
{
protected:
	kn_string m_strText;
	bool m_b_frame;

public:
	KTextDrawable(const kn_char* szText, REColor color = RE_ColorBLACK,  kn_int iFontSize = 20, REPaint::Align align = REPaint::kLeft_Align);

	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);

	virtual void SetText(const kn_char* szText);
	virtual void SetText(const kn_string &szText);
	const kn_string& GetText();
	void setColor(REColor color);
	REColor getColor();
	void SetFontSize(kn_int iFontSize);
	kn_int GetFontSize();
	void setFont( RETypeface* font );
	RETypeface* GetFont() const;

	const char* GetFontName() const;
	void setTextAlign(REPaint::Align align);
	REPaint::Align getTextAlign();

	virtual int getTextWidth();

	// 设置粗体
	void setBold(kn_bool b);
	kn_bool isBold();

	void setTextFrame(kn_bool b);
	kn_bool isTextFrame();

	// 设置超长时添加省略号
	void setOutBoundDot();
};

typedef boost::shared_ptr<KTextDrawable> KTextDrawable_PTR;

#endif
