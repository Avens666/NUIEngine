//  **************************************
//  File:        DataSync.cpp
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
#include "DataSync.h"
#include "KView.h"
#include "KMoveableView.h"
#include <algorithm> 
//////////////////////CPropertyBase////////////////////
//------------------------------------------------
// CPropertyBase
//------------------------------------------------

CPropertyBase::CPropertyBase(void)
{

}

CPropertyBase::~CPropertyBase(void)
{

}

CPropertyBase* CPropertyBase::getCurrent(CPropertyBase* target, kn_double& dv, int iEasingCurveType)
{
	 KEasingCurve curve = KEasingCurve((KEasingCurve::Type)iEasingCurveType);
	 return getCurrent(target, dv, &curve);
}

KView_PTR CPropertyBase::getView()
{
	return m_p_view;
}

void CPropertyBase::setView(KView_PTR p)
{
	m_p_view = p;
}

kn_bool CPropertyBase::isChange()
{
	return FALSE;
}
//------------------------------------------------
// CPropertyPos
//------------------------------------------------
//////////////////////////// /////////
CPropertyPos::CPropertyPos(KView_PTR p, kn_int x, kn_int y)
{
	setProperty(p,x,y);
}

CPropertyPos::~CPropertyPos(void)
{

}

CPropertyBase* CPropertyPos::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	if (getView()->getType() ==  KVIEW_MOVEABLE)
	{
		if (  boost::dynamic_pointer_cast<KMoveableView>(getView() )->IsMousePicked() )
		{
			return NULL;
		}
	}
	kn_int tar_x, tar_y, x, y;
	((CPropertyPos*)target)->getProperty( tar_x, tar_y);

	x= curve->GetInterValue(m_point.x(), tar_x,dv);
	y= curve->GetInterValue(m_point.y(), tar_y,dv);

	CPropertyPos* p = new CPropertyPos(getView(), x, y);
	return p;
}

CPropertyBase* CPropertyPos::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);
	kn_int x,y;
	p_v->GetScreenXY(x, y);
	CPropertyPos* p = new CPropertyPos(p_v, x, y);
	return p;
}


CPropertyBase* CPropertyPos::copyProperty()
{
	kn_int x,y;
	getView()->GetPosition(x, y);
	CPropertyPos* p = new CPropertyPos(getView(), x, y);
	return p;
}


void CPropertyPos::getProperty(kn_int &x, kn_int &y)
{
	x = m_point.x();
	y = m_point.y();

}

void CPropertyPos::setProperty(KView_PTR p, kn_int x, kn_int y)
{
	setView(p);
	m_point.set(x, y);
}

void CPropertyPos::DoChange()
{
	getView()->SetPosition(m_point.x(), m_point.y());

}

