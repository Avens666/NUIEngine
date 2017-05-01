//  **************************************
//  File:        KColorTextDrawable.cpp
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
#include "KColorTextDrawable.h"


KFocusColorTextDrawable::KFocusColorTextDrawable(const kn_char* szText, REColor color,  kn_int iFontSize, REPaint::Align align):KTextDrawable(szText,  color ,  iFontSize, align)
{
	m_focus_color = RE_ColorRED;
}

void KFocusColorTextDrawable::SetFocusText(const kn_string &szText)
{
	m_focus_text = szText;
}

void KFocusColorTextDrawable::setFocusColor(REColor color)
{
	m_focus_color = color;
}

void KFocusColorTextDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}

	int iTarX = m_rect.left() + iDstX;
	int iTarY = m_rect.top() + iDstY;

	REPaint::Align align = m_paint.getTextAlign();
	if(align == REPaint::kCenter_Align )
	{
		iTarX += m_rect.width() / 2;
	}
	else if(align == REPaint::kRight_Align)
	{
		iTarX += m_rect.width();
	}

	// 设置文字垂直居中
	// 获取文字高度, 文字从baseline 开始画起,不是从最底部画起
	REPaint::FontMetrics fm; 
	m_paint.getFontMetrics(&fm);

	//文字总的高度
	int iTextHeight = ceil (fm.fDescent - fm.fAscent);

	// 文字在baseline以上的高度
	int iBaseline = abs((int)fm.fAscent);

	iTarY =  (m_rect.height() - iTextHeight) / 2 + iBaseline + iTarY;

	try
	{
		if ( m_focus_text.empty())
		{//小外框
			pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY, m_paint);
		}
		else
		{
			std::size_t find_pos = m_strText.find(m_focus_text);
			if (find_pos == string::npos)
			{// not find
				pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY, m_paint);
			}
			else
			{
				//分三部分绘制
				//第一部分
				int x_step = 0;
				if (find_pos > 0)
				{
					pDstSurface->DrawText(m_strText.c_str(), find_pos* sizeof(kn_char), iTarX, iTarY, m_paint);
					x_step = m_paint.measureText(m_strText.c_str(), find_pos* sizeof(kn_char));
				}
				//第二部分 高亮字
				REColor clr = m_paint.getColor();
				m_paint.setColor(m_focus_color); //设为高亮颜色
				pDstSurface->DrawText(m_focus_text.c_str(), m_focus_text.length()* sizeof(kn_char), iTarX + x_step, iTarY, m_paint);
				x_step += m_paint.measureText(m_focus_text.c_str(),  m_focus_text.length() * sizeof(kn_char));
				m_paint.setColor(clr); //还原颜色

				//第三部分
				kn_string str = m_strText.substr(find_pos+ m_focus_text.size());
				pDstSurface->DrawText(str.c_str(), str.length()* sizeof(kn_char), iTarX + x_step, iTarY, m_paint);
			}
		}
	}
	catch (...)
	{
		iTarX = iTarX;
	}

	EndDraw(pDstSurface, pMatrix);
 
}


REColor KFocusColorTextDrawable::getFocusColor()
{
	return m_focus_color;
}

const kn_string& KFocusColorTextDrawable::GetFocusText()
{
	return m_focus_text;
}
