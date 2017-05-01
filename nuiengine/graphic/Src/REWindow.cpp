//  **************************************
//  File:        REWindow.cpp
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

#ifdef WIN32
REWinDeviceSurface::REWinDeviceSurface(HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	m_hdc = hdc;
	m_x_dc = 0;
	m_y_dc = 0;
	Initialize(w,h, cf);

	//用new是为了防止底层函数发生越界读
	m_p_bmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 10 * sizeof(RGBQUAD));

	switch (cf)
	{
	case REBitmap::kRGB_565_Config:
		{
			m_p_bmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			m_p_bmi->bmiHeader.biWidth       = w;
			m_p_bmi->bmiHeader.biHeight      = -h; // top()-down image
			m_p_bmi->bmiHeader.biPlanes      = 1;
			m_p_bmi->bmiHeader.biSizeImage   = 0;
			m_p_bmi->bmiHeader.biCompression = BI_BITFIELDS;
			m_p_bmi->bmiHeader.biBitCount = 16;
			m_p_bmi->bmiHeader.biSizeImage =  GetWidth() * GetHeight() * 2;
			kn_dword* p = (kn_dword*)((kn_byte*)m_p_bmi + m_p_bmi->bmiHeader.biSize);
			*p++ = (kn_dword)(0xF800);
			*p++ = (kn_dword)(0x07E0);
			*p++ = (kn_dword)(0x001F);
		}
		break;
	case REBitmap::kARGB_4444_Config:
		{
			m_p_bmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			m_p_bmi->bmiHeader.biWidth       = w;
			m_p_bmi->bmiHeader.biHeight      = -h; // top()-down image
			m_p_bmi->bmiHeader.biPlanes      = 1;
			m_p_bmi->bmiHeader.biSizeImage   = 0;
			m_p_bmi->bmiHeader.biCompression = BI_RGB;
			m_p_bmi->bmiHeader.biBitCount = 16;
		}
		break;
	case REBitmap::kARGB_8888_Config:
		{
			m_p_bmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
			m_p_bmi->bmiHeader.biWidth       = w;
			m_p_bmi->bmiHeader.biHeight      = -h; // top()-down image
			m_p_bmi->bmiHeader.biPlanes      = 1;
			m_p_bmi->bmiHeader.biSizeImage   = 0;
			m_p_bmi->bmiHeader.biBitCount    = 32;
			m_p_bmi->bmiHeader.biCompression = BI_RGB;
		}
		break;
	default:
		break;
	}
}

REWinDeviceSurface::~REWinDeviceSurface() 
{
	SAFE_DELETE(m_p_bmi);

}

bool g_b_draw_drawing = FALSE;

void ShowRefreshingBound(kn_bool bShow)
{
	extern bool g_b_draw_drawing;
	g_b_draw_drawing = bShow;
}

void REWinDeviceSurface::Flip(LSTRECT &lst_rect)
{
	if (m_hdc)
	{
		//SelectObject(m_hdc, GetStockObject(BLACK_BRUSH)); 
		//Rectangle(m_hdc, 0, 0 , GetWidth(), GetHeight());

		kn_int width = GetWidth();
		kn_int height = GetHeight();
		REBitmap::Config colorFormat =GetColorFormat();
		if (colorFormat != SkBitmap::kNo_Config)
		{
				int x = m_x_dc;
				int y = m_y_dc;
				int w = width;
				int h = height;
	
				LockBuffer();
				kn_byte* pBitmapBuffer = GetBitmapBuffer();
				if (lst_rect.size() == 0)
				{// 全屏渲染
					int iRet = SetDIBitsToDevice(m_hdc,
						x, y,
						w, h,
						0, 0,
						y, h,
						pBitmapBuffer,
						m_p_bmi,
						DIB_RGB_COLORS);
				}
				else
				{
					for(LSTRECT::iterator ite = lst_rect.begin();ite!=lst_rect.end();ite++)
					{
						RERect& obj = *ite;
						obj.intersect(0,0,width, height);
						int iRet = SetDIBitsToDevice(m_hdc,
							x + obj.left(), y + obj.top(),
							obj.width(), obj.height(),
							obj.left(), 0,
							0, obj.top() + obj.height(),
							pBitmapBuffer,
							m_p_bmi,
							DIB_RGB_COLORS);

						if (g_b_draw_drawing)
						{//绘制刷新区域，测试用
							SelectObject(m_hdc, GetStockObject(NULL_BRUSH)); 
							SelectObject(m_hdc, GetStockObject(BLACK_PEN)); 
							Rectangle(m_hdc, x + obj.left(), y + obj.top(), x + obj.left()+obj.width(),  y + obj.top()+obj.height());						
						}

					}

				}

				UnlockBuffer();
		}
	}
}


