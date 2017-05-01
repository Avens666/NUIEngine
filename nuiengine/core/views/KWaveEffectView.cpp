//  **************************************
//  File:        KWaveEffectView.cpp
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
#include "KWaveEffectView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"
#include "KScreen.h"
/////////////////// KTipBase //////////////////////////

KWaveEffectView::KWaveEffectView()
{
	m_i_frame = 0;

	m_wave_drawable = KWaveEffectDrawable_PTR(new KWaveEffectDrawable() );
	m_wave_drawable->setRect(0, 0, 100, 100);
	addDrawable(m_wave_drawable);
}

KWaveEffectView::~KWaveEffectView()
{
}


void KWaveEffectView::init(kn_string file)
{
	m_wave_drawable->CreateFromFile(file);
	m_wave_drawable->setRect(0,0,m_wave_drawable->GetRect().width(), m_wave_drawable->GetRect().height()*2 );
}

void KWaveEffectView::init( IRESurface* pSurface, kn_bool b_out)
{
	m_wave_drawable->CreateFromSurface(pSurface, b_out);
	m_wave_drawable->setRect(0,0,m_wave_drawable->GetRect().width(), m_wave_drawable->GetRect().height()*2 );
}


kn_bool  KWaveEffectView::isChange(kn_int para_id, kn_double v)
{
	if (para_id == PropertyPARA_ID_WAVE_FRAME)
	{
		kn_int i_v = v;
		return  (m_i_frame != i_v) ;
	}
	return KView::isChange(para_id, v);
}

void  KWaveEffectView::doChange(kn_int para_id, kn_double v)
{
	KView::doChange(para_id, v);
	if (para_id == PropertyPARA_ID_WAVE_FRAME)
	{
		kn_int i_v = v;
		m_i_frame = (i_v);
		m_wave_drawable->setFrame( m_i_frame);
	}
}

kn_double  KWaveEffectView::getParaValue(kn_int para_id)
{
	if (para_id == PropertyPARA_ID_WAVE_FRAME)
	{
		return m_i_frame;
	}
	return KView::getParaValue(para_id);
}

void  KWaveEffectView::play(int speed)
{
	if (!m_mov_ani_thread.isPlaying())
	{
		m_mov_ani_thread.clearAnimation();
		m_i_frame = 0;
		m_speed = speed;
		CNUIAnimation* ani = m_mov_ani_thread.addAnimationHelper(shared_from_this(), PropertyPARA_ID_WAVE_FRAME, WAVE_EFFECT_TOTALFRAME, m_speed, 0, 0, KEasingCurve::Linear);
		ani->setLoopType(CNUIAnimation::LOOP_PINGPONG);
		ani->setLoopTimes(LOOP_ALWAYS);
		m_mov_ani_thread.Start();
	}
}

void  KWaveEffectView::stop()
{
	m_mov_ani_thread.Stop();
}
