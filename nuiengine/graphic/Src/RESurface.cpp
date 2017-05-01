//  **************************************
//  File:        RESurface.cpp
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
#include "SkImageDecoder.h"
#include "SkStream.h"
#include "REPath.h"
#include "SkiaRESurface.h"
#include "PathEffect.h"
#include "CharSet.h"
#include "FilePath.h"

RESurface::RESurface()
	:m_pBitmap(0), m_pCanvas(0)
{
}

RESurface::~RESurface()
{
	Release();
}

kn_bool RESurface::Initialize( kn_dword width, kn_dword height, REBitmap::Config format )
{
	kn_bool ret = false;
	if (format != SkBitmap::kNo_Config)
	{
		m_pBitmap = new REBitmap;
		m_pBitmap->setConfig(format, width, height);
		m_pBitmap->allocPixels();
		m_pCanvas = new RECanvas(*m_pBitmap);
		ret = true;
	}
	return ret;
}

kn_bool RESurface::Initialize(void* p, kn_dword width, kn_dword height, REBitmap::Config format )
{
	kn_bool ret = false;
	if (format != SkBitmap::kNo_Config)
	{
		m_pBitmap = new REBitmap;
		m_pBitmap->setConfig(format, width, height);
		m_pBitmap->setPixels(p);
		m_pCanvas = new RECanvas(*m_pBitmap);
		ret = true;
	}
	return ret;
}

void RESurface::Release()
{
	SAFE_DELETE(m_pCanvas)
	SAFE_DELETE(m_pBitmap)
}

kn_dword RESurface::GetWidth() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->width();
	}
	return 0;
}

kn_dword RESurface::GetHeight() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->height();
	}
	return 0;
}

kn_dword RESurface::GetXPitch() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->bytesPerPixel();
	}
	return 0;
}

kn_dword RESurface::GetYPitch() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->rowBytes();
	}
	return 0;
}

REBitmap::Config RESurface::GetColorFormat() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->getConfig();
	}
	return REBitmap::kNo_Config;
}

void RESurface::DrawPoint( REScalar x, REScalar y, REColor color )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawPoint(x, y, color);
	}
}

void RESurface::DrawPoint( REScalar x, REScalar y, const REPaint& paint )
{
	if (m_pCanvas)
	{
        SkPoint point = SkPoint::Make(x, y);
		m_pCanvas->drawPoints(SkCanvas::kPoints_PointMode, 1, &point, paint);
	}
}

void RESurface::DrawLine( REScalar x1, REScalar y1, REScalar x2, REScalar y2, const REPaint& paint )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawLine(x1, y1,	x2, y2,	paint);
	}
}

void RESurface::DrawRect( const RERect& rct, const REPaint& paint )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawRect(rct, paint);
	}
}

void RESurface::DrawCircle( REScalar x, REScalar y, REScalar radius, const REPaint& paint )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawCircle(x,  y, 	radius, 	paint);
	}
}

void RESurface::DrawArc(RERect& oval, SkScalar startAngle, SkScalar sweepAngle, bool useCenter, SkPaint& paint)
{
	if (m_pCanvas)
	{
		m_pCanvas->drawArc(oval,  startAngle, sweepAngle, useCenter, paint);
	}
}

bool RESurface::DrawBitmap(const IRESurface* pSourceRE, REScalar left, REScalar top, const REPaint* pPaint)
{
	if (m_pCanvas && pSourceRE && pSourceRE->GetBitmap())
	{
		m_pCanvas->drawBitmap(*pSourceRE->GetBitmap(), left, top,  pPaint);
		return true;
	}
	return false;
}

bool RESurface::DrawPartialBitmap( const IRESurface* pSourceRE, 
	RERect srcRect,
	REScalar destLeft, 
	REScalar destTop, 
	const REPaint* pPaint )
{
	RERect rectClip = RERect::MakeXYWH(destLeft, destTop, srcRect.width(), srcRect.height());
	// 重设目标左上角坐标
	REScalar xDst = destLeft - srcRect.left();
	REScalar yDst = destTop - srcRect.top();

	Save();
	ClipRect(rectClip);
	bool ret = DrawBitmap(pSourceRE, xDst, yDst, pPaint);

	Restore();

	return ret;
}

