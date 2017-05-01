//  **************************************
//  File:        KColorDrawable.cpp
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
#include "KColorDrawable.h"


KColorDrawable::KColorDrawable(REColor color)
{
	m_type = KDrawable::KDRAWABLE_COLOR;
	m_color = color;
	m_paint.setColor(m_color);

//	m_rect.setXYWH(0, 0, 800, 480);
	
}


//KColorDrawable::~KColorDrawable(void)
//{
//}


void  KColorDrawable::setColorAlpha(kn_byte a)
{
	m_alpha = a; // ColorSetA(m_color, a);
}

void KColorDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
	// 设置绘制区域
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}
	if (m_b_clip_rect)
	{
		RERect rect = RERect::MakeXYWH(m_rect.left() + iDstX, m_rect.top() + iDstY, m_rect.width(), m_rect.height());
		pDstSurface->ClipRect(rect);
	}
	//颜色填充会忽略当前透明度设置，所以如下处理
	REColor clr = ColorSetA(m_color, ColorGetA(m_color)*m_alpha/255 );
	pDstSurface->DrawColor(clr);


	//pDstSurface->Restore();
	EndDraw(pDstSurface, pMatrix);
}

void KColorDrawable::setColor( REColor color )
{
	writeLock lock(m_drawable_draw_mutex); 
	m_color = color;
}

REColor KColorDrawable::getColor()
{
	return m_color;
}

