//  **************************************
//  File:        KTextDrawable.cpp
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
#include "KTextDrawable.h"



KTextDrawable::KTextDrawable(const kn_char* szText, REColor color,  kn_int iFontSize, REPaint::Align align)
{
	m_type = KDrawable::KDRAWABLE_TEXT;
	SetText(szText);
	setColor(color);
	SetFontSize(iFontSize);
	setTextAlign(align);
	

	//m_paint.setTextEncoding(REPaint::kUTF16_TextEncoding);
//	SetPaintEncodingAuto(m_paint);
	
	m_paint.setAntiAlias(true);
	m_paint.setLCDRenderText(true);

	m_b_frame = FALSE;
	//m_paint.setHinting(SkPaint::kFull_Hinting);

}

kn_int KTextDrawable::getTextWidth()
{
	kn_int iSize = 0;

	if (!m_strText.empty())
	{
		iSize = m_paint.measureText(m_strText.c_str(),m_strText.size() * sizeof(kn_char));
	}
	return iSize;
	
}

void KTextDrawable::setBold(kn_bool b)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_paint.setFakeBoldText(b);
}

kn_bool KTextDrawable::isBold()
{
	return m_paint.isFakeBoldText();
}

void KTextDrawable::SetText(const kn_char* szText)
{
	if (szText)
	{
		m_strText = szText;
	}

}

void KTextDrawable::SetText(const kn_string &szText)
{
	SetText(szText.c_str() );
}

void KTextDrawable::setColor(REColor color)
{
	m_paint.setColor(color);
}

void KTextDrawable::SetFontSize(kn_int iFontSize)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_paint.setTextSize(iFontSize);
}

void KTextDrawable::setTextAlign(REPaint::Align align)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_paint.setTextAlign(align);
}


void KTextDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
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
	int iBaseline = ceil (fabs(fm.fAscent));
#ifndef WIN32
	if(iBaseline > 1)
	{
		iBaseline -= 1;
	}
	
#endif

	iTarY =  (m_rect.height() - iTextHeight) / 2 + iBaseline + iTarY;

	try
	{
		if ( isTextFrame())
		{//小外框
			REColor clr = m_paint.getColor();
			//暂时外框色取补色
			REColor frameclr = ColorSetARGB(ColorGetA(clr), 255 - ColorGetR(clr), 255 - ColorGetG(clr),255 - ColorGetB(clr) );
			m_paint.setColor(frameclr);
			pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX-1, iTarY, m_paint);
			pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX+1, iTarY, m_paint);
			pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY-1, m_paint);
			pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY+1, m_paint);
			m_paint.setColor(clr);
		}
	  pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY, m_paint);
	}
	catch (...)
	{
		iTarX = iTarX;
	}
	EndDraw(pDstSurface, pMatrix);
 
}

void KTextDrawable::setTextFrame(kn_bool b)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_b_frame = b;
}

kn_bool KTextDrawable::isTextFrame()
{
	return m_b_frame;
}

REColor KTextDrawable::getColor()
{
	return m_paint.getColor();
}

const kn_string& KTextDrawable::GetText()
{
	return m_strText;
}

kn_int KTextDrawable::GetFontSize()
{
	return m_paint.getTextSize();
}

REPaint::Align KTextDrawable::getTextAlign()
{
	return m_paint.getTextAlign();
}


// 设置超长时添加省略号
void KTextDrawable::setOutBoundDot()
{
	writeLock lock(m_drawable_draw_mutex); 
	REScalar len = m_paint.measureText(m_strText.c_str(), m_strText.length() * sizeof(kn_char));
	if(len > m_rect.width())
	{
		// 需要截断
		kn_char szDot[] = _T("...");
		kn_int LEN_MAX = m_strText.length() + 4;
		kn_char* pszTemp = new kn_char [LEN_MAX];
		memset(pszTemp, 0, LEN_MAX * sizeof(kn_char));

		kn_int i = 0;
		for(i = 0; i < m_strText.length(); i++)
		{
			pszTemp[i] = m_strText[i];
			pszTemp[i + 1] = szDot[0];
			pszTemp[i + 2] = szDot[1];
			pszTemp[i + 3] = szDot[2];

			REScalar lentemp = m_paint.measureText(pszTemp, (i+4) * sizeof(kn_char));

			if(lentemp > m_rect.width())
			{

				break;
			}
		}

		SAFE_DELETE_GROUP(pszTemp);
		 
		m_strText = m_strText.substr(0, i);
		m_strText += szDot;


	}

}

void KTextDrawable::setFont( RETypeface* font )
{
	writeLock lock(m_drawable_draw_mutex); 
	m_paint.setTypeface(font);
}

RETypeface* KTextDrawable::GetFont() const
{
	return m_paint.getTypeface();
}
