//  **************************************
//  File:        REEffectHelper.cpp
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
#include "REEffectHelper.h"
#include "Rasterizer.h"
#include "PathEffect.h"

void REEffectHelper::AddStrokeAndFilledEffect( REPaint& paint, REScalar strokeWidth, REScalar filledAlpha )
{
	REPaint p;
	RELayerRasterizer*  rast = new RELayerRasterizer;

	p.setAntiAlias(true);
	
	p.setStyle(SkPaint::kStroke_Style);
	p.setStrokeWidth(strokeWidth);
	rast->addLayer(p);

	p.setAlpha(filledAlpha);
	p.setStyle(SkPaint::kFill_Style);
	p.setXfermodeMode(SkXfermode::kSrc_Mode);
	rast->addLayer(p);

	paint.setRasterizer(rast)->unref();
}

void REEffectHelper::AddShadowEffect(REPaint& paint, REScalar shadowDeviation, REScalar shadowAlpha, bool bUseGap, REScalar gap)
{
	REPaint p;
	RELayerRasterizer*  rast = new RELayerRasterizer;

	p.setAntiAlias(true);

	rast->addLayer(p);

	p.setAlpha(shadowAlpha);
	rast->addLayer(p, shadowDeviation, shadowDeviation);

	if (bUseGap)
	{
		p.setAlpha(0xFF);
		p.setXfermodeMode(SkXfermode::kClear_Mode);
		rast->addLayer(p, gap, gap);
	}

	p.setXfermode(NULL);

	paint.setRasterizer(rast)->unref();
}

void REEffectHelper::AddDiscreteEffect( REPaint& paint, 
	DiscreteTextStyle style /*= DTS_Clear*/, 
	REScalar segLength /*= 4*/, 
	REScalar deviation /*= 3*/)
{
	REPaint p;
	RELayerRasterizer*  rast = new RELayerRasterizer;

	p.setAntiAlias(true);

	rast->addLayer(p);

	if (style == DTS_Fill)
	{
		p.setPathEffect(new SkDiscretePathEffect(SK_Scalar1*4, SK_Scalar1*3))->unref();
		p.setXfermodeMode(SkXfermode::kSrcOut_Mode);
		rast->addLayer(p);
	}
	else
	{
		p.setAntiAlias(false);
		RELayerRasterizer* rast2 = new RELayerRasterizer;
		rast->addLayer(p);
		p.setPathEffect(new SkDiscretePathEffect(SK_Scalar1*4, SK_Scalar1*3))->unref();
		p.setXfermodeMode(SkXfermode::kSrcOut_Mode);
		rast->addLayer(p);
		p.setRasterizer(rast2)->unref();

		p.setXfermodeMode(SkXfermode::kClear_Mode);
		rast->addLayer(p);
	}

	paint.setRasterizer(rast)->unref();
}

void REEffectHelper::AddBlurShadowEffect( REPaint& paint, 
	REScalar radius /*= 3*/, 
	REScalar deviation /*= 3*/, 
	REBlurMaskFilter::BlurStyle blurStyle /*= REBlurMaskFilter::kNormal_BlurStyle*/, 
	REBlurMaskFilter::BlurFlags blurFlags /*= REBlurMaskFilter::kAll_BlurFlag*/ )
{
	REPaint p;
	RELayerRasterizer*  rast = new RELayerRasterizer;

	p.setAntiAlias(true);

	p.setMaskFilter(REBlurMaskFilter::Create(radius,
		blurStyle, blurFlags))->unref();
	rast->addLayer(p, deviation, deviation);

	p.setMaskFilter(NULL);
	rast->addLayer(p);

	paint.setRasterizer(rast)->unref();
}