//////////////////////DDraw///////////////////////////////////////////////////////
#ifdef USE_DDRAW
#pragma comment(lib,"ddraw.lib")
#include <ddraw.h>
LPDIRECTDRAW m_pMyDD=0;
LPDIRECTDRAWSURFACE m_pMyDDSFront=0;
LPDIRECTDRAWSURFACE m_pMyDDSBack=0;
LPDIRECTDRAWCLIPPER m_pMyClipper=0; 
LPDIRECTDRAWPALETTE m_myDDPal=0; 
REDDrawSurface::REDDrawSurface(HWND hwnd,kn_int w, kn_int h, REBitmap::Config cf)
{
	m_hwnd = hwnd;
	m_hdc = GetDC(hwnd);
	m_x_dc = 0;
	m_y_dc = 0;
	Initialize(w,h, cf);

	RECT rc,rcClient;
	GetWindowRect(m_hwnd, &rc);
	GetClientRect(m_hwnd, &rcClient);

	if (FAILED(DirectDrawCreate(NULL,&m_pMyDD,NULL)))goto error ;//初始化
	if (FAILED(m_pMyDD->SetCooperativeLevel(m_hwnd,DDSCL_NORMAL)))goto error ;//设置屏幕的显示方式

//	if (FAILED(m_pMyDD->SetCooperativeLevel(m_hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)))goto error ;//设置屏幕的显示方式
//	if (FAILED(m_pMyDD->SetDisplayMode(1024,768,32)))goto error;	
	
	DDSURFACEDESC ddsd; //这个结构描述"页"的特徵.
	::memset(&ddsd,0,sizeof(ddsd));


//	// Create the primary surface	
	ddsd.dwFlags = DDSD_CAPS; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;//指定我们用的是前页.
	ddsd.dwSize = sizeof(ddsd); //尺寸
	ddsd.dwWidth = rcClient.right(); //设定后页的大小, 
	ddsd.dwHeight =rcClient.bottom();
	// 做前页:
	if(DD_OK!=m_pMyDD->CreateSurface(&ddsd, &m_pMyDDSFront, NULL))goto error;

	//指定 我们要后页
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN ;
	//做后页
	if(DD_OK!=m_pMyDD->CreateSurface(&ddsd, &m_pMyDDSBack, NULL))goto error;

	if(DD_OK!=m_pMyDD->CreateClipper(0, &m_pMyClipper, NULL))goto error;
	m_pMyClipper->SetHWnd(0,hwnd);
	m_pMyDDSFront->SetClipper(m_pMyClipper);
	

//	// Create the primary surface with 1 back buffer
//	DDSCAPS2                    ddscaps;
//	HRESULT                     hRet;
//	ZeroMemory(&ddsd, sizeof(ddsd));
//	ddsd.dwSize = sizeof(ddsd);
//	ddsd.dwFlags = DDSD_CAPS;// | DDSD_BACKBUFFERCOUNT;
//	ddsd.dwWidth = rcClient.right(); //设定后页的大小, 
//	ddsd.dwHeight =rcClient.bottom();
//	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP ;
////	ddsd.dwBackBufferCount = 1;
//	hRet = m_pMyDD->CreateSurface(&ddsd, &m_pMyDDSFront, NULL);
//	if (hRet != DD_OK)
//		goto error;
//
//	// Get a pointer to the back buffer
//	ZeroMemory(&ddscaps, sizeof(ddscaps));
//	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
//	hRet = m_pMyDDSFront->GetAttachedSurface((DDSCAPS*)&ddscaps, &m_pMyDDSBack);

	return ;
error:
	if(m_pMyDD)m_pMyDD->Release();
	m_pMyDD = NULL;
	return ;
}

REDDrawSurface::~REDDrawSurface() 
{
	if (m_pMyDD != NULL)
	{
		if (m_pMyDDSFront != NULL)
		{
			m_pMyDDSFront->Release();
			m_pMyDDSFront = NULL;
		}
		if (m_pMyDDSBack != NULL)
		{
			m_pMyDDSBack->Release();
			m_pMyDDSBack = NULL;
		}

		m_pMyDD->Release();
		m_pMyDD = NULL;
	}
	if(m_pMyDD)m_pMyDD->Release();
}

