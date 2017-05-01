//  **************************************
//  File:        SkiaRESurface.h
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
#ifndef RESurface_DEFINED
#define RESurface_DEFINED

#include "renderingengine.h"

using namespace skia_surface;
class NUI_API RESurface: public IRESurface
{
public:
	// 构造函数函数
	RESurface();

	// 析构函数
	virtual ~RESurface();

	//创建一个空surface
	virtual kn_bool Initialize(kn_dword width, kn_dword height, REBitmap::Config colorFormat);
	//通过外部指定内存创建surface
	virtual kn_bool Initialize(void* p, kn_dword width, kn_dword height, REBitmap::Config colorFormat);

	//从文件创建一个surface
	virtual kn_bool InitializeFromFile(const kn_string& filename);

	//从文件创建一个缩略图surface
	virtual kn_bool InitializeThumbnailFromFile(const kn_string& filename, int width, int height, int scalemode, REColor bkgcolor);

	// 从内存图片建立surface
	virtual kn_bool InitializeFromPicBuffer(const kn_byte* pBuf, kn_int iBufLen);

	// 资源释放函数
	virtual void Release();

	// 查询是否成功初始化, 初始化失败时, 以下操作无效.
	virtual kn_bool IsInitialized();

	// 取得BitmapBuffer
	virtual kn_byte* GetBitmapBuffer() const;

	// 取得BitmapBufferSize
	virtual size_t GetBitmapBufferSize() const;

	// 取得Bitmap指针
	virtual REBitmap* GetBitmap() const;

	// 取得Canvas指针
	virtual RECanvas* GetCanvas() const;

	// 确保BitmapBuffer指针指向真正的内存地址
	virtual void LockBuffer();
	virtual void UnlockBuffer();

	// 取得宽度,像素单位.
	virtual kn_dword GetWidth() const;

	// 取得高度,像素单位
	virtual kn_dword GetHeight() const;

	// 取得每像素字节
	virtual kn_dword GetXPitch() const;

	// 取得每行字节
	virtual kn_dword GetYPitch() const;

	// 取得颜色格式
	virtual REBitmap::Config GetColorFormat() const;

	// 绘制背景
	virtual void DrawColor(REColor color, SkXfermode::Mode mode = SkXfermode::kSrcOver_Mode);

	virtual void Clear(REColor );
	// 绘制点
	virtual void DrawPoint(REScalar x, REScalar y, REColor color);
	virtual void DrawPoint(REScalar x, REScalar y, const REPaint& paint);

	// 绘制直线
	virtual void DrawLine(REScalar x1, REScalar y1, REScalar x2, REScalar y2, const REPaint& paint);

	// 绘制一系列线段
	virtual void DrawLines(const REPoint* pPoint, size_t count, const REPaint& paint);

	// 绘制DashLine
	// 用法: pPoint&count确定描绘的轨迹, pIntervals&interCount确定间距, phase确定起始间距
	// 例如, 假如描绘黑白相间的铁路线, 则可以设定pIntervals = {20, 20}, 颜色为黑色
	// 假如描绘dot dash line, 则可以设定pIntervals = {30, 20, 5, 20}, 表示绘制线段长为30, 间隔20, 然后绘制dot为5, 再间隔20, 再绘制线段
	// phase为起始间距, 实际值为phase mod (pIntervals所有值之和), 假如pIntervals = {20, 20}, 则phase取值-5, 35, 75, 效果一样
	virtual void DrawDashLine(const REPoint* pPoint, size_t count, const REScalar* pIntervals, size_t interCount,  REScalar phase, const REPaint& paint);

	// 绘制矩形
	virtual void DrawRect(const RERect& rct, const REPaint& paint);

	// 绘制多边形
	virtual void DrawPolygon(const REPoint* pPoint, size_t count, kn_bool bClose, const REPaint& paint );

	// 多点绘制
	virtual void DrawPonits(SkCanvas::PointMode mode, size_t count, const REPoint pts[], const REPaint& paint );

	// 绘制圆
	virtual void DrawCircle(REScalar x, REScalar y, REScalar radius, const REPaint& paint);

	virtual void DrawArc(RERect& oval, SkScalar startAngle, SkScalar sweepAngle, bool useCenter, SkPaint& paint);

	// 绘制位图, 将整张源图绘制到目标图上(left(), top())位置
	virtual kn_bool DrawBitmap(const IRESurface* pSourceRE, 
		REScalar left, 
		REScalar top, 
		const REPaint* pPaint);

	// 以变换矩阵的方式绘制位图
	virtual bool DrawBitmapMatrix(const IRESurface* pSourceRE, const REMatrix& m, const REPaint* pPaint);

	// 绘制位图, 将整张源图的部分srcRect绘制到目标图上(left(), top())位置
	virtual kn_bool DrawPartialBitmap(const IRESurface* pSourceRE, 
		RERect srcRect,
		REScalar destLeft, 
		REScalar destTop, 
		const REPaint* pPaint);

	virtual void drawBitmapNine(const IRESurface* bitmap, const RERect& center, const RERect& dst, const SkPaint* paint = NULL);

	// 绘制位图, 支持拉伸
	virtual kn_bool DrawBitmapRectToRect(const IRESurface* pSourceRE, 
		const RERect* pSourceRect, 
		const RERect& destRect, 
		const REPaint* pPaint);
	//指定位图以指定填充方式绘制到画布
	virtual void DrawPaint(const REPaint& pPaint);
	// 绘制文字
	virtual void DrawText(const void* text, size_t byteLength, REScalar x,
		REScalar y, const REPaint& paint);
	virtual void DrawText(const kn_string& text, REScalar x,
		REScalar y, const REPaint& paint);
	// 沿轨迹绘制文字
	    /** Draw the text, with origin at (x,y), using the specified paint, along
        the specified path. The paint's Align setting determins where along the
        path to start the text.
        @param path         The path the text should follow for its baseline
