//  **************************************
//  File:        KProcessView.cpp
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
#include "KProcessView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"
#include "boost/lexical_cast.hpp"
#include "KFontManager.h"
//////////////////////////////////////////////////////////////////////////
// KProcessView
KProcessView::KProcessView()
{
	m_value = 0;
	m_e_dir = PROCESS_Horizontal;
	m_b_process_clip = TRUE;
	m_b_auto_center_text = FALSE;
}

KProcessView::~KProcessView()
{
	 
}

void KProcessView::setIconDrawableRect(int l, int r, int w, int h)
{
	if (m_b_process_clip)
	{
		m_icon_drawable->SetClip( RERect::MakeXYWH(l, r, w, h ) );
	}
	else
	{
		m_icon_drawable->setRect( l, r, w, h );
	}
}

void KProcessView::init(PROCESS_DIR e)
{
	m_e_dir = e;

	RERect rect = m_icon_drawable->GetRect();
	switch (e)
	{
	case PROCESS_Horizontal://水平
		setIconDrawableRect( rect.left(), rect.top(), 0, rect.height());
		break;
	case PROCESS_Vertical:
		setIconDrawableRect(  rect.left(), rect.top(), rect.width(), 0 );
		break;
	}

	if (m_b_auto_center_text)
	{
		m_text_drawable->setRect(  (m_rect.width() -50)/2, m_rect.height()/2-10, 50, 28 );
	}
}

void KProcessView::enableProcessClip(kn_bool b)
{
	m_b_process_clip = b;
}

void KProcessView::setValue(int iPos)    //设定进度的起始位置
{
	if (m_text_drawable->isShow())
	{
		kn_string str = boost::lexical_cast<kn_string>(iPos);
		str += _T("%");
		m_text_drawable->SetText(str);
		int w = m_text_drawable->getTextWidth();

		if (m_b_auto_center_text)
		{
			RERect txt_rect = m_text_drawable->GetRect();
			m_text_drawable->SetRect( RERect::MakeXYWH( (m_rect.width() -w)/2, txt_rect.top(), w, txt_rect.height()) );
		}
	}

	if (iPos == 0)
	{
		m_icon_drawable->SetShow(FALSE);
	}
	else
	{
		m_icon_drawable->SetShow(TRUE);
		RERect rect;
		if (m_b_process_clip)
		{
			rect = m_icon_drawable->GetRect();
		}
		else
		{//非裁剪显示，icon rect不断变化，不能使用，但是用bk的，如果bk和icon不一样大也有问题
			rect = m_bk_drawable->GetRect();
		}
		
		switch (m_e_dir)
		{
		case PROCESS_Horizontal:
			setIconDrawableRect( rect.left(), rect.top(), iPos * rect.width()/100, rect.height() );
			break;
		case PROCESS_Vertical:
			setIconDrawableRect(  rect.left(), rect.top()+ (100-iPos) * rect.height()/100, rect.width(), iPos * rect.height()/100 );
			break;
		}
	}


	m_value = iPos;
}

int KProcessView::getValue()
{
	return m_value;
}

//////////////////////////////////////////////////////////////////////////
// KProcessView
KProcessCirleView::KProcessCirleView()
{

}

KProcessCirleView::~KProcessCirleView()
{

}