void REDDrawSurface::Flip(LSTRECT &lst_rect)
{
	if (m_pMyDD != NULL)
	{
		//SelectObject(m_hdc, GetStockObject(BLACK_BRUSH)); 
		//Rectangle(m_hdc, 0, 0 , GetWidth(), GetHeight());

		kn_int width = GetWidth();
		kn_int height = GetHeight();
		REBitmap::Config colorFormat =GetColorFormat();
		if (colorFormat != REBitmap::kNo_Config)
		{
			int x = m_x_dc;
			int y = m_y_dc;
			int w = width;
			int h = height;
			int l,t;

			LockBuffer();
			kn_byte* pBitmapBuffer = GetBitmapBuffer();
			if (lst_rect.size() == 0)
			{// 全屏渲染

			}
			else
			{
				DDSURFACEDESC   ddsd;
				ddsd.dwSize = sizeof(ddsd);
				HRESULT hRet = m_pMyDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
				if (hRet != DD_OK)	return;
				LPSTR pBits = NULL;
				RECT rect;
				//for(LSTRECT::iterator ite = lst_rect.begin();ite!=lst_rect.end();ite++)
				//{
				//	if ( !(*ite).Intersect(0,0,width, height))
				//	{
				//		//屏幕外
				//		continue;
				//	}
				//	 
				//	w = (*ite).width();
				//	h = (*ite).height();
				//	l = (*ite).left();
				//	t=  (*ite).top();

				//	kn_byte* psrc= pBitmapBuffer;
				//	pBits = (LPSTR) ddsd.lpSurface + l*4 + t*ddsd.lPitch;
				//	for (int i = 0; i < h; i++)
				//	{
				//		memcpy(pBits, psrc, w *4);
				//		pBits += ddsd.lPitch;
				//		psrc += ddsd.lPitch;
				//	}

				//	rect.left() = (*ite).left();
				//	rect.top()  = (*ite).top();
				//	rect.right() = (*ite).right();
				//	rect.bottom() = (*ite).bottom();
				//}

				memcpy(ddsd.lpSurface, pBitmapBuffer, width*height*4);
					m_pMyDDSBack->Unlock(NULL);

					rect.left() = x;
					rect.top()  = y;
					rect.right() = x+w;
					rect.bottom() = y+h;
				RECT rc,rcClient;
				GetWindowRect(m_hwnd, &rc);
				m_pMyDDSFront->Blt(&rc,m_pMyDDSBack,&rect,DDBLT_WAIT,0);

				m_pMyDDSFront->BltFast(x,y,m_pMyDDSBack,&rect,DDBLT_WAIT );
				//while (TRUE)
				//{
				//	hRet = m_pMyDDSFront->Flip(NULL, 0);
				//	if (hRet == DD_OK)
				//		break;
				//	if (hRet == DDERR_SURFACELOST)
				//	{
				//		hRet = m_pMyDDSFront->Restore();
				//		if (hRet != DD_OK)
				//			break;
				//	}
				//	if (hRet != DDERR_WASSTILLDRAWING)
				//		break;
				//}

			}

			UnlockBuffer();
		}
	}
}
#endif // USE_DDRAW



#include <gl\gl.h>
#include <gl\glu.h>
#include "gpu/GrTypes.h"
#include "gpu/SkGpuDevice.h"
#include "gpu/GrContext.h"
#include "gpu/GrContextFactory.h"
#include "glenv.h"
//class CGLEvn
//{
//public:
//	CGLEvn();
//	~CGLEvn();
//	//释放
//	void glenvRelease();
//	//初始化环境
//	bool glenvInit(HDC hdc, int x,  int y,  int t, int b);
//	//设为当前环境
//	bool glenvActive();
//	//上屏
//	void SwapBuffers();
//
//	HGLRC m_hrc;
//	HDC m_hdc;
//
//	RECT m_rectClient;
//
//	kn_bool  m_b_init;
//};

REGlSurface::REGlSurface(HDC hdc,kn_int w, kn_int h, REBitmap::Config cf)
{
	m_hdc = hdc;
	Initialize(w,h, REBitmap::kARGB_8888_Config);
	m_p_data = new kn_byte[w*h*4];

	m_vt[0] = 0;
	m_vt[1] = 0;
	m_vt[2] = -100;

	m_vt[3] = w;
	m_vt[4] = 0;
	m_vt[5] = -100;

	m_vt[6] = w;
	m_vt[7] = h;
	m_vt[8] = -100;

	m_vt[9] = 0;
	m_vt[10] = h;
	m_vt[11] = -100;

	m_uv[0] = 0.0f;
	m_uv[1] = 1.0f;
	m_uv[2] = 1.0f;
	m_uv[3] = 1.0f;
	m_uv[4] = 1.0f;
	m_uv[5] = 0.0f;
	m_uv[6] = 0.0f;
	m_uv[7] = 0.0f;	

	extern CGLEvn g_nui_gl_env;
	setGLEnv(&g_nui_gl_env);
	return;
	///gpu加速，还没走通
	//GrContext* ctx = GrContext::Create(kOpenGL_GrBackend, 0);

	//GLint texID;
	////... assign texID

	//GrContextFactory::GLContextType glType = (GrContextFactory::kNative_GLContextType);
	//GrContextFactory factory;
	//GrContext* grContext = factory.get(glType);
	//if (NULL == grContext) {
	//	return;
	//}

	//GrTextureDesc desc;
	//desc.fConfig = kSkia8888_GrPixelConfig;
	//desc.fFlags = kRenderTarget_GrTextureFlagBit;
	//desc.fWidth = w;
	//desc.fHeight = h;
	//desc.fSampleCnt = 4;
	//SkAutoTUnref<GrTexture> texture(grContext->createUncachedTexture(desc, NULL, 0));
	//SkAutoTUnref<SkGpuDevice> device(SkNEW_ARGS(SkGpuDevice, (grContext, texture.get())));

	//m_pBitmap = new REBitmap;
	//m_pBitmap->setConfig(cf, w, h);
	//m_pBitmap->allocPixels();

	//m_pCanvas = new RECanvas(device.get());

}

