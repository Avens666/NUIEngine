//  **************************************
//  File:        glenv.cpp
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

#include "REWindow.h"
#include "glenv.h"
CGLEvn g_nui_gl_env;
CGLEvn::CGLEvn()
{
	m_hrc = NULL;
	m_hdc = NULL;

	m_b_init = FALSE;
}

CGLEvn::~CGLEvn()
{

}


void CGLEvn::glenvRelease()
{
#ifdef GL_ENV
	::wglMakeCurrent(NULL,NULL);
	if (NULL != m_hrc)
	{		
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
#endif
}

bool CGLEvn::glenvActive()
{
	return wglMakeCurrent(m_hdc,m_hrc);
}

bool CGLEvn::glenvInit(HDC hdc, int x,  int y,  int t, int b, bool b_mem)
{
	if (m_b_init)
	{
		return TRUE;
	}
	int pixelformat;
	GLdouble rate;

	m_hdc=hdc;

	static PIXELFORMATDESCRIPTOR pdf = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,																			//WORD  nVersion;
		PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER ,		//DWORD dwFlags;
		PFD_TYPE_RGBA,	//BYTE  iPixelType;
		32,				//BYTE  cColorBits;
		0,				//BYTE  cRedBits;
		0,				//BYTE  cRedShift;
		0,				//BYTE  cGreenBits;
		0,				//BYTE  cGreenShift;
		0,				//BYTE  cBlueBits;
		0,				//BYTE  cBlueShift;
		8,				//BYTE  cAlphaBits;
		0,				//BYTE  cAlphaShift;
		0,				//BYTE  cAccumBits;
		0,				//BYTE  cAccumRedBits;
		0,				//BYTE  cAccumGreenBits
		0,				//BYTE  cAccumBlueBits;
		0,				//BYTE  cAccumAlphaBits
		16,				//BYTE  cDepthBits;
		0,				//BYTE  cStencilBits;
		0,				//BYTE  cAuxBuffers;
		PFD_MAIN_PLANE,	//BYTE  iLayerType;
		0,				//BYTE  bReserved;
		0,				//DWORD dwLayerMask;
		0,																		
		0															
	};																			
																				
	if (b_mem)
	{
		pdf.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_TYPE_RGBA|PFD_DRAW_TO_BITMAP|PFD_SUPPORT_GDI;
		
	}


	if((pixelformat = ChoosePixelFormat(m_hdc,&pdf)) == 0)
	{
		return false;
	}

	if(SetPixelFormat(m_hdc,pixelformat,&pdf) == FALSE)
	{
		return false;
	}
	
	//int n = GetPixelFormat(m_hdc);
	//DescribePixelFormat(m_hdc,n,sizeof(pdf),&pdf);

	m_hrc = wglCreateContext(m_hdc);
	bool bb = wglMakeCurrent(m_hdc,m_hrc);

	glViewport(x, y, t, b);		// 重置当前的视口
	glMatrixMode(GL_PROJECTION);				// 选择投影矩阵
	glLoadIdentity();							// 重置投影矩阵
	glMatrixMode(GL_MODELVIEW);					// 选择模型观察矩阵
	glLoadIdentity();							// 重置模型观察矩阵
	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);					// 启用深度测试
	glDepthFunc(GL_LEQUAL);						// 所作深度测试的类型
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glClearColor((0.0f), (0.0f), (0.0f), (0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     
    glFlush();
	glEnable(GL_BLEND);
	glEnable (GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	m_b_init = TRUE;
	return true;
}

void CGLEvn::SwapBuffers()
{
	::SwapBuffers(m_hdc);
}

 //#ifndef UNDER_CE
 //
 //void glenvFlush()
 //{
 //	::SwapBuffers(g_hdc);
 //}
 //
 //#endif


