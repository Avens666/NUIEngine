//  **************************************
//  File:        KImageDrawable.h
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
#ifndef K_IMAGE_DRAWABLE_H
#define K_IMAGE_DRAWABLE_H

#include "KDrawable.h"
#include "SkShader.h" //huw 
class NUI_API KImageDrawable : public KDrawable
{


public:
	enum FillType
	{
		FILL_NORMAL,
		FILL_ALL,
		FILL_NOSCALE,
		FILL_TILE//huw
	};
	KImageDrawable();
	KImageDrawable(const kn_string& strFile);
	KImageDrawable(IRESurface* pSurface,  kn_bool b_out = FALSE);
	void Initialize();


	virtual ~KImageDrawable(void);
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);

	kn_bool CreateFromFile(const kn_string& strFile);
	kn_bool CreateFromSurface(IRESurface* pSurface, kn_bool b_out = FALSE);
	bool GetImageBound(RERect& rct);	// 返回资源大小, 假如没有资源, 则返回false
	bool IsNull();	//	资源是否加载成功
	void SizeToImage(); // 将Drawable拉伸到资源大小
	void Release();
	
	void SetFillType(FillType type);

	// 设置边框，调试用
	void SetBorder(kn_bool bEnable, REColor color = RE_ColorLTGRAY, int iStrokeWidth = 1);
	//huw
	void setTileModeX(SkShader::TileMode mode);
	void setTileModeY(SkShader::TileMode mode);
	void setTileModeXY(SkShader::TileMode xmode, SkShader::TileMode ymode);
	
	virtual kn_bool checkPoint(int x, int y);
protected:
	IRESurface* m_p_surface;
	kn_int m_iFillType;
	//是否释放资源
	kn_bool m_b_out_resource;

	// 设置边框，调试用
	kn_bool m_b_border;
	REColor m_cl_border_color;
	kn_int m_i_border_width;

	//huw
	SkShader::TileMode m_xmode;
	SkShader::TileMode m_ymode;

};

typedef boost::shared_ptr<KImageDrawable> KImageDrawable_PTR;


//双缓冲drable
class NUI_API KDBufferDrawable: public KImageDrawable
{

public:
	KDBufferDrawable(int w, int h);
	virtual ~KDBufferDrawable(void);

	//后台绘制，取出后台buffer指针，可以使用此指针进行绘制 绘制完成调用draw_end
	IRESurface* drawBegin();

	//结束后台绘制
	void drawEnd();
	//交换前台和后台buffer
	void swapBuffer();

protected:
	IRESurface* m_p_bk_surface;
	kn_bool m_lock_bk;

};
typedef boost::shared_ptr<KDBufferDrawable> KDBufferDrawable_PTR;

//局部Drawable
class NUI_API KImagePartDrawable: public KImageDrawable
{

public:
	KImagePartDrawable( const kn_string& strFile, const RERect& srcRect);
	virtual ~KImagePartDrawable(void);

	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);

	void setSrcRect(const RERect& srcRect);

protected:
	RERect m_src_rect;
};
typedef boost::shared_ptr<KImagePartDrawable> KImagePartDrawable_PTR;

#endif