REGlSurface::~REGlSurface() 
{
	glDeleteTextures(1, &m_texture_id);
	SAFE_DELETE(m_p_data);
}

void REGlSurface::Flip(LSTRECT &lst_rect)
{
///////////////
/////////////////
	int h = GetHeight();
	int w = GetWidth();

	LockBuffer();
	kn_byte* p_data = GetBitmapBuffer();
	//
	int i, j, vi;
	vi = h-1;
	int offset = 0;
	int lw = w*4;
	int j_max = 0;
	for (i = 0; i<h; i++ )
	{
		j_max = offset + lw;
		for(j=offset; j < j_max; j+=4 )
		{// RGBA -> BGRA 
			m_p_data[j]   = p_data[j + 2];
			m_p_data[j+1] = p_data[j + 1];
			m_p_data[j+2] = p_data[j + 0];
			m_p_data[j+3] = p_data[j + 3];
		}
		vi--;
		offset += lw;
	}

	UnlockBuffer();

	/////////////////////////////////

	//glDisable(GL_LIGHTING);
	//glViewport( 0, 0, GetWidth(), GetHeight());

	//glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glPushMatrix();										// Store The Projection Matrix
	//glLoadIdentity();									// Reset The Projection Matrix
	//glOrtho(0,GetWidth(),0,GetHeight(),-1,1);							// Set Up An Ortho Screen
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	//glPushMatrix();										// Store The Modelview Matrix
	//glLoadIdentity();	
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
	//glEnable(GL_BLEND);
	//glRasterPos2i(0,0);

	//glDrawPixels(GetWidth(), GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, m_p_data);

	//glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	//glPopMatrix();										// Restore The Old Projection Matrix
	//glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	//glPopMatrix();										// Restore The Old Projection Matrix
	//glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	///////////////////////////

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -400, 400);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	 glViewport(0, 0, w, h);


	glColor4f(1.0f,1.0f,1.0f,1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// Create Nearest Filtered Texture

		glBindTexture(GL_TEXTURE_2D, m_texture_id);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_p_data);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexCoordPointer(2, GL_FLOAT, 0, m_uv);	
		glVertexPointer (3, GL_FLOAT, 0, m_vt);
		glDisableClientState(GL_COLOR_ARRAY);
		glDrawArrays(GL_TRIANGLE_FAN,0,4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	
	///////////////////////////////
	m_nui_gl_env->SwapBuffers();



}

void REGlSurface::setGLEnv(CGLEvn*  gl)
{
	
	m_nui_gl_env = gl;
	glGenTextures(1, &m_texture_id);
}

void REGlSurface::preDraw()
{
	//if (m_nui_gl_env->m_b_init == FALSE)
	//{
	//	m_nui_gl_env->glenvInit(m_hdc, 0,0, GetWidth(), GetHeight() );
	//	
	//}
	m_nui_gl_env->glenvActive();

	glClearColor((0.0f), (0.0f), (0.0f), (0.0f));
	glClear(GL_COLOR_BUFFER_BIT );  
}

#if 0
/*
 *  add by boy for GLES2.0 --- 2015.6.5
 *	Create GLES Env
 */
#include "KGlesEnv.h"
#include "IRender.h"

REGLESSurface::REGLESSurface(kn_int w, kn_int h, REBitmap::Config cf)
{
	m_isSetEnv = false;

	Initialize(w,h, REBitmap::kARGB_8888_Config);
	m_p_data = new kn_byte[w*h*4];

	m_vt[0] = 0;
	m_vt[1] = 0;
	m_vt[2] = -100;

	m_vt[3] = w;
	m_vt[4] = 0;
	m_vt[5] = -100;

	m_vt[6] = w;
	m_vt[7] = h;
	m_vt[8] = -100;

	m_vt[9] = 0;
	m_vt[10] = h;
	m_vt[11] = -100;

	m_uv[0] = 0.0f;
	m_uv[1] = 1.0f;
	m_uv[2] = 1.0f;
	m_uv[3] = 1.0f;
	m_uv[4] = 1.0f;
	m_uv[5] = 0.0f;
	m_uv[6] = 0.0f;
	m_uv[7] = 0.0f;	

	//extern KGlesEnv g_nui_gles_env;
	//setGLESEnv(&g_nui_gles_env);

	return;
}