kn_bool CPropertyPos::isChange()
{
	if (!getView())
	{
		return FALSE;
	}
	kn_int x,y;
	getView()->GetScreenXY(x, y);

	if (x == m_point.x() && y == m_point.y())
	{//不变
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

//------------------------------------------------
// CPropertyOpacity
//------------------------------------------------
CPropertyOpacity::CPropertyOpacity(KView_PTR p, kn_int opacity)
{
	setProperty(p, opacity);
}

CPropertyOpacity::~CPropertyOpacity()
{

}

void CPropertyOpacity::getProperty(kn_int &opacity)
{
	opacity = m_i_opacity;
}

void CPropertyOpacity::setProperty(KView_PTR p, kn_int opacity)
{
	setView(p);
	m_i_opacity = opacity;

}

void CPropertyOpacity::DoChange()
{
	getView()->setOpacity(m_i_opacity);
}

kn_bool CPropertyOpacity::isChange()
{
	if (!getView())
	{
		return FALSE;
	}

	if (getView()->getOpacity() == m_i_opacity)
	{//不变
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

CPropertyBase* CPropertyOpacity::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	kn_int tar_opacity;
	((CPropertyOpacity*)target)->getProperty(tar_opacity);

	kn_int opacity = curve->GetInterValue(m_i_opacity, tar_opacity,dv);

	CPropertyOpacity* p = new CPropertyOpacity(getView(), opacity);
	return p;
}


CPropertyBase* CPropertyOpacity::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);
	kn_int opacity = p_v->getOpacity();
	CPropertyOpacity* p = new CPropertyOpacity(p_v, opacity);
	return p;

}

//从自己的view copy
CPropertyBase* CPropertyOpacity::copyProperty()
{
	kn_int opacity = getView()->getOpacity();
	CPropertyOpacity* p = new CPropertyOpacity(getView(), opacity);
	return p;
}


//------------------------------------------------
// CPropertyRect
//------------------------------------------------
CPropertyRect::CPropertyRect(KView_PTR p, RERect rect)
{
	setProperty(p, rect);
}

CPropertyRect::~CPropertyRect()
{

}

void CPropertyRect::getProperty(RERect& rect)
{
	rect = m_rect;
}


void CPropertyRect::setProperty(KView_PTR p, RERect rect)
{
	setView(p);
	m_rect = rect;

}

kn_bool CPropertyRect::isChange()
{
	if (!getView())
	{
		return FALSE;
	}

	return (getView()->GetRect() != m_rect);
}

void CPropertyRect::DoChange()
{
	getView()->SetRect(m_rect);
}


// 获取rect的中间值
RERect GetInterRect(RERect rectSrc, RERect rectDst, kn_double dv, KEasingCurve* curve)
{
	int left	= curve->GetInterValue(rectSrc.left(), rectDst.left(), dv);
	int right	= curve->GetInterValue(rectSrc.right(), rectDst.right(), dv);
	int top		= curve->GetInterValue(rectSrc.top(), rectDst.top(), dv);
	int bottom	= curve->GetInterValue(rectSrc.bottom(), rectDst.bottom(), dv);

	return RERect::MakeLTRB(left, top, right, bottom);
}


CPropertyBase* CPropertyRect::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	RERect rectTarget;
	((CPropertyRect*)target)->getProperty(rectTarget);

	RERect rectInter = GetInterRect(m_rect, rectTarget, dv, curve);
   
	CPropertyRect* p = new CPropertyRect(getView(), rectInter);
	return p;
}

CPropertyBase* CPropertyRect::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);
	RERect rect = p_v->GetRect();
	CPropertyRect* p = new CPropertyRect(p_v, rect);
	return p;
}

//从自己的view copy
CPropertyBase* CPropertyRect::copyProperty()
{

	RERect rect = getView()->GetRect();
	CPropertyRect* p = new CPropertyRect(getView(), rect);
	return p;

}


//////////////////////////////////////////////////////////////////////////
//	CPropertyText
//////////////////////////////////////////////////////////////////////////


CPropertyText::CPropertyText(KView_PTR p, RERect rectBound, RERect rectClip)
{
	setProperty(p, rectBound, rectClip);
}


CPropertyText::~CPropertyText()
{

}


void CPropertyText::getProperty(RERect& rectBound, RERect& rectClip)
{
	rectBound = m_rect;
	rectClip = m_rect_clip;
}

void CPropertyText::setProperty(KView_PTR p, RERect rectBound, RERect rectClip)
{
	setView(p);
	m_rect = rectBound;
	m_rect_clip = rectClip;
}


CPropertyBase* CPropertyText::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	RERect rectBoundTarget;
	RERect rectClipTarget;
	((CPropertyText*)target)->getProperty(rectBoundTarget, rectClipTarget);

	RERect rectBoundInter = GetInterRect(m_rect, rectBoundTarget, dv, curve);
	RERect rectClipInter = GetInterRect(m_rect_clip, rectClipTarget, dv, curve);

	CPropertyText* p = new CPropertyText(getView(), rectBoundInter, rectClipInter);
	return p;
}

CPropertyBase* CPropertyText::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);

	KStaticView_PTR pTextView = boost::dynamic_pointer_cast<KStaticView>(p_v);
	ASSERT(pTextView);

	pTextView->GetTextBound();
	CPropertyText* p = new CPropertyText(pTextView, pTextView->GetTextBound(), pTextView->GetTextClip());
 

	return p;
}

CPropertyBase* CPropertyText::copyProperty()
{

	KStaticView_PTR pTextView = boost::dynamic_pointer_cast<KStaticView>(getView());
	ASSERT(pTextView);
 
	CPropertyText* p = new CPropertyText(pTextView, pTextView->GetTextBound(), pTextView->GetTextClip());
	return p;
}