bool RESurface::DrawBitmapRectToRect(const IRESurface* pSourceRE, const RERect* pSourceRect, const RERect& destRect, const REPaint* pPaint)
{
	if (pSourceRE && pSourceRE->GetBitmap() && pSourceRect)
	{
			m_pCanvas->drawBitmapRectToRect(*pSourceRE->GetBitmap(), pSourceRect, destRect, pPaint);
			return true;
	}
	return false;
}

void RESurface::drawBitmapNine(const IRESurface* pSourceRE, const RERect& center, const RERect& dst, const SkPaint* paint )
{
	if (pSourceRE && pSourceRE->GetBitmap() )
	{
		SkIRect ir;
		ir.fLeft = center.left();
		ir.fTop = center.top();
		ir.fRight = center.right();
		ir.fBottom = center.bottom();
		m_pCanvas->drawBitmapNine(*pSourceRE->GetBitmap(), ir, dst, paint);
	}
}

void RESurface::DrawPaint(const REPaint& pPaint)
{

	m_pCanvas->drawPaint(pPaint);
}
kn_byte* RESurface::GetBitmapBuffer() const
{
	if (m_pBitmap)
	{
		return (kn_byte*)m_pBitmap->getPixels();
	}
	return 0;
}

void RESurface::LockBuffer()
{
	if (m_pBitmap)
	{
		m_pBitmap->lockPixels();
	}
}

void RESurface::UnlockBuffer()
{
	if (m_pBitmap)
	{
		m_pBitmap->unlockPixels();
	}
}

REColor RESurface::getColor(int x, int y)
{
	REColor c;
	if (m_pBitmap)
	{
		c = m_pBitmap->getColor(x,y);
	}
	return c;
}

size_t RESurface::GetBitmapBufferSize() const
{
	if (m_pBitmap)
	{
		return m_pBitmap->getSize();
	}
	return 0;
}

kn_bool RESurface::InitializeFromFile( const kn_string& filename )
{
	kn_string strFullFile = GetCurrentPath();
	strFullFile += filename;

	m_pBitmap = new SkBitmap;
#ifdef UNICODE
	char path[MAX_PATH] = {0};
	WideCharToMultiByte(CP_ACP, 0, strFullFile.c_str(), strFullFile.length(), path, MAX_PATH, 0, 0);
	kn_bool ret = SkImageDecoder::DecodeFile(path, m_pBitmap);

	if(!ret)
	{
		LOGE ("RESurface::InitializeFromFile ERROR %s", path);
	}
#else
	kn_bool ret = SkImageDecoder::DecodeFile(filename.c_str(), m_pBitmap);
#endif
	 

	if (ret)
	{
		m_pCanvas = new RECanvas(*m_pBitmap);
	}
	else
	{

		SAFE_DELETE(m_pBitmap)

	}

	return ret;
}

// 从图片内存(png/...)建立surface
kn_bool RESurface::InitializeFromPicBuffer(const kn_byte* pBuf, kn_int iBufLen)
{
	m_pBitmap = new SkBitmap;
  	kn_bool ret =  SkImageDecoder::DecodeMemory(pBuf, iBufLen, m_pBitmap);
 
	 
	if (ret)
	{
		m_pCanvas = new RECanvas(*m_pBitmap);
	}
	else
	{
		SAFE_DELETE(m_pBitmap)
	}

	return ret;
}