REGLESSurface::~REGLESSurface() 
{
	glDeleteTextures(1, &m_texture_id);
	SAFE_DELETE(m_p_data);
}

void REGLESSurface::Flip(LSTRECT &lst_rect)
{
	int h = GetHeight();
	int w = GetWidth();

	LockBuffer();
	kn_byte* p_data = GetBitmapBuffer();
#if 0
	int i, j, vi;
	vi = h-1;
	int offset = 0;
	int lw = w*4;
	int j_max = 0;
	for (i = 0; i<h; i++ )
	{
		j_max = offset + lw;
		for(j=offset; j < j_max; j+=4 )
		{// RGBA -> BGRA 
			m_p_data[j]   = p_data[j + 2];
			m_p_data[j+1] = p_data[j + 1];
			m_p_data[j+2] = p_data[j + 0];
			m_p_data[j+3] = p_data[j + 3];
		}
		vi--;
		offset += lw;
	}
#endif
	UnlockBuffer();

	IRender *m_p_render = GetRenderSingleton();

	m_p_render->loadIdentity(GLRENDER_PROJECTION);
	m_p_render->setOrthof(0, w, 0, h, -400, 400);
	m_p_render->loadIdentity(GLRENDER_MODELVIEW);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glViewport(0, 0, w, h);

	// Create Nearest Filtered Texture
	glBindTexture(GL_TEXTURE_2D, m_texture_id);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA_EXT, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, p_data);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	m_p_render->setTexCoordPointer(m_uv);
	m_p_render->setVertexPointer(m_vt);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	m_p_render->pushMatrix();
	m_p_render->useShader(m_p_render->getModelViewProjectionMatrix(), 0);
	m_p_render->drawArrays(GL_TRIANGLE_FAN,0, 4);
	m_p_render->popMatrix();
	glDisable(GL_BLEND);

	m_nui_gles_env->GLESFlush();
}

void REGLESSurface::setGLESEnv(KGlesEnv* gles)
{
	m_nui_gles_env = gles;
	glGenTextures(1, &m_texture_id);
}

void REGLESSurface::preDraw(void)
{
	if( !m_isSetEnv )
	{
		extern KGlesEnv g_nui_gles_env;
		setGLESEnv(&g_nui_gles_env);

		m_isSetEnv = true;
	}

	m_nui_gles_env->GLESActive();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}
/**********************************************************/
#endif

#ifdef SKIAGL
// add by boy for skia-gpu draw --- 2015.6.5
RESkiaGLSurface::RESkiaGLSurface(HWND wnd, kn_int w, kn_int h, REBitmap::Config cf)
{
	m_isSetEnv = false;

	m_context = NULL;
	m_surface = NULL;
	m_interface = NULL;
	m_renderTarget = NULL;

	m_width = w;
	m_height = h;
	m_wnd = wnd;
}

RESkiaGLSurface::~RESkiaGLSurface() 
{
	SkSafeUnref(m_context);
	m_context = NULL;

	SkSafeUnref(m_interface);
	m_interface = NULL;

	SkSafeUnref(m_renderTarget);
	m_renderTarget = NULL;

	SkSafeUnref(m_surface);
	m_surface = NULL; 

	m_SkWind->detach();

	// 避免父类delete
	m_pCanvas = NULL;

}

void RESkiaGLSurface::Flip(LSTRECT &lst_rect)
{
#if 0	// use to test
	SkPaint paint;   

	m_pCanvas->clear(0xff555555);

	// draw points with red.   
	paint.setARGB(255, 255, 0, 0);  
	paint.setStrokeWidth(4); 
	paint.setAntiAlias(true);
	m_pCanvas->drawPoint(100,200, paint);    
	m_pCanvas->drawPoint(80,200, paint);    
	m_pCanvas->drawPoint(120,200, paint);    

	//draw a line with green.   
	paint.setARGB(255, 0, 255, 0);    
	paint.setStrokeWidth(4);    
	SkMatrix m;
	m.setRotate(30, 180, 160);
	m_pCanvas->setMatrix(m);
	m_pCanvas->drawLine(160,150,320,110,paint);  
	m_pCanvas->resetMatrix();

	//draw a circle with bule.  
	paint.setARGB(100, 0, 0, 255);    
	m_pCanvas->drawCircle(100,100,50,paint); 

	SkPaint paintRect;
	paintRect.setColor(0xffff0000);
	paintRect.setStrokeJoin(SkPaint::kRound_Join);
	paintRect.setAntiAlias(true);
	paintRect.setStyle( SkPaint::kStroke_Style); 
	paintRect.setStrokeWidth(3);
	SkRect m_rect;
	m_rect.setXYWH(200, 200, 200, 200);
	m_pCanvas->drawRect(m_rect, paintRect);
#endif

	m_context->resetContext();
	m_context->flush();

	m_SkWind->present();
}

