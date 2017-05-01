//  **************************************
//  File:        KWaveEffectDrawable.cpp
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
#include "KWaveEffectDrawable.h"

KWaveEffectDrawable::KWaveEffectDrawable()
{
//	m_b_clip_rect = FALSE;
//	m_paint.setMaskFilter(REBlurMaskFilter::Create(25, REBlurMaskFilter::kNormal_BlurStyle, REBlurMaskFilter::kHighQuality_BlurFlag))->unref();
//	m_paint.setColor(ARGB(255,50,100,150));
	m_i_frame = 0;
	m_rect.setXYWH(0,0,1,1);
}

KWaveEffectDrawable::KWaveEffectDrawable(const kn_string& strFile):KImageDrawable(strFile)
{
	if(m_p_surface ) 
	{
		setRect(0,0, m_p_surface->GetWidth(), m_p_surface->GetHeight()*2);
	}
}
KWaveEffectDrawable::KWaveEffectDrawable(IRESurface* pSurface, kn_bool b_out):KImageDrawable(pSurface, b_out)
{
	if(m_p_surface) 
	{
		setRect(0,0, m_p_surface->GetWidth(), m_p_surface->GetHeight()*2);
	}
}

KWaveEffectDrawable::~KWaveEffectDrawable()
{
	
}

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

void KWaveEffectDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}

	if(m_p_surface == NULL) 
	{
		EndDraw(pDstSurface, pMatrix);
		return ;
	}
	int iTarX = iDstX + m_rect.left();
	int iTarY = iDstY + m_rect.top();
	RERect rectDst = RERect::MakeXYWH(iTarX, iTarY, m_rect.width(), m_rect.height());
	RERect rectSrc = RERect::MakeXYWH(0, 0, m_p_surface->GetWidth(), m_p_surface->GetHeight());

	pDstSurface->DrawBitmap(m_p_surface, iTarX, iTarY, &m_paint);

// draw wave
	int iWidthSrc = m_p_surface->GetWidth();
	int iHeightSrc = m_p_surface->GetHeight();

	int iHeightDest = pDstSurface->GetHeight();

	int iNumFrames = WAVE_EFFECT_TOTALFRAME;
	int iWaveHeight = 100;
	int iRippleGranularity = 40;


	int iTargetLine = 0;
	int iSourceLine = 0;
	double dRipples = 0; 
	double dFrameAsRadians = 0;

	dRipples = iHeightSrc / iRippleGranularity; 

	int iFrame = m_i_frame % iNumFrames;
	dFrameAsRadians = 2 * PI * (double)iFrame / (double)iNumFrames;

	 for (int y = 0; y < iHeightSrc; y++)
	{

		int iDisplacement = (int)(dRipples * 5 / 3 						
			* sin((double)((dRipples) * (iHeightSrc - y) * 8 / (double)(y + 1) / 2 )  + dFrameAsRadians) 
			* (y + iWaveHeight) * 3 / (double) iHeightSrc / 2) ;

		if (y < iHeightSrc/10) 
		{
			iDisplacement = iDisplacement *2 / 5;
		}

		if (y < iHeightSrc/5) 
		{
			iDisplacement = iDisplacement*3/5;
		}

		iSourceLine = (y < iDisplacement) ? y : y - iDisplacement;
		iSourceLine = max(0, min( iSourceLine, iHeightSrc - 1));

		

		if ((y < 1) || (iHeightSrc <  y ))
		{
			continue;
		}

		//iTargetLine = iTarY + y ;
		RERect draw_rectDst = RERect::MakeXYWH(iTarX, iTarY + y + iHeightSrc-2, iWidthSrc, 1);
		RERect draw_rectSrc = RERect::MakeXYWH(0, iHeightSrc - iSourceLine, iWidthSrc, 1);

		pDstSurface->DrawBitmapRectToRect(m_p_surface, &draw_rectSrc, draw_rectDst, &m_paint);

	}

	//m_i_frame++;
	EndDraw(pDstSurface, pMatrix);
}

void KWaveEffectDrawable::setFrame(int n)
{
	writeLock lock(m_drawable_draw_mutex); 
	m_i_frame = n;
}

int KWaveEffectDrawable::getFrame()
{
	return m_i_frame ;
}
