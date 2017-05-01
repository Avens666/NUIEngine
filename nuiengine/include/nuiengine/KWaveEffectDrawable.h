//  **************************************
//  File:        KWaveEffectDrawable.h
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
#ifndef K_WAVEEFFECT_DRAWABLE_H
#define K_WAVEEFFECT_DRAWABLE_H


#include "KImageDrawable.h"

#define WAVE_EFFECT_TOTALFRAME 64
class NUI_API KWaveEffectDrawable : public KImageDrawable
{
public :
	KWaveEffectDrawable();
	KWaveEffectDrawable(const kn_string& strFile);
	KWaveEffectDrawable(IRESurface* pSurface, kn_bool b_out = FALSE);
	virtual ~KWaveEffectDrawable();
	unsigned char* m_p_data;
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);
	void setFrame(int n);
	int getFrame();
protected:
	int m_i_frame;
	IRESurface* m_wave_surface;
};
typedef boost::shared_ptr<KWaveEffectDrawable> KWaveEffectDrawable_PTR;

#endif
