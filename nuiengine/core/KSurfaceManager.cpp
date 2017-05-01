//  **************************************
//  File:        KSurfaceManager.cpp
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
#include "KSurfaceManager.h"
#include "renderingengine.h"

KSurfaceManager* g_m = NULL;
KSurfaceManager* getSurfaceManager()
{
	if (g_m == NULL)
	{
		g_m = new KSurfaceManager();
	}
	return g_m;
}

void ReleaseSurfaceManager()
{
	SAFE_DELETE(g_m);
}

KSurfaceManager::KSurfaceManager()
{
}


KSurfaceManager::~KSurfaceManager()
{
	MAPSurfaceIter it;
	for ( it = m_SurfaceManagerMap.begin(); it != m_SurfaceManagerMap.end(); it++ )
	{
		 delete it->second;
	}

	m_SurfaceManagerMap.clear();
}

IRESurface* KSurfaceManager::GetSurface(kn_string surfacepath)
{
	MAPSurfaceIter mapitersurface = m_SurfaceManagerMap.find(surfacepath);
	if (mapitersurface != m_SurfaceManagerMap.end())
	{
		return mapitersurface->second;
	}
	//没有找到
	IRESurface* Surface = RESurfaceFactory::CreateRESurface(surfacepath);
	m_SurfaceManagerMap[surfacepath] = Surface;
	return Surface;
}
