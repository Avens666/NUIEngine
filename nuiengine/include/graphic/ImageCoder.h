//  **************************************
//  File:        ImageCoder.h
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
#ifndef ImageCoder_DEFINED
#define ImageCoder_DEFINED

#include "SkImageDecoder.h"
#include "SkImageEncoder.h"

typedef SkImageEncoder REImageEncoder;
typedef SkImageDecoder REImageDecoder;

// 缩放模式
enum RESCALEMODE
{
	RE_FIT_WIDTH,	//	适合宽度
	RE_FIT_RECT,	// 适合高度和宽度
	RE_FIT_FULL	// 充满区域

};

#endif