void KProcessCirleView::init(REColor color)
{
	KCircleShape* p_shape2 = new KCircleShape(m_rect.width()/2, m_rect.height()/2, m_rect.width()/2 );
	REColor clr = ARGB(255, 223,223,223) ;
	p_shape2->setFillColor( clr );
	p_shape2->initPaint();
	KShapeDrawable_PTR drawable = KShapeDrawable_PTR(new KShapeDrawable( p_shape2) );
	drawable->SetRect( RERect::MakeXYWH(0,0, m_rect.width(), m_rect.height() ) );
	setBKDrawable(drawable);

	int interval = 8; //边框
	KArcShape* p_shape = new KArcShape(RERect::MakeXYWH(0,0, m_rect.width(), m_rect.height() ) );
	p_shape->setStartAngle(-90);
	p_shape->setSweepAngle(0);
	p_shape->setFillColor(color);
	p_shape->initPaint();
	m_arc_drawable = KShapeDrawable_PTR(new KShapeDrawable( p_shape) );
	m_arc_drawable->setRect(0,0,m_rect.width(), m_rect.height() );

	addDrawable(m_arc_drawable);

	//覆盖层
	p_shape2 = new KCircleShape(m_rect.width()/2, m_rect.height()/2, m_rect.width()/2 - interval );
	clr = ARGB(255, 240,240,240) ;
	p_shape2->setFillColor( clr );
	p_shape2->initPaint();
	drawable = KShapeDrawable_PTR(new KShapeDrawable( p_shape2) );
	drawable->SetRect( RERect::MakeXYWH(0,0, m_rect.width(), m_rect.height() ) );
	addDrawable(drawable);

	//Txt提到最上层
//	KDrawable_PTR tmp = m_lst_drawable[2];
	//删除老的txt
	m_lst_drawable.erase( m_lst_drawable.begin()+2 );

	m_text_drawable = KTextDrawable_PTR (new KTextDrawable(_T(""), RE_ColorBLACK, 40, REPaint::kCenter_Align) );
	//m_text_drawable->setFontName("Microsoft YaHei");
	m_text_drawable->setFont(GetFontManagerSingleton()->GetFontFromFile(_T("./resource/bebas.ttf") ));
	m_text_drawable->SetRect(RERect::MakeXYWH(0, 15, m_rect.width(), m_rect.height() ) );
	m_text_drawable->setColor(color);
	addDrawable(m_text_drawable);

	
}

void KProcessCirleView::setValue(int iPos)    //设定进度的起始位置
{
	if (m_text_drawable->isShow())
	{
		kn_string str = boost::lexical_cast<kn_string>(iPos);
		str += _T("%");
		m_text_drawable->SetText(str);
//		int w = m_text_drawable->getTextWidth();
		//RERect txt_rect = m_text_drawable->GetRect();
		//m_text_drawable->SetRect( RERect::MakeXYWH( (m_rect.width() -w)/2, txt_rect.top(), w, txt_rect.height()) );

	}

	KArcShape* p_shape = (KArcShape* )(m_arc_drawable->getShape() );
	if (p_shape)
	{
		p_shape->setSweepAngle( iPos*360/100);
	}


	m_value = iPos;
}


//////////////////////////////////////////////////////////////////////////
// KLoadingView
 
KLoadingView::KLoadingView(): KView()
{

}

KLoadingView::~KLoadingView()
{
	endAnimation();
}

// 设置动画图片
void KLoadingView::setCycleImage(const kn_string& strResPath)
{
	KImageDrawable_PTR drawable = KImageDrawable_PTR(new KImageDrawable(strResPath));
 
	REPaint* paint = drawable->GetPaint();
	paint->setFilterBitmap(true);
	paint->setAntiAlias(true);

    drawable->SetFillType(KImageDrawable::FILL_ALL);

 
	setCycleDrawable(drawable);
}

void KLoadingView::setCycleDrawable(KDrawable_PTR drawable)
{
	writeLock lock(m_lst_drawable_mutex);

	VEC_DRAWABLE::iterator it = find(m_lst_drawable.begin(), m_lst_drawable.end(), drawable);
	if(it == m_lst_drawable.end())
	{
		m_lst_drawable.push_back(drawable);
	}

	m_drawable_cycle = drawable;
}




void KLoadingView::startAnimation(kn_int iCycleTime,kn_int frame_time)
{
 	CNUIAnimation* pNUIAnimation = new CNUIAnimation (iCycleTime, 0, 0);	//  一圈

	RERect rect = m_drawable_cycle->GetRect();

	CPropertyAngle* pAngleStart = new CPropertyAngle(shared_from_this(), 0, rect.width() / 2, rect.height() / 2);
	CPropertyAngle* pAngleEnd = new CPropertyAngle(shared_from_this(), 360, rect.width() / 2, rect.height() / 2);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pAngleStart, pAngleEnd, KEasingCurve::Linear);
	pNUIAnimation->addAnimationData(pAnimationData);
	pNUIAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
	pNUIAnimation->setLoopTimes(LOOP_ALWAYS);
	pNUIAnimation->setFrameTime(frame_time);
	m_ani_thread.addAnimation(pNUIAnimation);

	m_ani_thread.Start();
}

void KLoadingView::endAnimation()
{
	m_ani_thread.Stop();
}
