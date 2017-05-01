//  **************************************
//  File:        REColor.h
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
#ifndef REColor_DEFINED
#define REColor_DEFINED

#include "core/SkColor.h"

typedef  SkColor REColor;

//颜色转换宏
#define RGB555(r, g, b) ((((b) >> 3) & 31) + ((((g) >> 3) & 31) << 5) + ((((r)>>3) & 31) << 10))
#define RGB565(r, g, b) ((((b) >> 3) & 31) + ((((g) >> 2) & 63) << 5) + ((((r)>>3) & 31) << 11))
#define RGB888(r, g, b) ( ((b) & 0xff) + (((g) & 0xff) << 8) + (((r) & 0xff) << 16))

#define ARGB(a,r,g,b) SkColorSetARGB(a, r, g, b)
 
//分离颜色宏
#define GetRPart_555(color) (unsigned char )(((color >> 10) & 31) << 3)
#define GetGPart_555(color) (unsigned char )(((color >> 5) & 31) << 3)
#define GetBPart_555(color) (unsigned char )((color & 31)<<3)

#define GetRPart_565(color) (unsigned char )(((color >> 11) & 31) << 3)
#define GetGPart_565(color) (unsigned char )(((color >> 5) & 63) << 2)
#define GetBPart_565(color) (unsigned char )((color & 31) << 3)

#define GetRPart_888(color) (unsigned char )(((color) & 0xff0000) >> 16)
#define GetGPart_888(color) (unsigned char )(((color) & 0xff00) >> 8)
#define GetBPart_888(color) (unsigned char )((color) & 0xff)

#define ColorSetARGB(a, r, g, b) \
	static_cast<SkColor>( \
	(static_cast<U8CPU>(a) << 24) | \
	(static_cast<U8CPU>(r) << 16) | \
	(static_cast<U8CPU>(g) << 8) | \
	(static_cast<U8CPU>(b) << 0))

/** return the alpha byte from a SkColor value */
#define ColorGetA(color)      (((color) >> 24) & 0xFF)
/** return the red byte from a SkColor value */
#define ColorGetR(color)      (((color) >> 16) & 0xFF)
/** return the green byte from a SkColor value */
#define ColorGetG(color)      (((color) >>  8) & 0xFF)
/** return the blue byte from a SkColor value */
#define ColorGetB(color)      (((color) >>  0) & 0xFF)

static inline SkColor ColorSetA(SkColor c, U8CPU a) {
	return (c & 0x00FFFFFF) | (a << 24);
}
// common colors
#define RE_ColorTRANSPARENT 0x00000000  //!< transparent

#define RE_ColorBLACK       0xFF000000  //!< black
#define RE_ColorDKGRAY      0xFF444444  //!< dark gray
#define RE_ColorGRAY        0xFF888888  //!< gray
#define RE_ColorLTGRAY      0xFFCCCCCC  //!< light
#define RE_ColorWHITE       0xFFFFFFFF  //!< white 

#define RE_ColorRED         0xFFFF0000  //!< red
#define RE_ColorGREEN       0xFF00FF00  //!< green
#define RE_ColorBLUE        0xFF0000FF  //!< blue
#define RE_ColorYELLOW      0xFFFFFF00  //!< yellow
#define RE_ColorCYAN        0xFF00FFFF  //!< cyan
#define RE_ColorMAGENTA     0xFFFF00FF  //!< magenta

#endif