// 载入缩略图
kn_bool RESurface::InitializeThumbnailFromFile(const kn_string& filename, int width, int height, int scalemode, REColor bkgcolor)
{
	// 原图
	SkBitmap bpOriginal;

	// SkImageDecoder::Factory(stream)  stream参数不起作用
	//	加载原图，windows 下setSampleSize不起作用, 	SkImageDecoder_WIC.cpp 未使用SkScaledBitmapSampler
#ifdef UNICODE
	char path[512] = {0};
	WideCharToMultiByte(CP_ACP, 0, filename.c_str(), filename.length(), path, 512, 0, 0);
	kn_bool ret = SkImageDecoder::DecodeFile(path, &bpOriginal);
#else
	kn_bool ret = SkImageDecoder::DecodeFile(filename.c_str(), &bpOriginal);
#endif

	if(!ret)
	{
		return false;
	}


	m_pBitmap = new SkBitmap;
	m_pBitmap->setConfig(bpOriginal.getConfig(), width, height);		
	m_pBitmap->allocPixels();

	m_pCanvas = new RECanvas(*m_pBitmap);

	// 绘制底色
	m_pCanvas->drawColor(bkgcolor);

	// 小于目标的不缩放
	bool bScale = false;

	SkRect rectOriginal = SkRect::MakeWH(bpOriginal.width(), bpOriginal.height());
	SkRect rectNew = SkRect::MakeXYWH(0, 0, width, height);

	if(scalemode == RE_FIT_WIDTH)
	{
		// 按宽度处理
		if(bpOriginal.width() > width)
		{
			bScale = true;

			int iNewWidth = width;
			int iNewHeight = iNewWidth * bpOriginal.height() /  bpOriginal.width(); 
			rectNew = SkRect::MakeXYWH(0, (height - iNewHeight) / 2,  iNewWidth, iNewHeight);
		}


	}
	else if(scalemode == RE_FIT_RECT)
	{
		// 适合宽度和高度
		if(bpOriginal.width() > width || bpOriginal.height() > height)
		{
			bScale = true;

			float factorW =  (float)width / bpOriginal.width();
			float factorH =  (float)height / bpOriginal.height();

			float factor = min (factorW, factorH);

			float fNewWidth =  bpOriginal.width()  * factor;
			float fNewHeight =  bpOriginal.height()  * factor;

			float fX = (width - fNewWidth) / 2;
			float fY = (height - fNewHeight) / 2;

			rectNew = SkRect::MakeXYWH(fX, fY, fNewWidth, fNewHeight);

		}

	}


	if(bScale)
	{
		SkPaint paint;
		paint.setAntiAlias(true);
		paint.setFilterBitmap(true);
		m_pCanvas->drawBitmapRectToRect (bpOriginal, &rectOriginal, rectNew, &paint);
	}
	else
	{
		// 不缩放
		int x = (width - bpOriginal.width()) / 2;
		int y = (height - bpOriginal.height()) / 2;
		m_pCanvas->drawBitmap(bpOriginal, x, y);
	}

	

	return ret;
}


void RESurface::DrawPolygon( const REPoint* pPoint, size_t count, kn_bool bClose, const REPaint& paint )
{
	if (m_pCanvas && pPoint && count)
	{
		SkPath path;
		path.addPoly(pPoint, count, bClose);
		m_pCanvas->drawPath(path, paint);
	}
}

void RESurface::DrawColor( REColor color ,  SkXfermode::Mode mode)
{
	if (m_pCanvas)
	{
		m_pCanvas->drawColor(color , mode);
	}
}

void RESurface::Clear( REColor color)
{
	if (m_pCanvas)
	{
		m_pCanvas->clear(color);
	}
}

void RESurface::DrawText( const void* text, size_t byteLength, REScalar x, REScalar y, const REPaint& paint )
{
	if (m_pCanvas && text && byteLength)
	{
		m_pCanvas->drawText(text, byteLength, x, y, paint);
	}
}

void RESurface::DrawText( const kn_string& text, REScalar x, REScalar y, const REPaint& paint )
{
	DrawText(text.c_str(), text.size() * sizeof(kn_char), x, y, paint);
}

kn_bool RESurface::IsInitialized()
{
	return m_pCanvas != 0 && m_pBitmap != 0;
}


int RESurface::Save()
{
	if (m_pCanvas)
	{
		return m_pCanvas->save();
	}
	return 0;
}

void RESurface::Restore()
{
	if (m_pCanvas)
	{
		m_pCanvas->restore();
	}
}

// 裁剪矩形
bool RESurface::ClipRect(const RERect& rect, RERegion::Op op, bool doAntiAlias)
{
	if (m_pCanvas)
	{
		return m_pCanvas->clipRect(rect, op, doAntiAlias);
	}
	return false;
}

bool RESurface::WriteToFile(const kn_string& path, REImageEncoder::Type type, kn_int quality)
{
	kn_bool ret = false;
	if (m_pBitmap)
	{
#ifdef UNICODE
		char pathbuf[512] = {0};
		wcstombs(pathbuf, path.c_str(), path.length());
		ret = SkImageEncoder::EncodeFile(pathbuf, *m_pBitmap, type, quality);
#else
		ret = SkImageEncoder::EncodeFile(pathbuf, *m_pBitmap, type, quality);
#endif
	}
	return ret;
}

