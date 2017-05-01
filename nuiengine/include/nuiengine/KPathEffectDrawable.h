//  **************************************
//  File:        KPathEffectDrawable.h
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
#ifndef K_PATHEFFECT_DRAWABLE_H
#define K_PATHEFFECT_DRAWABLE_H


#include "KShapeDrawable.h"


class NUI_API KPathEffectDrawable : public KShapeDrawable
{
public :
	KPathEffectDrawable();
	virtual ~KPathEffectDrawable();
	
};
typedef boost::shared_ptr<KPathEffectDrawable> KPathEffectDrawable_PTR;


#endif
