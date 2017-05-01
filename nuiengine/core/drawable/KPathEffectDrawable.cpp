//  **************************************
//  File:        KPathEffectDrawable.cpp
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
#include "KPathEffectDrawable.h"
#include "Sk1DPathEffect.h"
#include "Sk2DPathEffect.h"
#include "SkCornerPathEffect.h"
#include "SkDashPathEffect.h"
#include "SkDiscretePathEffect.h"
#include "Rasterizer.h"
#include "MaskFilter.h"

int gXY[] = {
4, 0, 0, -4, 8, -4, 12, 0, 8, 4, 0, 4
};

static void compose_pe(SkPaint* paint) {
    SkPathEffect* pe = paint->getPathEffect();
    SkPathEffect* corner = new SkCornerPathEffect(25);
    SkPathEffect* compose;
    if (pe) {
        compose = new SkComposePathEffect(pe, corner);
        corner->unref();
    } else {
        compose = corner;
    }
    paint->setPathEffect(compose)->unref();
}

static void scale(SkPath* path, SkScalar scale) {
    SkMatrix m;
    m.setScale(scale, scale);
    path->transform(m);
}

static void one_d_pe(REPaint* paint, int* p_xy, int count) {
    REPath  path;
    path.moveTo(SkIntToScalar(p_xy[0]), SkIntToScalar(p_xy[1]));
    for (unsigned i = 2; i < count; i += 2)
        path.lineTo(SkIntToScalar(p_xy[i]), SkIntToScalar(p_xy[i+1]));
    path.close();
    path.offset(SkIntToScalar(-6), 0);
    //scale(&path, 1.5f);

    paint->setPathEffect(new SkPath1DPathEffect(path, SkIntToScalar(30), 0,
                                SkPath1DPathEffect::kRotate_Style))->unref();
    compose_pe(paint);
}


KPathEffectDrawable::KPathEffectDrawable()
{
		m_paint.setAntiAlias(true);
		m_paint.setStyle(SkPaint::kStroke_Style);

		one_d_pe(&m_paint, gXY, SK_ARRAY_COUNT(gXY) );

		m_paint.setColor(RE_ColorWHITE);

	m_paint.setStyle(SkPaint::kStroke_Style);
	m_paint.setStrokeWidth(3);
		REPath path;
		int x=0;
		int y =200;
		path.moveTo(x, y);

		for (int i = 0; i < 30; i++)
		{
			path.lineTo(x+i*30, y+ sin(i*5.0/3.14)*(50+i*5));
		}
		
		//path.lineTo(70, 320);
		//path.lineTo(100, 322);
		//path.lineTo(150, 328);
		//path.lineTo(180, 325);
		//path.lineTo(220, 300);
		//path.lineTo(290, 290);
		//path.lineTo(320, 150);
		//path.lineTo(370, 350);
		//path.lineTo(440, 210);	
		m_p_shape = new KPathShape(path);

	REPaint p;
	RELayerRasterizer*  rast = new RELayerRasterizer;

	p.setAntiAlias(true);
	
	p.setStyle(SkPaint::kStroke_Style);
	p.setStrokeWidth(3);
	rast->addLayer(p);

	p.setStyle(SkPaint::kFill_Style);
	p.setColor(ARGB(255,0,0,100));
	p.setXfermodeMode(SkXfermode::kDst_Mode);
	rast->addLayer(p);

	m_paint.setRasterizer(rast)->unref();

}

KPathEffectDrawable::~KPathEffectDrawable()
{
}