kn_bool CPropertyText::isChange()
{
	// 待完善
	return CPropertyBase::isChange();
 
}

void CPropertyText::DoChange()
{
	KStaticView_PTR pTextView = boost::dynamic_pointer_cast<KStaticView>(getView() );


	pTextView->SetTextBound(m_rect);

	// m_rect_clip 相对于View的坐标
// 	kn_int x, y;
// 	pTextView->GetScreenXY(x, y);
// 
// 	RERect rectScreenClip = m_rect_clip;
// 	rectScreenClip.offset(x, y);
// 
// 	// 转成屏幕坐标
// 	pTextView->SetTextClip(rectScreenClip);


	pTextView->SetTextClip(m_rect_clip);

	//pTextView->SetTextClipByView(m_rect_clip);

}

//------------------------------------------------
// CPropertyClip
//------------------------------------------------


CPropertyClip::CPropertyClip(KView_PTR p, RERect rect)
{
	setProperty(p, rect);
}

CPropertyClip::~CPropertyClip(void)
{

}

void CPropertyClip::getProperty(RERect& rect)
{
	rect = m_rect;
}

void CPropertyClip::setProperty(KView_PTR p, RERect rect)
{
	setView(p);
	m_rect = rect;
}

CPropertyBase* CPropertyClip::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	RERect rectTarget;
	((CPropertyClip*)target)->getProperty(rectTarget);

	RERect rectInter = GetInterRect(m_rect, rectTarget, dv, curve);

	CPropertyClip* p = new CPropertyClip(getView(), rectInter);
	return p;
}

CPropertyBase* CPropertyClip::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);
	RERect rect = p_v->GetClip();
	CPropertyClip* p = new CPropertyClip(p_v, rect);
	return p;
}

//从自己的view copy
CPropertyBase* CPropertyClip::copyProperty()
{
	RERect rect = getView()->GetClip();
	CPropertyClip* p = new CPropertyClip(getView(), rect);
	return p;
}

kn_bool CPropertyClip::isChange()
{
	if (!getView())
	{
		return FALSE;
	}

	return (getView()->GetClip() != m_rect);
}

void CPropertyClip::DoChange()
{
//	MyTraceA("CPropertyClip::DoChange m_rect (%.2f, %.2f, %.2f, %.2f)", m_rect.left(), m_rect.right(), m_rect.top(), m_rect.bottom());
	getView()->SetClip(m_rect);
}




//------------------------------------------------
// CDataSync
//------------------------------------------------
CDataSync::CDataSync(void)
{
	m_p_lst_data = NULL;
	m_current_id = 1;
}


CDataSync::~CDataSync(void)
{
	release();
}

void CDataSync::release()
{
	boost::mutex::scoped_lock lock(m_data_mutex);
	if (m_p_lst_data)
	{
		SAFE_DELETE(m_p_lst_data);
	}
}

CPropertyList* CDataSync::getDataList()
{
	boost::mutex::scoped_lock lock(m_data_mutex);
	CPropertyList* p = m_p_lst_data;
	m_p_lst_data = NULL;
	//清除动画ID
	writeLock lock2(m_id_mutex);
	m_id_lst.clear();
	return p;
}

void CDataSync::addProperty(CPropertyBase* p)
{
	boost::mutex::scoped_lock lock(m_data_mutex);
	if (m_p_lst_data == NULL)
	{
		m_p_lst_data = new CPropertyList;
	}

	m_p_lst_data->addProperty(p);
}

//获得一个新id
kn_int CDataSync::getNewID()
{
	return m_current_id++;

}

//检查ID是否存在
kn_bool CDataSync::checkID(kn_int& n)
{
	readLock lock(m_id_mutex);
	if (m_id_lst.size() == 0)
	{
		return FALSE;
	}
	vector<kn_int>::iterator it = find(m_id_lst.begin(),m_id_lst.end(),n);
	if (it == m_id_lst.end())
	{
		return FALSE;
	}

	return TRUE;
}

//加入ID标识
void CDataSync::addID(kn_int& n)
{
	//这里没做重复性检查,感觉不太必要,而且消耗效率.动画线程本身会控制,而且即使重复加入也不影响
	writeLock lock(m_id_mutex);
	m_id_lst.push_back(n);
}


//////////////////////////////////

