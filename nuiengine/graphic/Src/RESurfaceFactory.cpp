//  **************************************
//  File:        RESurfaceFactory.cpp
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
#include "SkiaRESurface.h"
#include "REWindow.h"
#include "renderingengine.h"
IRESurface* RESurfaceFactory::CreateRESurface()
{
	return new RESurface;
}

IRESurface* RESurfaceFactory::CreateRESurface( kn_int width, kn_int height, REBitmap::Config colorFormat )
{
	RESurface* pRet = new RESurface;
	if (pRet)
	{
		if (!pRet->Initialize(width, height, colorFormat))
		{
			delete pRet;
			pRet = 0;
		} 
	}
	return pRet;
}

IRESurface* RESurfaceFactory::CreateRESurface(void* p, kn_int width, kn_int height, REBitmap::Config colorFormat )
{
	RESurface* pRet = new RESurface;
	if (pRet)
	{
		if (!pRet->Initialize(p, width, height, colorFormat))
		{
			delete pRet;
			pRet = 0;
		} 
	}
	return pRet;
}

IRESurface* RESurfaceFactory::CreateRESurface( const kn_string& filePath )
{
	RESurface* pRet = new RESurface;
	if (pRet)
	{
		if (!pRet->InitializeFromFile(filePath))
		{
			delete pRet;
			pRet = 0;
		} 
	}
	return pRet;
}

#if defined(WIN32)
IRESurface* RESurfaceFactory::CreateDeviceReSurface(HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = new REWinDeviceSurface(hdc, w, h, cf);
	return pRet;
}

IRESurface* RESurfaceFactory::CreateLayerDeviceReSurface(HWND wnd, HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = new RELayerWinDeviceSurface(wnd, hdc, w, h);
	return pRet;
}

IRESurface* RESurfaceFactory::CreateGLLayerDeviceReSurface(HWND wnd, HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = new REGLLayerWinDeviceSurface(wnd, hdc, w, h);
	return pRet;
}

IRESurface* RESurfaceFactory::CreateDDrawReSurface(HWND hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = NULL;
#ifdef USE_DDRAW
	pRet = new REDDrawSurface(hdc, w, h, cf);
#endif
	return pRet;
}


IRESurface* RESurfaceFactory::CreateGLReSurface(HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = new REGlSurface(hdc, w, h, cf);
	return pRet;
}

IRESurface* RESurfaceFactory::CreateSkiaGLReSurface(HWND wnd, kn_int w, kn_int h, REBitmap::Config cf)
{
	RESurface* pRet = NULL;
	#ifdef SKIAGL
	RESurface* pRet = new RESkiaGLSurface(wnd, w, h, cf);
	#endif
	return pRet;
}

#elif defined(ANDROID_NDK)


IRESurface* RESurfaceFactory::CreateAndroidReSurface(char* buff, kn_int width, kn_int height, REBitmap::Config colorFormat)
{
	//REAndroidSurface::REAndroidSurface(AndroidBitmapInfo& bitmapInfo, char* buff)
	RESurface* pRet = new REAndroidSurface(buff, width, height, colorFormat);
	return pRet;
}

#elif defined(IOS_DEV)

IRESurface* RESurfaceFactory::CreateIOSReSurface(void*& dwAddr, kn_int width, kn_int height, REBitmap::Config colorFormat)
{
	//REAndroidSurface::REAndroidSurface(AndroidBitmapInfo& bitmapInfo, char* buff)
	RESurface* pRet = new REIOSSurface(dwAddr, width, height, colorFormat);
	return pRet;
}

#elif defined(QT_HMI)

IRESurface* RESurfaceFactory::CreateQtSoftReSurface(void* p, kn_int width, kn_int height, REBitmap::Config colorFormat)
{
    RESurface* pRet = new REQtSoftSurface(p, width, height, colorFormat);
    return pRet;
}

#endif