kn_bool RESkiaGLSurface::Initialize(kn_dword width, kn_dword height, REBitmap::Config colorFormat)
{
	m_SkWind = new SkOSWindow(m_wnd);
	SkASSERT(NULL != m_SkWind);
	if( m_SkWind == NULL )
	{
		return false;
	}

	m_SkWind->attach(SkOSWindow::kNativeGL_BackEndType, 4, new SkOSWindow::AttachmentInfo());

	m_interface = GrGLCreateNativeInterface();
	SkASSERT(NULL != m_interface);
	if( m_interface == NULL )
	{
		return false;
	}

	m_context = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)m_interface);
	SkASSERT(NULL != m_context);
	if( m_context == NULL )
	{
		return false;
	}

	GrBackendRenderTargetDesc desc;
	desc.fWidth = m_width;
	desc.fHeight = m_height;
	desc.fConfig = kRGB_565_GrPixelConfig;
	desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
	desc.fSampleCnt = 4;
	desc.fStencilBits = 8;
	GrGLint buffer = 0;
	//GR_GL_GetIntegerv(fInterface, GR_GL_FRAMEBUFFER_BINDING, &buffer);
	desc.fRenderTargetHandle = buffer;
	m_renderTarget = m_context->wrapBackendRenderTarget(desc);
	SkASSERT(NULL != m_renderTarget);
	if( m_renderTarget == NULL ) 
	{
		return false;
	}

	m_surface = SkSurface::NewRenderTargetDirect(m_context, m_renderTarget);
	SkASSERT(NULL != m_surface);
	if( m_surface == NULL )
	{
		return false;
	}

	m_pCanvas = m_surface->getCanvas();
	SkASSERT(NULL != m_pCanvas);
	if( m_pCanvas == NULL )
	{
		return false;
	}

	m_isSetEnv = true;

	return true;
}

void RESkiaGLSurface::preDraw(void)
{
	m_pCanvas->clear(0xff555555);
}


