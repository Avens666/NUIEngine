//  **************************************
//  File:        renderingengine.h
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

#ifndef RenderingEngine_DEFINED
#define RenderingEngine_DEFINED

#include "NE_pubhead.h"
#include "REScalar.h"
#include "REPoint.h"
#include "RERect.h"
#include "REPath.h"
#include "RETypeface.h"
#include "REPaint.h"
#include "REMatrix.h"
#include "REColor.h"
#include "RERegion.h"
#include "RECanvas.h"
#include "REBitmap.h"
#include "ImageCoder.h"

namespace skia_surface
{


class IRESurface
{
public:
	// 析构函数
	virtual ~IRESurface() {};

	//创建一个空surface
	virtual kn_bool Initialize(kn_dword width, kn_dword height, REBitmap::Config colorFormat) = 0;
	//通过外部指定内存创建surface
	virtual kn_bool Initialize(void* p, kn_dword width, kn_dword height, REBitmap::Config colorFormat) = 0;

	//从文件创建一个surface
	virtual kn_bool InitializeFromFile(const kn_string& filename) = 0;

	//  从文件创建一个缩略图surface
	virtual kn_bool InitializeThumbnailFromFile(const kn_string& filename, int width, int height, int scalemode, REColor bkgcolor) = 0;

	// 从内存图片建立surface
	virtual kn_bool InitializeFromPicBuffer(const kn_byte* pBuf, kn_int iBufLen) = 0;

	// 资源释放函数
	virtual void Release() = 0;

	// 查询是否成功初始化, 初始化失败时, 以下操作无效.
	virtual kn_bool IsInitialized() = 0;

	// 取得BitmapBuffer
	virtual kn_byte* GetBitmapBuffer() const = 0;

	// 取得BitmapBufferSize
	virtual size_t GetBitmapBufferSize() const = 0;

	// 取得Bitmap指针
	virtual REBitmap* GetBitmap() const = 0;

	// 取得Canvas指针
	virtual RECanvas* GetCanvas() const = 0;

	// 确保BitmapBuffer指针指向真正的内存地址
	virtual void LockBuffer() = 0;
	virtual void UnlockBuffer() = 0;

	// 取得宽度,像素单位.
	virtual kn_dword GetWidth() const = 0;

	// 取得高度,像素单位
	virtual kn_dword GetHeight() const = 0;

	// 取得每像素字节
	virtual kn_dword GetXPitch() const = 0;

	// 取得每行字节
	virtual kn_dword GetYPitch() const = 0;

	// 取得颜色格式
	virtual REBitmap::Config GetColorFormat() const = 0;

	// 绘制背景 
	virtual void DrawColor(REColor color, SkXfermode::Mode mode = SkXfermode::kSrcOver_Mode) = 0;
	virtual void Clear(REColor ) = 0;
	// 绘制点
	virtual void DrawPoint(REScalar x, REScalar y, REColor color) = 0;
	virtual void DrawPoint(REScalar x, REScalar y, const REPaint& paint) = 0;
	
	// 绘制直线
	virtual void DrawLine(REScalar x1, REScalar y1, REScalar x2, REScalar y2, const REPaint& paint) = 0;

	// 绘制一系列线段
	virtual void DrawLines(const REPoint* pPoint, size_t count, const REPaint& paint) = 0;

	// 绘制DashLine
	// 用法: pPoint&count确定描绘的轨迹, pIntervals&interCount确定间距, phase确定起始间距
	// 例如, 假如描绘黑白相间的铁路线, 则可以设定pIntervals = {20, 20}, 颜色为黑色
	// 假如描绘dot dash line, 则可以设定pIntervals = {30, 20, 5, 20}, 表示绘制线段长为30, 间隔20, 然后绘制dot为5, 再间隔20, 再绘制线段
	// phase为起始间距, 实际值为phase mod (pIntervals所有值之和), 假如pIntervals = {20, 20}, 则phase取值-5, 35, 75, 效果一样
	virtual void DrawDashLine(const REPoint* pPoint, size_t count, const REScalar* pIntervals, size_t interCount,  REScalar phase, const REPaint& paint) = 0;

	// 绘制矩形
	virtual void DrawRect(const RERect& rct, const REPaint& paint) = 0;

	// 绘制多边形, 支持frame或者area模式
	virtual void DrawPolygon(const REPoint* pPoint, size_t count, kn_bool bClose, const REPaint& paint ) = 0;

	// 多点绘制, 支持点, 线, frame模式
	virtual void DrawPonits(SkCanvas::PointMode mode, size_t count, const REPoint pts[], const REPaint& paint ) = 0;

	// 绘制圆
	virtual void DrawCircle(REScalar x, REScalar y, REScalar radius, const REPaint& paint) = 0;
	virtual void DrawArc(RERect& oval, SkScalar startAngle, SkScalar sweepAngle, bool useCenter, SkPaint& paint) = 0;

	// 绘制位图, 将整张源图绘制到目标图上(left(), top())位置
	virtual kn_bool DrawBitmap(const IRESurface* pSourceRE, 
		REScalar left, 
		REScalar top, 
		const REPaint* pPaint = 0) = 0;

	// 以变换矩阵的方式绘制位图
	virtual bool DrawBitmapMatrix(const IRESurface* pSourceRE, const REMatrix& m, const REPaint* pPaint = 0) = 0;

	virtual void drawBitmapNine(const IRESurface* bitmap, const RERect& center, const RERect& dst, const SkPaint* paint = NULL) = 0;
	
	// 绘制位图, 将整张源图的部分srcRect绘制到目标图上(left(), top())位置
	virtual kn_bool DrawPartialBitmap(const IRESurface* pSourceRE, 
		RERect srcRect,
		REScalar destLeft, 
		REScalar destTop, 
		const REPaint* pPaint = 0) = 0;

