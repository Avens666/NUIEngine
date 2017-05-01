//  **************************************
//  File:        KAreaBlurDrawable.cpp
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

///////----------------- AreaBlurDrawable ------------------
#include "SkBitmapSource.h"
#include "SkBlurImageFilter.h"
#include "SkColorFilter.h"
#include "SkColorMatrixFilter.h"
#include "SkColorFilterImageFilter.h"
#include "SkMergeImageFilter.h"
#include "SkMorphologyImageFilter.h"
#include "SkBitmapDevice.h"
#include "KAreaBlurDrawable.h"


KAreaBlurDrawable::KAreaBlurDrawable()
{
//	m_b_clip_rect = FALSE;
//	m_paint.setMaskFilter(REBlurMaskFilter::Create(25, REBlurMaskFilter::kNormal_BlurStyle, REBlurMaskFilter::kHighQuality_BlurFlag))->unref();
//	m_paint.setColor(ARGB(255,50,100,150));
	m_blur_surface = NULL;
	m_rect.setXYWH(0,0,1,1);
}

KAreaBlurDrawable::~KAreaBlurDrawable()
{
	SAFE_DELETE(m_blur_surface);
}

void KAreaBlurDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}

	int iTarX = iDstX +  m_rect.left();
	int iTarY = iDstY + m_rect.top();
	int tar_w, tar_h; //目标宽高，对于溢出的区域要裁剪
	if (iTarX < 0)
	{
		tar_w = m_rect.width() + iTarX;
		iTarX = 0;
	}
	else
	{
		tar_w = m_rect.width();
	}

	if (iTarY < 0)
	{
		tar_h = m_rect.height() + iTarY;
		iTarY = 0;
	}
	else
	{
		tar_h = m_rect.height();
	}

	m_paint.reset();
	m_paint.setAntiAlias(true);

	m_blur_surface->Clear(0x00000000);
	RERect rectSrc = RERect::MakeXYWH(iTarX, iTarY, tar_w, tar_h);
	RERect rectDst = RERect::MakeXYWH(0, 0, tar_w, tar_h);
	m_blur_surface->DrawBitmapRectToRect(pDstSurface, &rectSrc, rectDst, &m_paint);
	

	SkAutoTUnref<SkImageFilter> bitmapSource(new SkBitmapSource( *m_blur_surface->GetBitmap() ));
	SkAutoTUnref<SkImageFilter> blur(new SkBlurImageFilter(4.0f, 4.0f, bitmapSource ));
	m_paint.setImageFilter(blur);

	pDstSurface->DrawPaint(m_paint);

	EndDraw(pDstSurface, pMatrix);
}

void KAreaBlurDrawable::SetRect(const RERect& rect)
{
	if (m_rect.width() != rect.width() || m_rect.height() != rect.height() )
	{
		SAFE_DELETE(m_blur_surface);
		//m_p_data = new unsigned char[rect.width()* rect.height()*2];
		//memset(m_p_data, 0, rect.width()* rect.height()*2);
		//m_blur_surface = RESurfaceFactory::CreateRESurface(m_p_data, rect.width() , rect.height(), REBitmap::kRGB_565_Config);
		m_blur_surface = RESurfaceFactory::CreateRESurface(rect.width() , rect.height(), REBitmap::kARGB_8888_Config);
	}
	KDrawable::SetRect(rect);
}
