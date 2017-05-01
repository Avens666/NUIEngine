//  **************************************
//  File:        KShapeDrawable.cpp
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
#include "KShapeDrawable.h"
#include "KSvgAnalyzing.h"

//////////////////////////////////////////////////////////
//KShape
KShape::KShape()
{
	m_fill_clr = 0x000000;
	m_fillOpacity = 1.0;
	m_stroke_clr = 0x000000;
	m_stroke_width = 0;
	m_stroke_miterlimit = 10;
	m_strokeOpacity = 1.0;
	m_opacity = 1.0;
	m_bfill = true;
	m_bstroke = true;
}

void KShape::setFillColor(REColor& fill)
{
	m_fill_clr = fill;
}
void KShape::setFillOpacity(float fillOpacity)
{
	m_fillOpacity = fillOpacity;
}
float KShape::getFillOpacity()
{
	return m_fillOpacity;
}
void KShape::setStrokeColor(REColor& stroke)
{
	m_stroke_clr = stroke;
}
void KShape::setStrokeWidth(int strokeWidth)
{
	m_stroke_width = strokeWidth;
}
void KShape::setStrokeMiterlimit(int miterlimit)
{
	m_stroke_miterlimit = miterlimit;
}
void KShape::setStrokeOpacity(float opacity)
{
	m_strokeOpacity = opacity;
}
float KShape::getStrokeOpacity()
{
	return m_strokeOpacity;
}
void KShape::setShapeType(SHAPETYPE type)
{
	m_shapeType = type;
}
SHAPETYPE KShape::getShapeType()
{		
	return m_shapeType;
}
void KShape::setOpacity(float opacity)
{
	m_opacity = opacity;
}
bool KShape::getFillShowFlag()
{
	return m_bfill;
}
bool KShape::getStrokeShowFlag()
{
	return m_bstroke;
}
void KShape::initPaint()
{
	m_fillPaint.setAntiAlias(true);
	m_fillPaint.setColor(m_fill_clr);
	m_fillPaint.setStyle(SkPaint::kFill_Style);
	int f_alpha = 255 * m_fillOpacity * m_opacity ;
	if(f_alpha == 0)
	{
		m_bfill = false;
	}
	m_fillPaint.setAlpha(f_alpha);

	m_strokePaint.setAntiAlias(true);
	m_strokePaint.setColor(m_stroke_clr);
	if (m_stroke_width == 0&&m_stroke_clr == 0x00000000)
	{
		m_bstroke = false;
		m_strokePaint.setAlpha(0);
	}
	else
	{
		int s_alpha = 255 * m_opacity * m_strokeOpacity;
		m_strokePaint.setAlpha(s_alpha);
	}
	m_strokePaint.setStyle(SkPaint::kStroke_Style);
	m_strokePaint.setStrokeWidth(m_stroke_width);
	m_strokePaint.setStrokeMiter(m_stroke_miterlimit);
}


void KShape::setRect(RERect& rect)
{
	m_rect = rect;
}

void KShape::setRect(int x, int y, int w, int h)
{
	m_rect.setXYWH(x, y, w, h);
}

//////////////////////////////////////////////////////////
//KRectShape
KRectShape::KRectShape()
{
	m_shapeType = SHAPE_RECT;
}
KRectShape::KRectShape(RERect rect)
{
	m_shapeType = SHAPE_RECT;
	m_rect = rect;
}


void KRectShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	if (m_bfill)
	{
		pDstSurface->DrawRect(m_rect, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawRect(m_rect, m_strokePaint);
	}
}
//////////////////////////////////////////////////////////////
//KOvalShape
KOvalShape::KOvalShape()
{

}

KOvalShape::KOvalShape(RERect rect) : KRectShape(rect)
{

}


void KOvalShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	pDstSurface->DrawOval(m_rect, paint);
}

///////////////////////////////////////////////////////////
//KRoundRectShape
void KRoundRectShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	if (m_bfill)
	{
		pDstSurface->DrawRoundRect(m_rect, m_rx, m_ry, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawRoundRect(m_rect, m_rx, m_ry, m_strokePaint);
	}
}

void KRoundRectShape::SetRX(float rx)
{
	m_rx = rx;
}
void KRoundRectShape::SetRY(float ry)
{
	m_ry = ry;
}

