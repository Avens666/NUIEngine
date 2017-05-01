//  **************************************
//  File:        KBitmapTextDrawable.cpp
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
#include "KBitmapTextDrawable.h"
#include "KProfile.h"
#include "FilePath.h"


KBitmapTextDrawable::KBitmapTextDrawable(const kn_char* szText, REColor color,  kn_int iFontSize, REPaint::Align align):KTextDrawable(szText, color,  iFontSize, align)
{
	m_type = KDrawable::KDRAWABLE_TEXTBITMAP;
	m_bitmap_surface = NULL;

	m_BitmapCharSetNum = 0;
	m_CharAverageWidth = 0;
	m_CharAverageHeight = 0;

	//test
	//LoadProfile(_T("BitmapTextConfig.ini"));
}

KBitmapTextDrawable::~KBitmapTextDrawable()
{
	SAFE_DELETE(m_bitmap_surface);
}

void KBitmapTextDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
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
	//REPaint::FontMetrics fm; 
	//m_paint.getFontMetrics(&fm);

	//文字总的高度
	int iTextHeight = m_CharAverageHeight;

	// 文字在baseline以上的高度
	//int iBaseline = abs((int)fm.fAscent);

	iTarY =  (m_rect.height() - iTextHeight) / 2 + iTarY;

	try
	{
		//pDstSurface->DrawText(m_strText.c_str(), m_strText.length() * sizeof(kn_char), iTarX, iTarY, m_paint);
		for(int i = 0 ; i < m_strText.length(); i++)
		{
			TCHAR str = m_strText[i];
			BITMAPCHAR* c = getBITMAPChar(str);
			if(c != NULL)
			{
				RERect rectSrc = RERect::MakeXYWH(c->x,c->y,c->w,c->h);
				RERect rectDst = RERect::MakeXYWH(iTarX,iTarY,c->w,c->h);
				pDstSurface->DrawBitmapRectToRect(m_bitmap_surface, &rectSrc, rectDst, &m_paint);
				iTarX += c->w;
			}
		}
	}
	catch (...)
	{
		iTarX = iTarX;
	}
//	pDstSurface->Restore();
	EndDraw(pDstSurface, pMatrix);
}

int KBitmapTextDrawable::getTextWidth()
{
	int Width = 0;
	for(int i = 0 ; i < m_strText.length(); i++)
	{
		TCHAR str = m_strText[i];
		BITMAPCHAR* c = getBITMAPChar(str);
		if(c != NULL)
		{
			Width += c->w;
		}
	}
	return Width;
}

kn_bool KBitmapTextDrawable::LoadProfile(const kn_string& strFile)
{
	KProfile m_profile;
	//初始化系统路径
	//TCHAR	str[MAX_PATH] = {0};
	//TCHAR	strexepath[MAX_PATH] = {0};
	TCHAR	strpath1[MAX_PATH] = {0};
	int  len = 0;


	//GetModuleFileName(NULL,str, MAX_PATH);
	//GetPath(str, strexepath);



	TCHAR strpath2[MAX_PATH] = {0};
	kn_string bitmapname;
	//_tcscpy(strpath1,strexepath);
	_tcscpy(strpath1,GetCurrentPath());
	_tcscat(strpath1,strFile.c_str());
	kn_string filename = strpath1;
	if(m_profile.Open(filename) == 0)
	{
		kn_int ulSize = 0;
		char buf[256] = {0};
		if(0 == m_profile.GetSection("Bitmap"))
		{
			if(0 == m_profile.GetString("BitmapName", buf,256))
			{
				//	 android _stprintf %S 未实现
				// _stprintf(strpath2,_T("%S"),buf);
				mbstowcs(strpath2, buf, MAX_PATH -1 );
				bitmapname += strpath2;
			}
			if(0 == m_profile.GetValue("BitmapCharSetNum", ulSize))
			{
				m_BitmapCharSetNum = ulSize;
			}
			if(0 == m_profile.GetValue("CharAverageWidth", ulSize))
			{
				m_CharAverageWidth = ulSize;
			}
			if(0 == m_profile.GetValue("CharAverageHeight", ulSize))
			{
				m_CharAverageHeight = ulSize;
			}
		}

	}
	m_bitmap_surface = RESurfaceFactory::CreateRESurface(bitmapname);


	if(0 == m_profile.GetSection("CharSet"))
	{
		kn_int ulSize = 0;
		char buf[256] = {0};
		for(int i = 0; i < m_BitmapCharSetNum; i++)
		{
			BITMAPCHAR c;

			if(0 == m_profile.GetString("Char", buf,256))
			{
	 
				//_stprintf(&c.word,_T("%S"),buf);
				c.word = (TCHAR)buf[0];
			}
			if(0 == m_profile.GetValue("X", ulSize))
			{
				c.x = ulSize;
			}
			if(0 == m_profile.GetValue("Y", ulSize))
			{
				c.y = ulSize;
			}
			if(0 == m_profile.GetValue("W", ulSize))
			{
				c.w = ulSize;
			}
			if(0 == m_profile.GetValue("H", ulSize))
			{
				c.h = ulSize;
			}
			m_chartable.push_back(c);
		}
	}

	return true;
}

BITMAPCHAR* KBitmapTextDrawable::getBITMAPChar(TCHAR c)
{
	for(int i = 0; i < m_chartable.size(); i++)
	{
		if(m_chartable[i].word == c)
		{
			return &m_chartable[i];
		}
	}
	return NULL;
}
