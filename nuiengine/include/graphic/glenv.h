//  **************************************
//  File:        glenv.h
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
#ifndef GLENV_H
#define GLENV_H

#include <GL/gl.h>
#include <GL/glu.h>

class NUI_API CGLEvn
{
public:
	CGLEvn();
	~CGLEvn();
	//释放
	void glenvRelease();
	//初始化环境
	bool glenvInit(HDC hdc, int x,  int y,  int t, int b, bool b_mem = FALSE);
	//设为当前环境
	bool glenvActive();
	//上屏
	void SwapBuffers();

	HGLRC m_hrc;
	HDC m_hdc;

	RECT m_rectClient;

	kn_bool  m_b_init;
};

#endif //GLENV_H
