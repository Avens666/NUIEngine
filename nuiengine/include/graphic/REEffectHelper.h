//  **************************************
//  File:        REEffectHelper.h
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
#ifndef REEffectHelper_DEFINED
#define REEffectHelper_DEFINED

#include "renderingengine.h"
#include "DrawLooper.h"
#include "MaskFilter.h"

class NUI_API REEffectHelper
{
public:
	// 带描边填充效果, 填充效果可以选择透明度, 达到镂空的效果
	static void AddStrokeAndFilledEffect(REPaint& paint, REScalar strokeWidth = 3, REScalar filledAlpha = 0x20);
	// 阴影效果, 阴影可以选择离本体一段距离(shadowDeviation). 假如阴影和本体重合了, 也可以选择在阴影与本体之间插入间隔效果(bUseGap, gap)
	static void AddShadowEffect(REPaint& paint, REScalar shadowDeviation = 3, REScalar shadowAlpha = 0x60, bool bUseGap = true, REScalar gap = 3 / 2);
	// 模糊阴影效果, 模糊阴影可以离本体一段距离
	static void AddBlurShadowEffect(REPaint& paint, REScalar radius = 3, REScalar deviation = 3,
		REBlurMaskFilter::BlurStyle blurStyle = REBlurMaskFilter::kNormal_BlurStyle, REBlurMaskFilter::BlurFlags blurFlags = REBlurMaskFilter::kAll_BlurFlag);
	// 斑驳效果
	enum DiscreteTextStyle
	{
		DTS_Clear,	// 用生成的斑驳图案清空, 其余部分不变
		DTS_Fill			// 用生成的斑驳图案填充, 其余部分清空
	};
	static void AddDiscreteEffect(REPaint& paint, DiscreteTextStyle style = DTS_Clear, REScalar segLength = 4, REScalar deviation = 3 );
};

#endif