	// 绘制位图, 支持拉伸
	virtual kn_bool DrawBitmapRectToRect(const IRESurface* pSourceRE, 
		const RERect* pSourceRect, 
		const RERect& destRect, 
		const REPaint* pPaint = 0) = 0;
	//指定位图以指定填充方式绘制到画布
	virtual void DrawPaint(const REPaint& pPaint) = 0;
	// 绘制文字
	virtual void DrawText(const void* text, size_t byteLength, REScalar x,
		REScalar y, const REPaint& paint) = 0;
		virtual void DrawText(const kn_string& text, REScalar x,
		REScalar y, const REPaint& paint) = 0;
	// 沿轨迹绘制文字
	    /** Draw the text, with origin at (x,y), using the specified paint, along
        the specified path. The paint's Align setting determins where along the
        path to start the text.
        @param path         The path the text should follow for its baseline
        @param hOffset      The distance along the path to add to the text's
                            starting position
        @param vOffset      The distance above(-) or below(+) the path to
                            position the text
        @param paint        The paint used for the text
    */
    virtual void DrawTextOnPathHV(const kn_string& text,
                          const REPath& path, REScalar hOffset,
                          REScalar vOffset, const REPaint& paint) = 0;
	/** Draw the text, with origin at (x,y), using the specified paint, along
    the specified path. The paint's Align setting determins where along the
    path to start the text.
    @param path         The path the text should follow for its baseline
    @param matrix       (may be null) Applied to the text before it is
                        mapped onto the path
    @param paint        The paint used for the text
    */
    virtual void DrawTextOnPath(const kn_string& text, size_t byteLength,
                                const REPath& path, const REMatrix* matrix,
                                const REPaint& paint) = 0;

	// 保存当前的matrix和clip
	virtual int Save() = 0;

	// 还原save时的matrix和clip
	virtual void Restore() = 0;

	// 裁剪矩形
	virtual bool ClipRect(const RERect& rect,  RERegion::Op op = RERegion::kIntersect_Op, bool doAntiAlias = false) = 0;

	// 保存surface到文件, 缺省格式为PNG, 支持JPEG
	virtual bool WriteToFile(const kn_string& path, REImageEncoder::Type type = REImageEncoder::kPNG_Type, kn_int quality = 100) = 0;

	// 绘制上屏
	virtual void Flip(LSTRECT &lst_rect) = 0;

	// 绘制椭圆
	virtual void DrawOval(const RERect& oval, const REPaint& paint) = 0;

	// 绘制圆角矩形
	virtual void DrawRoundRect(const RERect& rect, REScalar rx, REScalar ry, const REPaint& paint) = 0;

	// 绘制轨迹
	virtual void DrawPath(const REPath& path, const REPaint& paint) = 0;

	// 平移
	virtual bool Translate(REScalar dx, REScalar dy) = 0;

	// 缩放
	virtual bool Scale(REScalar sx, REScalar sy) = 0;

	// 旋转
	virtual bool Rotate(REScalar degrees) = 0;

	// 倾斜
	virtual bool Skew(REScalar sx, REScalar sy) = 0;

	// 获取画布矩阵
	virtual REMatrix GetTotalMatrix() = 0;

	// 设置画布矩阵
	virtual void SetMatrix(const REMatrix& m) = 0;

	// 变换画布矩阵
	virtual void Concat(const REMatrix& m) = 0;

	// 还原画布矩阵
	virtual void ResetMatrix() = 0;

	//绘制的预处理，screen的draw调用，用于硬件渲染的环境预设置
	virtual void preDraw() = 0;

	virtual REColor getColor(int x, int y) = 0;

};

// 第一个版本的RESurface Factory, 基于Skia2D引擎
class RESurfaceFactory
{
public:
	// 获取新的surface
	static IRESurface* CreateRESurface();

	// 获取新的surface并立即进行初始化, 假如初始化失败, 则返回空指针
	static IRESurface* CreateRESurface(kn_int width, kn_int height, REBitmap::Config colorFormat);

	static IRESurface* CreateRESurface(void* p, kn_int width, kn_int height, REBitmap::Config colorFormat);

	// 获取新的surface并立即从文件进行初始化, 假如初始化失败, 则返回空指针
	static IRESurface* CreateRESurface(const kn_string& filePath);

#if defined(WIN32)
	// 创建Win32上屏surface
	static IRESurface* CreateDeviceReSurface(HDC,kn_int width, kn_int height, REBitmap::Config colorFormat);
	static IRESurface* CreateLayerDeviceReSurface(HWND wnd, HDC hdc,kn_int w, kn_int h, REBitmap::Config cf);
	static IRESurface* CreateDDrawReSurface(HWND,kn_int width, kn_int height, REBitmap::Config colorFormat);
	static IRESurface* CreateGLReSurface(HDC,kn_int width, kn_int height, REBitmap::Config colorFormat);
	static IRESurface* CreateSkiaGLReSurface(HWND wnd, kn_int w, kn_int h, REBitmap::Config cf);
	static IRESurface* CreateGLLayerDeviceReSurface(HWND wnd, HDC hdc,kn_int w, kn_int h, REBitmap::Config cf);

#elif ANDROID_NDK


	static IRESurface* CreateAndroidReSurface(char* buff, kn_int width, kn_int height, REBitmap::Config colorFormat);
    
#elif IOS_DEV
    
    static IRESurface* CreateIOSReSurface(void*& buff, kn_int width, kn_int height, REBitmap::Config colorFormat);
    

#elif QT_HMI

    static IRESurface* CreateQtSoftReSurface(void* p, kn_int width, kn_int height, REBitmap::Config colorFormat);

#endif

};

}
#endif
