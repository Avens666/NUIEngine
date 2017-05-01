//  **************************************
//  File:        KTextMultiLineDrawable.h
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
#ifndef K_TEXTMULTILINE_DRAWABLE_H
#define K_TEXTMULTILINE_DRAWABLE_H

#include "KTextDrawable.h"
#include "DataSync.h"

#define ALIGN_TOP 0
#define ALIGN_CENTER 1
#define ALIGN_BOTTOM 2

class NUI_API KTextMultiLineDrawable : public KTextDrawable
{
public:
	KTextMultiLineDrawable( REColor color,  kn_int iFontSize, REPaint::Align align);
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);

	int GetTextLine(std::vector<kn_string>& vStr);

	virtual void SetText(const kn_char* szText);
	virtual void SetText(const kn_string& szText);

	void setAlignType(int type){m_aligntype = type;};
	int getAlignType(){return m_aligntype;};

	//根据文字尺寸自动增加换行 简单处理
	void autoMLine();

	//取得换行排版后的宽度和高度
//	void getTextWH(int &w, int& h);

	//取得文字行数
	int getLine();
	int getMaxWidth();
	void setRowInterval(int);
	void setMaxLine(int);
	bool isOverroad(); //是否超出了最大行
protected:
	//用户绘制的string，避免每次绘制都转换
    boost::shared_mutex    m_draw_string_mutex;
	std::vector<kn_string> m_v_draw_string;
	int m_aligntype;

	//行间距，字体高度额外的间距
	int m_txt_row_interval;
	//文本行数
	//int m_txt_line;
	int m_multi_line_w;
	int m_max_line;
	kn_bool m_b_overroad;	//是否超出了最大行
};

typedef boost::shared_ptr<KTextMultiLineDrawable> KTextMultiLineDrawable_PTR;

#endif