void RESkiaGLSurface::Release()
{
	//m_SkWind
	//printf("1111");
}
#endif //SKIAGL
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
RELayerWinDeviceSurface::RELayerWinDeviceSurface(HWND wnd, HDC hdc,kn_int w, kn_int h):REWinDeviceSurface(hdc,w,h, REBitmap::kARGB_8888_Config )
{
	m_hwnd = wnd;
	
	DWORD dwExStyle=GetWindowLong(m_hwnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(m_hwnd,GWL_EXSTYLE,dwExStyle^0x80000);

	m_mem_dc = ::CreateCompatibleDC(m_hdc);
	void* pBits = NULL;
	m_hbitmap = CreateDIBSection(m_hdc,m_p_bmi,DIB_RGB_COLORS,&pBits,NULL,0);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(m_mem_dc,m_hbitmap);
}

RELayerWinDeviceSurface::~RELayerWinDeviceSurface() 
{
	::SelectObject(m_mem_dc,NULL);
	::DeleteObject(m_hbitmap);
	DeleteDC(m_mem_dc);
}

void RELayerWinDeviceSurface::Flip(LSTRECT &lst_rect)
{
	if (m_hdc)
	{
		RECT rc;
		GetWindowRect(m_hwnd, &rc);
		POINT ptDest ={rc.left,rc.top};
		SIZE  sizeDest = {rc.right - rc.left, rc.bottom - rc.top };

		if (m_hdc)
		{
			kn_int w = GetWidth();
			kn_int h = GetHeight();
			int x = m_x_dc;
			int y = m_y_dc;

			LockBuffer();
			kn_byte* pBitmapBuffer = GetBitmapBuffer();
			// 全屏渲染
				int iRet = SetDIBitsToDevice(m_mem_dc,
					x, y,
					w, h,
					0, 0,
					y, h,
					pBitmapBuffer,
					m_p_bmi,
					DIB_RGB_COLORS);

			UnlockBuffer();

		}


		BLENDFUNCTION blf;
		blf.BlendOp = AC_SRC_OVER;
		blf.BlendFlags = 0;
		blf.SourceConstantAlpha = 255;
		blf.AlphaFormat = 1;//AC_SRC_ALPHA;
		POINT ptSrc = {0,0};
		UpdateLayeredWindow( m_hwnd,m_hdc,&ptDest,&sizeDest,m_mem_dc,&ptSrc,0,&blf,2/*ULW_ALPHA*/);


		//::SelectObject(hMemDc,hOldBitmap);
		//::DeleteObject(hBitmap);
		//DeleteDC(hMemDc);
		//::ReleaseDC(GetSafeHwnd(),hDC);
		
	}
}

///////////////////////////////////////////////////////
REGLLayerWinDeviceSurface::REGLLayerWinDeviceSurface(HWND wnd, HDC hdc,kn_int w, kn_int h):REGlSurface(hdc,w,h, REBitmap::kARGB_8888_Config )
{
	m_p_bmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 10 * sizeof(RGBQUAD));
	m_p_bmi->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	m_p_bmi->bmiHeader.biWidth       = w;
	m_p_bmi->bmiHeader.biHeight      = h; // top()-down image
	m_p_bmi->bmiHeader.biPlanes      = 1;
	m_p_bmi->bmiHeader.biSizeImage   = 0;
	m_p_bmi->bmiHeader.biBitCount    = 32;
	m_p_bmi->bmiHeader.biCompression = BI_RGB;

	m_blf.BlendOp = AC_SRC_OVER;
	m_blf.BlendFlags = 0;
	m_blf.SourceConstantAlpha = 255;
	m_blf.AlphaFormat = AC_SRC_ALPHA;
	
	m_hwnd = wnd;
	m_buffer = NULL;
	m_enable_gl = true;

	DWORD dwExStyle=GetWindowLong(m_hwnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(m_hwnd,GWL_EXSTYLE,dwExStyle^0x80000);

	m_mem_dc = ::CreateCompatibleDC(m_hdc);
	void* pbyte;
	m_hbitmap = CreateDIBSection(m_hdc,m_p_bmi,DIB_RGB_COLORS,&(pbyte),NULL,0);
  	m_p_mem_data = (BYTE *)pbyte;
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(m_mem_dc,m_hbitmap);

}

REGLLayerWinDeviceSurface::~REGLLayerWinDeviceSurface() 
{
	::SelectObject(m_mem_dc,NULL);
	::DeleteObject(m_hbitmap);
	DeleteDC(m_mem_dc);

}

void REGLLayerWinDeviceSurface::Flip(LSTRECT &lst_rect)
{
	if (m_hdc)
	{
//		REGlSurface::Flip(lst_rect);
	
		kn_int w = GetWidth();
		kn_int h = GetHeight();
		int x = 0;
		int y = 0;
		
	  if (m_enable_gl)
	  {
		glReadPixels(0,0,w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, m_p_mem_data);

		int i, j, vi;
		vi = h-1;
		int offset = 0;
		int lw = w*4;
		int j_max = 0;
		BYTE tmp;


		LockBuffer();
		kn_byte* pBitmapBuffer = GetBitmapBuffer();

		vi = h-1;
		offset = 0;
		int offset2 = lw*(h-1);
		j_max = 0;
		kn_byte*  p_buf_mem;
		kn_byte*  p_buf_bitmap;
		for (i = 0; i<h; i++ )
		{
			j_max = offset + lw;

			p_buf_mem = m_p_mem_data + offset;
			p_buf_bitmap = pBitmapBuffer + offset2;

			for(j=offset ; j < j_max; j+=4 )
			{
				tmp = *(p_buf_bitmap+3);
				if (tmp > 0)
				{
					if (tmp == 255)
					{
						memcpy(p_buf_mem, p_buf_bitmap, 4);
					}
					else
					{
						*(p_buf_mem)	= (*(p_buf_mem)  *(255-tmp) + *(p_buf_bitmap)  *tmp)>>8;
						*(p_buf_mem+1)	= (*(p_buf_mem+1)*(255-tmp) + *(p_buf_bitmap+1)*tmp)>>8;
						*(p_buf_mem+2)	= (*(p_buf_mem+2)*(255-tmp) + *(p_buf_bitmap+2)*tmp)>>8;
						*(p_buf_mem+3)	= max( *(p_buf_mem+3), tmp );
					}
				}

				p_buf_mem+=4;
				p_buf_bitmap +=4;
			}
			vi--;
			offset += lw;
			offset2-=lw;
		}
		UnlockBuffer();
	  }
	  else
	  {
		  LockBuffer();
		  kn_byte* pBitmapBuffer = GetBitmapBuffer();
		  // 全屏渲染
		  int iRet = SetDIBitsToDevice(m_mem_dc,
			  x, y,
			  w, h,
			  0, 0,
			  y, h,
			  pBitmapBuffer,
			  m_p_bmi,
			  DIB_RGB_COLORS);

		  UnlockBuffer();
	  }

		RECT rc;
		GetWindowRect(m_hwnd, &rc);
		POINT ptDest ={rc.left,rc.top};
		SIZE  sizeDest = {rc.right - rc.left, rc.bottom - rc.top };

		POINT ptSrc = {0,0};
		UpdateLayeredWindow( m_hwnd,m_hdc,&ptDest,&sizeDest,m_mem_dc,&ptSrc,0,&m_blf,ULW_ALPHA);

	}
}

HDC REGLLayerWinDeviceSurface::getMemDC()
{
	return m_mem_dc;
}

#endif  //WIN32

#ifdef ANDROID_NDK


REAndroidSurface::REAndroidSurface(char* buff, kn_int width, kn_int height, REBitmap::Config colorFormat)
{

	m_pixels = buff;
	Initialize(width, height, colorFormat);
}

// 析构函数
REAndroidSurface::~REAndroidSurface()
{

}

void CallJavaRequestRender();
void LockJavaBitmap();
void UnlockJavaBitmap();

// 绘制上屏
void REAndroidSurface::Flip(LSTRECT &lst_rect)
{

	LockBuffer();

	LOGI ("REAndroidSurface::Flip");
	kn_byte* pBitmapBuffer = GetBitmapBuffer();
	if (lst_rect.empty())
	{
		LockJavaBitmap();
		// 全屏渲染
		memcpy(m_pixels, pBitmapBuffer, m_pBitmap->getSize());

		UnlockJavaBitmap();
	}

	UnlockBuffer();

	CallJavaRequestRender();

}

void REAndroidSurface::SetDstPointer(char* pBuf)
{
	m_pixels = pBuf;
}

#endif  // ANDROID_NDK



#ifdef QT_HMI


REQtSoftSurface::REQtSoftSurface(void* p, kn_int width, kn_int height, REBitmap::Config colorFormat)
{
    m_label = p;
    m_image = new QImage(width, height, QImage::Format_RGB32);

    Initialize(width, height, colorFormat);
}


REQtSoftSurface::~REQtSoftSurface()
{
    SAFE_DELETE(m_image);
}

// 绘制上屏
void REQtSoftSurface::Flip(LSTRECT &lst_rect)
{


    if(m_label == NULL)
    {
        return;
    }

    char* pDst = m_image->bits();


    LockBuffer();

    kn_byte* pBitmapBuffer = GetBitmapBuffer();
    if (lst_rect.empty())
    {
        // 全屏渲染
        memcpy(pDst, pBitmapBuffer, m_pBitmap->getSize());
    }

    UnlockBuffer();

//    static int i  = 0;
//    memset(pDst, i++ % 50 + 200, m_pBitmap->getSize());

  //  printf("REQtSoftSurface::Flip m_pBitmap size = %d\n", m_pBitmap->getSize());


   // m_image->fill(Qt::blue);


    // flip
    m_label->setPixmap(QPixmap::fromImage (*m_image));
}

void REQtSoftSurface::SetDstLabel(QLabel* pLabel)
{
    m_label = pLabel;
}


#endif  // QT_HMI


#ifdef IOS_DEV



REIOSSurface::REIOSSurface(void*& buff, kn_int width, kn_int height, REBitmap::Config colorFormat)
{
    Initialize(width, height, colorFormat);
    m_bGLFilp = true;
    m_pTexBuf = NULL;
    
    if(m_bGLFilp)
    {
        m_pTexBuf = new unsigned char [m_pBitmap->getSize()];
    }
    else
    {
        m_cgimage = SkCreateCGImageRef(*m_pBitmap);
        buff = (void*)m_cgimage;
    }
}

// 析构函数
REIOSSurface::~REIOSSurface()
{
    if(m_bGLFilp)
    {
        SAFE_DELETE_GROUP(m_pTexBuf);
        
    }
    else
    {
        CGImageRelease(m_cgimage);
    }

}


extern void IOSFlip(CGImageRef, unsigned char* pTexBuf);

// 绘制上屏
void REIOSSurface::Flip(LSTRECT &lst_rect)
{

    if(m_bGLFilp)
    {
        
        kn_int width = GetWidth();
		kn_int height = GetHeight();
        
        kn_int iRowBytes = width * 4;
        
        kn_byte* pSrc = GetBitmapBuffer() + m_pBitmap->getSize() - iRowBytes;
        kn_byte* pDst = m_pTexBuf;
        
        for(int i = 0; i < height; i++)
        {
            memcpy(pDst, pSrc, iRowBytes);
            pSrc -= iRowBytes;
            pDst += iRowBytes;
            
        }
    }
    else
    {
        
        m_cgimage = SkCreateCGImageRef(*m_pBitmap);

    }
    
    IOSFlip(m_cgimage, m_pTexBuf);
 
    
}

void REIOSSurface::SetDstPointer(char* pBuf)
{

}

CGImageRef REIOSSurface::GetCGImageRef()
{
    return m_cgimage;
}
    


#endif