// 绘制圆角矩形
void RESurface::DrawRoundRect(const RERect& rect, REScalar rx, REScalar ry, const REPaint& paint)
{
	if (m_pCanvas)
	{
		m_pCanvas->drawRoundRect(rect, rx, ry, paint);
	}
}

// 绘制椭圆
void RESurface::DrawOval(const RERect& oval, const REPaint& paint)
{
	if (m_pCanvas)
	{
		m_pCanvas->drawOval(oval, paint);
	}
}

// 平移
bool RESurface::Translate(REScalar dx, REScalar dy)
{
	if (m_pCanvas)
	{
		return m_pCanvas->translate(dx, dy);
	}
	return false;
}

// 缩放
bool RESurface::Scale(REScalar sx, REScalar sy)
{
	if (m_pCanvas)
	{
		return m_pCanvas->scale(sx, sy);
	}
	return false;
}

// 旋转
bool RESurface::Rotate(REScalar degrees)
{
	if (m_pCanvas)
	{
		return m_pCanvas->rotate(degrees);
	}
	return false;
}

// 倾斜
bool RESurface::Skew(REScalar sx, REScalar sy)
{
	if (m_pCanvas)
	{
		return m_pCanvas->skew(sx, sy);
	}
	return false;
}

// 还原单位矩阵
void RESurface::ResetMatrix()
{
	if (m_pCanvas)
	{
		m_pCanvas->resetMatrix();
	}
}

bool RESurface::DrawBitmapMatrix( const IRESurface* pSourceRE, const REMatrix& m, const REPaint* pPaint )
{
	if (m_pCanvas && pSourceRE && pSourceRE->GetBitmap())
	{
		m_pCanvas->drawBitmapMatrix(*pSourceRE->GetBitmap(), m, pPaint);
		return true;
	}
	return false;
}

REMatrix RESurface::GetTotalMatrix()
{
	if (m_pCanvas)
	{
		return m_pCanvas->getTotalMatrix();
	}
	return REMatrix();
}

void RESurface::SetMatrix( const REMatrix& m )
{
	if (m_pCanvas)
	{
		m_pCanvas->setMatrix(m);
	}
}

void RESurface::Concat( const REMatrix& m )
{
	if (m_pCanvas)
	{
		m_pCanvas->concat(m);
	}
}

void RESurface::DrawPonits( SkCanvas::PointMode mode, size_t count, const REPoint pts[], const REPaint& paint )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawPoints(mode, count, pts, paint);
	}
}

void RESurface::DrawLines( const REPoint* pPoint, size_t count, const REPaint& paint )
{
	if (m_pCanvas && pPoint && count)
	{
		REPath path;
		path.addPoly(pPoint, count, false);
		m_pCanvas->drawPath(path, paint);
	}
}

void RESurface::DrawDashLine( const REPoint* pPoint, size_t count, const REScalar* pIntervals, size_t interCount, REScalar phase, const REPaint& paint )
{
	if (m_pCanvas && pPoint && count && pIntervals && interCount)
	{
		REPath path;
		path.addPoly(pPoint, count, false);
		REPaint temp = paint;
		temp.setPathEffect(new SkDashPathEffect(pIntervals, interCount, phase))->unref();
		m_pCanvas->drawPath(path, temp);
	}
}

REBitmap* RESurface::GetBitmap() const
{
	return m_pBitmap;
}

RECanvas* RESurface::GetCanvas() const
{
	return m_pCanvas;
}

void RESurface::DrawPath( const REPath& path, const REPaint& paint )
{
	if (m_pCanvas)
	{
		m_pCanvas->drawPath(path, paint);
	}
}

void RESurface::DrawTextOnPath( const kn_string& text, size_t byteLength, const REPath& path, const REMatrix* matrix, const REPaint& paint )
{
	if (m_pCanvas && !text.empty())
	{
		m_pCanvas->drawTextOnPath(text.c_str(), text.size() * sizeof(kn_char), path, matrix, paint);
	}
}

void RESurface::DrawTextOnPathHV( const kn_string& text, const REPath& path, REScalar hOffset, REScalar vOffset, const REPaint& paint )
{
	if (m_pCanvas && !text.empty())
	{
		m_pCanvas->drawTextOnPathHV(text.c_str(), text.size() * sizeof(kn_char), path, hOffset, vOffset, paint);
	}
}

void RESurface::preDraw()
{

}
