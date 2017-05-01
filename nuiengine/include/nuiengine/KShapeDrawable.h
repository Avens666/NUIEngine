//  **************************************
//  File:        KShapeDrawable.h
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
#ifndef K_SHAPE_DRAWABLE_H
#define K_SHAPE_DRAWABLE_H


#include "KDrawable.h"
class KSvgAnalyzing;
enum PATHTYPE
{
	PATH_ABS_MOVETO = 0,    //absolute moveto
	PATH_ABS_LINETO,        //absolute lineto
	PATH_ABS_H_LINETO,      //absolute horizontal lineto
	PATH_ABS_V_LINETO,	    //absolute vertical lineto
	PATH_ABS_CURVETO,       //absolute curveto
	PATH_ABS_S_CURVETO,     //absolute smooth curveto
	PATH_ABS_Q_CURVE,       //absolute quadratic Belzier curve
	PATH_ABS_T_CURVETO,      //absolute smooth quadratic Belzier curveto
	PATH_ABS_ARC,			//absolute elliptical Arc
	PATH_REL_MOVETO,        //relative moveto
	PATH_REL_LINETO,        //relative lineto
	PATH_REL_H_LINETO,      //relative horizontal lineto
	PATH_REL_V_LINETO,	    //relative vertical lineto
	PATH_REL_CURVETO,       //relative curveto
	PATH_REL_S_CURVETO,     //relative smooth curveto
	PATH_REL_Q_CURVE,       //relative quadratic Belzier curve
	PATH_REL_T_CURVETO,      //relative smooth quadratic Belzier curveto
	PATH_REL_ARC,			//relative elliptical Arc
	PATH_CLOSEPATH			//relative closepath
};
//图形类别
enum SHAPETYPE
{
	SHAPE_RECT = 0,
	SHAPE_CIRCLE,
	SHAPE_ELLIPSE,
	SHAPE_LINE,
	SHAPE_POLYGON,
	SHAPE_PATH
};
//path段数据结构体
struct PathData
{
	PATHTYPE pathType;
	vector<float> PathUnitDataList;

};
class NUI_API KShape : public KObject
{
protected:
	REColor m_fill_clr; //填充颜色
	float m_fillOpacity; //填充颜色透明度
	REColor m_stroke_clr;//边框颜色
	int m_stroke_width;//边框、线的宽度
	int m_stroke_miterlimit;
	float m_strokeOpacity;//边框、线的透明度
	float m_opacity;//整体透明度
	bool m_bfill;
	bool m_bstroke;
	SHAPETYPE m_shapeType;
	RERect m_rect;
public:
	REPaint m_fillPaint;
	REPaint m_strokePaint;
	KShape();
	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint) = 0;
	void setFillColor(REColor& fill);
	void setFillOpacity(float fillOpacity);
	float getFillOpacity();
	void setStrokeColor(REColor& stroke);
	void setStrokeWidth(int strokeWidth);
	void setStrokeMiterlimit(int miterlimit);
	void setStrokeOpacity(float opacity);
	float getStrokeOpacity();
	void setShapeType(SHAPETYPE type);
	SHAPETYPE getShapeType();
	void setOpacity(float opacity);
	bool getFillShowFlag();
	bool getStrokeShowFlag();
	void initPaint();
	void setRect(RERect& rect);
	void setRect(int x, int y, int w, int h);
};


class NUI_API KRectShape : public KShape
{	
protected:


public:
	KRectShape();
	KRectShape(RERect rect);

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);

};
//椭圆形
class NUI_API KOvalShape : public KRectShape
{
public:
	KOvalShape();
	KOvalShape(RERect rect);

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);
};

//扇形
class NUI_API KArcShape : public KRectShape
{
protected:
	int m_start_angle;
	int m_sweep_angle;

public:
	KArcShape();
	KArcShape(RERect rect);

	void setStartAngle(int );
	void setSweepAngle(int );

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);



};

//圆角矩形
class NUI_API KRoundRectShape : public KRectShape
{
protected:
	float m_rx;	// x方向上的圆角半径
	float m_ry;	//y方向上的圆角半径

public: 
	KRoundRectShape(){};
	KRoundRectShape(RERect rect, float rx, float ry): KRectShape(rect) 
	{
		m_rx = rx;
		m_ry = ry;
	}

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);
	void SetRX(float rx);
	void SetRY(float ry);
};
//圆形
class NUI_API KCircleShape : public KShape
{
protected:
	float m_cx;    //圆心x坐标
	float m_cy;	//圆心y坐标
	float m_r;	//半径


public: 
	KCircleShape();
	KCircleShape(int cx, int cy,int r) ;
	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);
	void SetCX(float cx);
	void SetCY(float cy);
	void SetCR(float cr);
};
//线
class NUI_API KLineShape : public KShape
{
protected:
	REPoint m_p1;
	REPoint m_p2;

public: 
	KLineShape();
	KLineShape(REPoint p1, REPoint p2);

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);
	void SetStartPoint(REPoint p1);
	void SetEndPoint(REPoint p2);
};
//多边形、折线
class NUI_API KPolygonShape : public KShape
{
protected:

	REPoint* m_p_points;
	int m_i_points_count;
	kn_bool m_b_close;

public:
	KPolygonShape();
	KPolygonShape(REPoint* pPoints, int count, kn_bool bClose) ;

	~KPolygonShape();

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);
	void SetPoints(vector<REPoint>& pointList);
	void SetCloseFlag(bool bClose);
	void Release();
	
};
//路径
class NUI_API KPathShape : public KShape
{
protected:

	REPath m_path;

public:
	KPathShape(){};
	KPathShape(REPath& path);

	~KPathShape();


	void Release();

	virtual void Draw(IRESurface* pDstSurface, const REPaint& paint);

	REPath* GetPath();
	
	void SetPath(REPath& path);
};

class NUI_API KShapeDrawable : public KDrawable
{
protected:
	KShape* m_p_shape;

public :
	KShapeDrawable();
	KShapeDrawable(KShape* pShape);
	virtual ~KShapeDrawable();
	virtual void SetRect(const RERect& rect);
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);
	void Release();
	void setShape(KShape* pShape);
	KShape* getShape();
	
};
typedef boost::shared_ptr<KShapeDrawable> KShapeDrawable_PTR;

class NUI_API KSVGDrawable : public KDrawable
{
protected:
	vector<KShapeDrawable_PTR> m_vec_shape_drawables;
	vector<KShape*> m_pShapeList;
protected:

	void addShape(KShape* pShape, REPaint* pPaint);

public:

	KSVGDrawable();
	KSVGDrawable(kn_string svgFilePath);
	KSVGDrawable(kn_string svgFilePath, RERect& rect);

	virtual ~KSVGDrawable();
	virtual void Draw(IRESurface* pDstSurface, const REMatrix* pMatrix = 0, int iDstX = 0, int iDstY = 0);
	void Release();

	void addLine(REPoint p1, REPoint p2, REPaint* pPaint);
	void addPolygon(REPoint* pPoints, int count,  kn_bool bClose, REPaint* pPaint);
	void addPath(REPath& path, REPaint* pPaint);
	void addLine(KShape* pShape, REPaint* pPaint);

	// 代替addPolygon,addPath,addRoundRect,addCircle,addEllipse
	void addSVGShape(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);

	void addPolygon(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);
	void addPath(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);
	void addRoundRect(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);
	void addCircle(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);
	void addEllipse(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint);


protected:
	void CreateOfSvg(RERect& rect);

};

typedef boost::shared_ptr<KSVGDrawable> KSVGDrawable_PTR;
void MyTraceA(const char* szFormat, ... );
#endif
