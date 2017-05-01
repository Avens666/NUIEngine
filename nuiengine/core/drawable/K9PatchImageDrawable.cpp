//  **************************************
//  File:        K9PatchImageDrawable.cpp
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
#include "K9PatchImageDrawable.h"

K9PatchImageDrawable::K9PatchImageDrawable()
{
	Initialize();
}

K9PatchImageDrawable::K9PatchImageDrawable(const kn_string& strFile)
{
	Initialize();
	kn_bool bOK = CreateFromFile(strFile);
	GetSrcRect();  //取得原图的每个区域的rect

	ASSERT(bOK);
}

K9PatchImageDrawable::K9PatchImageDrawable(IRESurface* pSurface,  kn_bool b_out )
{
	Initialize();
	CreateFromSurface(pSurface, b_out);
	GetSrcRect();  //取得原图的每个区域的rect

}

K9PatchImageDrawable::~K9PatchImageDrawable(void)
{
	m_Row = 0;
	m_Col = 0;
	m_DesRect.clear();
	m_SrcRect.clear();
	Release();
}

void K9PatchImageDrawable::Initialize()
{
	KImageDrawable::Initialize();
	m_Row = 0;
	m_Col = 0;
	m_DesRect.clear();
	m_SrcRect.clear();

	m_type = KDrawable::KDRAWABLE_9PATCH_IMAGE;

	setAutoSize(TRUE);
}

typedef struct stLINE_
{
    int x1;
    int x2;
    stLINE_()
    {
        x1 = x2 = 0;
    }
    stLINE_(int a,int b)
    {
        x1 = a;
        x2 = b;
    }
}stLINE;

void K9PatchImageDrawable::GetSrcRect()
{
	m_SrcRect.clear();



	vector<stLINE> xLine;
	vector<stLINE> yLine;

	kn_byte* pBuff = m_p_surface->GetBitmapBuffer();
	int xpitch = m_p_surface->GetXPitch();
	int ypitch = m_p_surface->GetYPitch();
	int x1 = 1;
	int x2 = 1;
	int pw=m_p_surface->GetWidth();
	int ph=m_p_surface->GetHeight();
	bool bBlackLine = false;
	
	//横向扫描第一行
	for(int i = 1; i < pw;i++)
	{
		//如果为黑色点
		int addr = i*xpitch;
		if(pBuff[addr] == 0 && pBuff[addr+1] == 0 && pBuff[addr+2] == 0 && pBuff[addr+3] != 0)
		{
			if(!bBlackLine)
			{
				xLine.push_back(stLINE(x1,x2 - 1));  //保存上一段线
				x1 = x2;
			}
			bBlackLine = true;
		}
		else
		{
			if(bBlackLine)
			{
				xLine.push_back(stLINE(x1,x2 - 1)); //保存上一段线
				x1 = x2;
			}
			bBlackLine = false;
		}

		x2++;
	}
	xLine.push_back(stLINE(x1,x2 )); //要+1 for循环最后一个x2没有+1  zhic

	//纵向扫描第一行
	x1 = 1;
	x2 = 1;
	bBlackLine = false;
	for(int i = 1; i < ph;i++)
	{
		//如果为黑色点
		int addr = i*ypitch;
		if(pBuff[addr] == 0 && pBuff[addr+1] == 0 && pBuff[addr+2] == 0 && pBuff[addr+3] != 0)
		{
			if(!bBlackLine)
			{
				yLine.push_back(stLINE(x1,x2 - 1));
				x1 = x2;
			}
			bBlackLine = true;
		}
		else
		{
			if(bBlackLine)
			{
				yLine.push_back(stLINE(x1,x2 - 1));
				x1 = x2;
			}
			bBlackLine = false;
		}

		x2++;
	}
	yLine.push_back(stLINE(x1,x2));


	//根据横轴和纵轴的黑线，算出原图的rect区域
	m_Row = yLine.size();
	m_Col = xLine.size();
	for(int i = 0; i < m_Row; i++)
	{
		for(int j = 0; j < m_Col; j++)
		{
			m_SrcRect.push_back(RERect::MakeXYWH(xLine[j].x1, yLine[i].x1, xLine[j].x2-xLine[j].x1, yLine[i].x2-yLine[i].x1));
		}
	}
}