///////////////////////////////////////////////////////////
//KArcShape
KArcShape::KArcShape()
{
	m_start_angle = 0;
	m_sweep_angle = 0;
}

KArcShape::KArcShape(RERect rect) : KRectShape(rect)
{
	m_start_angle = 0;
	m_sweep_angle = 0;
}

void KArcShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	if (m_bfill)
	{
		pDstSurface->DrawArc(m_rect, m_start_angle, m_sweep_angle, TRUE, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawArc(m_rect, m_start_angle, m_sweep_angle, TRUE, m_strokePaint);
	}
}

void KArcShape::setStartAngle(int v)
{
	m_start_angle = v;
}
void KArcShape::setSweepAngle(int v)
{
	m_sweep_angle = v;
}


//////////////////////////////////////////////////////////
//KCircleShape
KCircleShape::KCircleShape()
{

}
KCircleShape::KCircleShape(int cx, int cy,int r) 
{
	m_cx = cx;
	m_cy = cy;
	m_r = r;
}
void KCircleShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
//	pDstSurface->DrawCircle(m_cx, m_cy, m_r, paint);

	if (m_bfill)
	{
		pDstSurface->DrawCircle(m_cx, m_cy, m_r, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawCircle(m_cx, m_cy, m_r, m_strokePaint);
	}
}
void KCircleShape::SetCX(float cx)
{
	m_cx = cx;
}
void KCircleShape::SetCY(float cy)
{
	m_cy = cy;
}
void KCircleShape::SetCR(float cr)
{
	m_r = cr;
}
//////////////////////////////////////////////////////////
//KLineShape
KLineShape::KLineShape()
{

}
KLineShape::KLineShape(REPoint p1, REPoint p2) 
{
	m_p1 = p1;
	m_p2 = p2;
}

void KLineShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	if (m_bfill)
	{
		pDstSurface->DrawLine(m_p1.fX, m_p1.fY, m_p2.fX, m_p2.fY, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawLine(m_p1.fX, m_p1.fY, m_p2.fX, m_p2.fY, m_strokePaint);
	}
	
}
void KLineShape::SetStartPoint(REPoint p1)
{
	m_p1 =p1;
}
void KLineShape::SetEndPoint(REPoint p2)
{
	m_p2 =p2;
}
//////////////////////////////////////////////////////////
// KPolygonShape
KPolygonShape::KPolygonShape()
{
	m_fill_clr = 0x000000;
	m_fillOpacity = 1.0;
	m_p_points = NULL;
}
KPolygonShape::KPolygonShape(REPoint* pPoints, int count, kn_bool bClose) 
{
	// 深拷贝
	m_p_points = new REPoint[count];
	memcpy(m_p_points, pPoints, sizeof(REPoint)*count);
	//m_p_points = pPoints;
	m_i_points_count = count;
	m_b_close = bClose;

}

KPolygonShape::~KPolygonShape()
{
	Release();
}

void KPolygonShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	// 暂时用Polygon代替
	if (m_bfill)
	{
		pDstSurface->DrawPolygon(m_p_points, m_i_points_count, m_b_close, m_fillPaint);
	}

	if (m_bstroke)
	{
		pDstSurface->DrawPolygon(m_p_points, m_i_points_count, m_b_close, m_strokePaint);
	}
}
void KPolygonShape::SetPoints(vector<REPoint>& pointList)
{
	m_i_points_count = pointList.size();
	m_p_points = new REPoint[m_i_points_count];
	for (int i=0;i<m_i_points_count;i++)
	{
		m_p_points[i] = pointList.at(i);
	}
	//m_p_points = pPoints;
}
void KPolygonShape::SetCloseFlag(bool bClose)
{
	m_b_close = bClose;
}
void KPolygonShape::Release()
{
	SAFE_DELETE_GROUP(m_p_points);
	m_i_points_count = 0;
}
////////////////////////////////////////////////////////////
//KPathShape
KPathShape::KPathShape(REPath& path)
{
	m_path = path;
}

KPathShape::~KPathShape()
{
	Release();
}


