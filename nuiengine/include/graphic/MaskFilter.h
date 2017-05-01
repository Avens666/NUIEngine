//  **************************************
//  File:        MaskFilter.h
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
#ifndef MaskFilter_DEFINED
#define MaskFilter_DEFINED

#include "SkMaskFilter.h"
#include "effects/SkEmbossMaskFilter.h"
#include "effects/SkBlurMaskFilter.h"
#include "effects/SkTableMaskFilter.h"
#include "effects/SkStippleMaskFilter.h"

typedef SkMaskFilter REMaskFilter;
typedef SkEmbossMaskFilter REEmbossMaskFilter;
typedef SkBlurMaskFilter REBlurMaskFilter;
typedef SkTableMaskFilter RETableMaskFilter;
typedef SkStippleMaskFilter REStippleMaskFilter;

#endif