void K9PatchImageDrawable::GetDesRect()
{
	m_DesRect.clear();

	float xScale = 1.0f;
	float yScale = 1.0f;

	int fix_w=0;   // 固定区域的宽高
	int fix_h=0;

	int zoom_w=0;   // 可缩放区域的宽高
	int zoom_h=0;
	for(int i = 0; i < m_Col; i++)
	{
		if((i & 0x01) == 0)
		{
			fix_w += m_SrcRect[i].width();  // 计算原图中不缩放的区域的宽（总和）
		}
		else
		{
			zoom_w += m_SrcRect[i].width(); // 计算原图中缩放的区域的宽（总和）
		}
	}

	for(int j = 0; j < m_Row; j++)
	{
		if((j & 0x01) == 0)
		{
			fix_h += m_SrcRect[j*m_Col].height(); // 计算原图中不缩放的区域的高（总和）
		}
		else
		{
			zoom_h += m_SrcRect[j*m_Col].height();// 计算原图中缩放的区域的高（总和）
		}
	}


	//if(fix_w > m_rectScaleBound.width() || fix_h > m_rectScaleBound.height())
	//{
	//	//如果不缩放的区域比目标区域小，则使用普通缩放
	//	xScale = 1.0f;
	//	yScale = 1.0f;

	//	m_Row = 1;
	//	m_Col = 1;

	//	m_SrcRect.clear();
	//	int W = m_p_surface->GetWidth();
	//	int H = m_p_surface->GetHeight();
	//	m_SrcRect.push_back(RERect::MakeXYWH(1, 1, W, H));
	//	m_DesRect.push_back(RERect::MakeXYWH(0, 0, m_rectScaleBound.width(), m_rectScaleBound.height()));
	//	return;
	//}
	//else
	{
		//计算出，缩放的比例
		xScale = (float)(m_rectScaleBound.width() - fix_w) / zoom_w;
		yScale = (float)(m_rectScaleBound.height() - fix_h) / zoom_h;

		//循环计算每一个目标rect
		bool xzoom = false;  //默认从不缩放开始
		bool yzoom = false;
		float x = 0;//m_SrcRect[0].fLeft;  //rect左上角的点
		float y = 0;//m_SrcRect[0].fTop;   //rect左上角的点
		for(int i = 0; i < m_Row; i++)
		{
			x = 0;//m_SrcRect[0].fLeft;
			xzoom = false;
			for(int j = 0; j < m_Col; j++)
			{
				RERect& rt = m_SrcRect[j + i * m_Col];
				if(xzoom && yzoom)  //x和Y都缩放
				{
					RERect trt = RERect::MakeXYWH(x, y, rt.width() * xScale, rt.height() * yScale);
					x += rt.width() * xScale;  // rect左上角的点x坐标，需要加上宽度
					m_DesRect.push_back(trt);				
				}
				else if(xzoom)  //x缩放
				{
					RERect trt = RERect::MakeXYWH(x, y, rt.width() * xScale, rt.height());
					x += rt.width() * xScale;
					m_DesRect.push_back(trt);
				}
				else if(yzoom) //Y缩放
				{
					RERect trt = RERect::MakeXYWH(x, y, rt.width(), rt.height() * yScale);
					x += rt.width();
					m_DesRect.push_back(trt);
				}
				else
				{
					RERect trt = RERect::MakeXYWH(x, y, rt.width(), rt.height());
					x += rt.width();
					m_DesRect.push_back(trt);
				}

				xzoom = !xzoom;  //改变是否缩放的状态
			}

			if(yzoom)
			{
				y += m_SrcRect[i * m_Col].height() * yScale;  // rect左上角的点y坐标，需要加上高度
			}
			else
			{
				y += m_SrcRect[i * m_Col].height();  // rect左上角的点y坐标，需要加上高度
			}

			yzoom = !yzoom;  //改变是否缩放的状态
		}
	}
}

void K9PatchImageDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 
	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}


	if(m_p_surface == NULL)
	{
		EndDraw(pDstSurface, pMatrix);
		return ;
	}
	int iTarX = iDstX + m_rect.left();
	int iTarY = iDstY + m_rect.top();

	m_rectScaleBound = m_rect;
	RERect rectDst = RERect::MakeXYWH(iTarX, iTarY, m_rectScaleBound.width(), m_rectScaleBound.height());
	pDstSurface->ClipRect(rectDst);


	GetDesRect();  //取得目标图的每个区域的rect


	//if (m_Row == 3 && m_Col == 3)
	//{
	//	RERect r_dst;
	//	r_dst.fLeft = m_DesRect[0].left() + iTarX;
	//	r_dst.fTop = m_DesRect[0].top() + iTarY;
	//	r_dst.fRight = m_DesRect[8].right() + iTarX;
	//	r_dst.fBottom = m_DesRect[8].bottom()+ iTarY;


	//	pDstSurface->drawBitmapNine( m_p_surface, m_SrcRect[4], r_dst, &m_paint );
	//}
	//else
	{
		//循环绘制每个区域
		for(int i = 0; i < m_Row; i++)
		{
			for(int j = 0; j < m_Col; j++)
			{
				RERect rectDst1 = m_DesRect[i*m_Col+j];
				rectDst1.fLeft += iTarX;
				rectDst1.fTop += iTarY;
				rectDst1.fRight += iTarX;
				rectDst1.fBottom += iTarY;

				RERect rectSrc1 = m_SrcRect[i*m_Col+j];
				pDstSurface->DrawBitmapRectToRect(m_p_surface, &rectSrc1, rectDst1, &m_paint);
			}
		}

	}

	EndDraw(pDstSurface, pMatrix);
}