void KPathShape::Release()
{	
}
void KPathShape::Draw(IRESurface* pDstSurface, const REPaint& paint)
{
	// 暂时用Polygon代替
	pDstSurface->DrawPath(m_path, paint);
}

REPath* KPathShape::GetPath()
{
	return &m_path;
}

void KPathShape::SetPath(REPath& path)
{
	m_path = path;
}
//////////////////////////////////////////////////////////////////////////
// KShapeDrawable
KShapeDrawable::KShapeDrawable()
{
	m_type = KDrawable::KDRAWABLE_SHAPE;
	m_p_shape = NULL;

}

KShapeDrawable::KShapeDrawable(KShape* pShape)
{
	m_type = KDrawable::KDRAWABLE_SHAPE;
	m_p_shape = pShape;

}

KShapeDrawable::~KShapeDrawable()
{
	Release();

}

void KShapeDrawable::SetRect(const RERect& rect)
{
	writeLock lock(m_drawable_draw_mutex); 
	KDrawable::SetRect(rect);

	if (m_p_shape)
	{
		switch(  m_p_shape->getShapeType() )
		{
		case SHAPE_RECT:
            {
                RERect therect = RERect::MakeXYWH(0,0, m_rect.width(), m_rect.height());
                ((KRectShape*)m_p_shape)->setRect(therect);
            }
			break;
		}
	}
}

void KShapeDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	readLock lock(m_drawable_draw_mutex); 

	if (! BeginDraw(pDstSurface, pMatrix, iDstX, iDstY ) )
	{
		return;
	}

	int iTarX = iDstX + m_rect.left();
	int iTarY = iDstY + m_rect.top();
	pDstSurface->Translate(iTarX, iTarY);
	if (m_p_shape)
	{
		m_p_shape->Draw(pDstSurface, m_paint);
	}

//	pDstSurface->Restore();
	EndDraw(pDstSurface, pMatrix);

}


void KShapeDrawable::Release()
{
	SAFE_DELETE(m_p_shape);
}

void KShapeDrawable::setShape(KShape* pShape)
{
	writeLock lock(m_drawable_draw_mutex); 
	if(pShape != m_p_shape)
	{
		SAFE_DELETE(m_p_shape);
		m_p_shape = pShape;
	}
}

KShape* KShapeDrawable::getShape()
{
	return m_p_shape;
}

//////////////////////////////////////////////////////////////////////////
// KSVGDrawable

KSVGDrawable::KSVGDrawable()
{

}
KSVGDrawable::KSVGDrawable(kn_string svgFilePath)
{ 
	RERect rect;
	rect.set(0,0,0,0);
	KSvgAnalyzing* m_pSvgAn = new KSvgAnalyzing;
	m_pSvgAn->LoadSvgFile(svgFilePath,m_pShapeList,rect,false);
	CreateOfSvg(rect);
	delete m_pSvgAn;
}
KSVGDrawable::KSVGDrawable(kn_string svgFilePath,RERect& rect)
{ 
	KSvgAnalyzing* m_pSvgAn = new KSvgAnalyzing;
	m_pSvgAn->LoadSvgFile(svgFilePath,m_pShapeList,rect,true);
	CreateOfSvg(rect);
	delete m_pSvgAn;
}

KSVGDrawable::~KSVGDrawable()
{
	
}

void KSVGDrawable::Release()
{

}

void KSVGDrawable::Draw(IRESurface* pDstSurface, const REMatrix* pMatrix, int iDstX, int iDstY)
{
	for(vector<KShapeDrawable_PTR>::iterator it = m_vec_shape_drawables.begin(); it != m_vec_shape_drawables.end(); it++)
	{
		int iAlphaBak = m_paint.getAlpha();
		REPaint* pPaint = (*it)->GetPaint();
		int iCurrentAlpha = pPaint->getAlpha() * iAlphaBak / 255;
		pPaint->setAlpha(iCurrentAlpha);
		(*it)->Draw(pDstSurface, pMatrix, iDstX, iDstY);
		pPaint->setAlpha(iAlphaBak);
	}
}

 

