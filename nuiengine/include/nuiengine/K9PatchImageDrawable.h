//  **************************************
//  File:        K9PatchImageDrawable.h
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
#ifndef K_9PATCH_IMAGE_DRAWABLE_H
#define K_9PATCH_IMAGE_DRAWABLE_H

#include "KImageDrawable.h"

class NUI_API K9PatchImageDrawable : public KImageDrawable
{
public:
	K9PatchImageDrawable();
	K9PatchImageDrawable(const kn_string& strFile);
	K9PatchImageDrawable(IRESurface* pSurface, kn_bool b_out = FALSE);
	void Initialize();

	virtual ~K9PatchImageDrawable(void);
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);


	void GetSrcRect();
	void GetDesRect();
	
	vector<RERect> m_SrcRect;
	int m_Row;
	int m_Col;
	vector<RERect> m_DesRect;

	RERect m_rectScaleBound;
};

typedef boost::shared_ptr<K9PatchImageDrawable> K9PatchImageDrawable_PTR;

#endif