CPropertyList::~CPropertyList()
{
	for(PropertyBaseVector::iterator ite = m_lst_data.begin();ite!=m_lst_data.end();ite++)
	{
		SAFE_DELETE(*ite);
	}
	m_lst_data.clear();
}

void CPropertyList::DoChange()
{
	for(PropertyBaseVector::iterator ite = m_lst_data.begin();ite!=m_lst_data.end();ite++)
	{
		CPropertyBase* p = (*ite);
		p->DoChange();
	}
}

void CPropertyList::addProperty(CPropertyBase* p)
{
	m_lst_data.push_back(p);
}




//------------------------------------------------
// CPropertyAngle
//------------------------------------------------

void CPropertyAngle::setProperty( KView_PTR p, REScalar degree, REScalar px, REScalar py )
{
	setView(p);
	m_degree = degree;
	m_px = px;
	m_py = py;
}

void CPropertyAngle::DoChange()
{
	getView()->SetRotate(m_degree, m_px, m_py);
}

kn_bool CPropertyAngle::isChange()
{
	if (!getView())
	{
		return FALSE;
	}

	return (getView()->getRotateAngle() != m_degree);
}

CPropertyAngle::CPropertyAngle( KView_PTR p, REScalar degree, REScalar px, REScalar py )
{
	setProperty(p, degree, px, py);
}

CPropertyAngle::~CPropertyAngle( void )
{

}

void CPropertyAngle::getProperty(REScalar &a, REScalar &x, REScalar &y)
{
	x = m_px;
	y = m_py;
	a = m_degree;
}

CPropertyBase* CPropertyAngle::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	REScalar tar_a, tar_x, tar_y, a, x, y;
	((CPropertyAngle*)target)->getProperty( tar_a, tar_x, tar_y);

	a= curve->GetInterValue(m_degree, tar_a,dv);
	x= curve->GetInterValue(m_px, tar_x,dv);
	y= curve->GetInterValue(m_py, tar_y,dv);

	CPropertyAngle* p = new CPropertyAngle(getView(), a, x, y);
	return p;
}

CPropertyBase* CPropertyAngle::copyProperty(KView_PTR p)
{
	ASSERT(p);
	REScalar a, x,y;
	a = p->getRotateAngle();
	p->getRotatePoint(x,y);
	CPropertyAngle* pa = new CPropertyAngle(p, a, x, y);
	return pa;
}

//从自己的view copy
CPropertyBase* CPropertyAngle::copyProperty()
{

	REScalar a, x,y;
	a = getView()->getRotateAngle();
	getView()->getRotatePoint(x,y);
	CPropertyAngle* p = new CPropertyAngle(getView(), a, x, y);
	return p;
}



//------------------------------------------------
// CPropertyPara
//------------------------------------------------
CPropertyPara::CPropertyPara(KView_PTR p, kn_int para_id, kn_double v)
{
	m_para_id = para_id;
	setProperty(p, v);
}

CPropertyPara::~CPropertyPara()
{

}

void CPropertyPara::getProperty(kn_double &v)
{
	v = m_value ;
}

void CPropertyPara::setProperty(KView_PTR p, kn_double v)
{
	setView(p);
	m_value = v;

}

void CPropertyPara::DoChange()
{
	getView()->doChange(m_para_id, m_value );
}

kn_bool CPropertyPara::isChange()
{
	if (!getView())
	{
		return FALSE;
	}

	return getView()->isChange(m_para_id, m_value );

}

CPropertyBase* CPropertyPara::getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)
{
	kn_double tar_opacity;
	((CPropertyPara*)target)->getProperty(tar_opacity);

	kn_double v = curve->GetInterValue(m_value, tar_opacity,dv);

	CPropertyPara* p = new CPropertyPara(getView(), m_para_id, v);
	return p;
}


CPropertyBase* CPropertyPara::copyProperty(KView_PTR p_v)
{
	ASSERT(p_v);
	kn_double v = p_v->getParaValue(m_para_id);
	CPropertyPara* p = new CPropertyPara(p_v, m_para_id, v);
	return p;

}

//从自己的view copy
CPropertyBase* CPropertyPara::copyProperty()
{
	kn_double  v = getView()->getParaValue(m_para_id);
	CPropertyPara* p = new CPropertyPara(getView(), m_para_id ,v);
	return p;
}