void KSVGDrawable::addShape(KShape* pShape, REPaint* pPaint)
{
	KShapeDrawable_PTR drawable =  KShapeDrawable_PTR(new KShapeDrawable(pShape));

	if(pPaint != NULL)
	{
		drawable->SetPaint(pPaint);
	}

	drawable->SetRect(m_rect);

	m_vec_shape_drawables.push_back(drawable);
}
void KSVGDrawable::addLine(REPoint p1, REPoint p2, REPaint* pPaint)
{
	KLineShape* pLine = new KLineShape(p1, p2);
	addShape(pLine, pPaint);

}
void KSVGDrawable::addLine(KShape* pShape, REPaint* pPaint)
{
	addShape(pShape, pPaint);

}
void KSVGDrawable::addPolygon(REPoint* pPoints, int count, kn_bool bClose, REPaint* pPaint)
{
	KPolygonShape* pPoly = new KPolygonShape(pPoints, count, bClose);
	addShape(pPoly, pPaint);

}
void KSVGDrawable::addPolygon(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}

}
void KSVGDrawable::addPath(REPath& path, REPaint* pPaint)
{
	KPathShape* pPathShape = new KPathShape(path);
	addShape(pPathShape, pPaint);
}
void KSVGDrawable::addPath(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}
}
void KSVGDrawable::addRoundRect(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}
}
void KSVGDrawable::addCircle(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}
}
void KSVGDrawable::addEllipse(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}
}


void KSVGDrawable::addSVGShape(KShape* pShape, REPaint* pFillPaint,REPaint* pStorkPaint)
{
	if (pShape->getFillShowFlag())
	{
		addShape(pShape,pFillPaint);
	} 
	if (pShape->getStrokeShowFlag())
	{
		addShape(pShape,pStorkPaint);
	}
}

void KSVGDrawable::CreateOfSvg(RERect& rect)
{
	this->SetRect(rect);
	for (UINT i=0;i<m_pShapeList.size();i++)
	{
		SHAPETYPE type = m_pShapeList.at(i)->getShapeType();
		switch (type)
		{
		case SHAPE_RECT:
			{
				((KRoundRectShape*)(m_pShapeList.at(i)))->initPaint();
				REPaint fillPaint = ((KRoundRectShape*)m_pShapeList.at(i))->m_fillPaint;
				REPaint strokePaint = ((KRoundRectShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addRoundRect((KRoundRectShape*)m_pShapeList.at(i),&fillPaint,&strokePaint);
			}	
			break;
		case SHAPE_CIRCLE:
			{
				((KCircleShape*)m_pShapeList.at(i))->initPaint();
				REPaint fillPaint = ((KCircleShape*)m_pShapeList.at(i))->m_fillPaint;
				REPaint strokePaint = ((KCircleShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addCircle((KCircleShape*)m_pShapeList.at(i),&fillPaint,&strokePaint);
			}
			break;
		case SHAPE_ELLIPSE:
			{
				((KOvalShape*)m_pShapeList.at(i))->initPaint();
				REPaint fillPaint = ((KOvalShape*)m_pShapeList.at(i))->m_fillPaint;
				REPaint strokePaint = ((KOvalShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addEllipse((KOvalShape*)m_pShapeList.at(i),&fillPaint,&strokePaint);
			}	
			break;
		case SHAPE_LINE:
			{
				((KLineShape*)m_pShapeList.at(i))->initPaint();
				REPaint strokePaint = ((KLineShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addLine((KLineShape*)m_pShapeList.at(i),&strokePaint);
			}
			break;
		case SHAPE_POLYGON:
			{
				((KPolygonShape*)m_pShapeList.at(i))->initPaint();
				REPaint fillPaint = ((KPolygonShape*)m_pShapeList.at(i))->m_fillPaint;
				REPaint strokePaint = ((KPolygonShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addPolygon((KLineShape*)m_pShapeList.at(i),&fillPaint,&strokePaint);
				}
			break;
		case SHAPE_PATH:
			{
				((KPathShape*)m_pShapeList.at(i))->initPaint();
				REPaint fillPaint = ((KPathShape*)m_pShapeList.at(i))->m_fillPaint;
				REPaint strokePaint = ((KPathShape*)m_pShapeList.at(i))->m_strokePaint;
				this->addPath((KPathShape*)m_pShapeList.at(i),&fillPaint,&strokePaint);
			}
			break;
		}
	}
}


