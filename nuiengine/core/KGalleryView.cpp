//  **************************************
//  File:        KGalleryView.cpp
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
#include "KGalleryView.h"
#include "KImageDrawable.h"
#include "DataSync.h"
#include "KScreen.h"
//////////////////////////////////////////////////////////////////////////
//	KGalleryItemView


KGalleryItemView::KGalleryItemView()
{
	m_e_viewtype = KVIEW_GALLERY_ITEM;

}

KGalleryItemView::~KGalleryItemView()
{

}

void KGalleryItemView::SetFileName(kn_string&  str)
{
	m_strFileName = str;
}

kn_string KGalleryItemView::GetFileName()
{
	return m_strFileName;
}



//////////////////////////////////////////////////////////////////////////
//	KGalleryView

KGalleryView::KGalleryView()
{
	m_e_viewtype = KVIEW_GALLERY;

	m_i_thum_width = 200;
	m_i_thum_height = 140;

 

	m_i_horizontal_interval = 20;
	m_i_vertical_interval = 20;
}

KGalleryView::~KGalleryView()
{


}


void KGalleryView::setGridStyle(kn_int w, kn_int h, kn_int hi, kn_int vi)
{
	 m_i_thum_width = w;
	 m_i_thum_height = h;
	 m_i_horizontal_interval = hi;
	 m_i_vertical_interval = vi;


}

 // 设置drawable列表
//void KGalleryView::bindDrawables(vector<KDrawable_PTR>& vDrawables)
//{
//
//}

 // 设置图片文件列表
kn_bool KGalleryView::bindFilelist (vector<kn_string>& vFlieList)
{
	m_p_thum_groupview = KViewGroup_PTR( new KViewGroup );
	m_p_thum_groupview->Create(0, 0, m_rect.width(), m_rect.height());

	// 设置布局
	kn_int col = m_rect.width() / (m_i_thum_width + m_i_horizontal_interval);
	kn_int row = m_rect.height() / (m_i_thum_height + m_i_vertical_interval);

	kn_uint iMaxInPage = col * row; 

	for(kn_uint i = 0; i < vFlieList.size() && i < iMaxInPage; i++)
	{ 
		IRESurface* pSurface = RESurfaceFactory::CreateRESurface();
		kn_bool bOK = pSurface->InitializeThumbnailFromFile(vFlieList[i], m_i_thum_width, m_i_thum_height, RE_FIT_WIDTH, RE_ColorBLACK);
		if(!bOK)
		{
			ASSERT(0);
			continue;
		}

		KImageDrawable_PTR imagedrawable = KImageDrawable_PTR(new KImageDrawable (pSurface));

		KGalleryItemView_PTR pView = KGalleryItemView_PTR(new KGalleryItemView);

		int x = (i % col) * (m_i_thum_width + m_i_horizontal_interval);
		int y = (i / col) * (m_i_thum_height + m_i_vertical_interval);

		pView->Create(x, y, m_i_thum_width, m_i_thum_height);
		pView->setBKDrawable(imagedrawable);

		//pView->setBKGImage(vFlieList[i]);

		// 设置单个项的点击事件
		pView->m_clicked_signal.connect(this, &KGalleryView::onItemClick);  
		pView->SetFileName(vFlieList[i]);
		m_p_thum_groupview->AddView(pView);
 
	}


	AddView(m_p_thum_groupview);

	return true;

}

void KGalleryView::onItemClick(KView_PTR pView)
{
	KGalleryItemView_PTR pItemView = VIEW_PTR(KGalleryItemView)(pView->shared_from_this() );

	MyTraceA ("onItemClick pItemView str = %S", pItemView->GetFileName().c_str());

	pItemView->setViewFocus();

	// 设置大图
	//pItemView->setBKGImage(pItemView->GetFileName());
	// kn_bool bEnlarge = (m_rect.width() > 400);

   kn_bool bEnlarge = false;

	if(bEnlarge)
	{
		RERect rect = pItemView->GetBoundRect();

		float factor = rect.width() / rect.height();
		rect.outset(factor * 200 , 200);

		CPropertyRect * pPropertyRect = new CPropertyRect (pItemView, rect);

		CNUIAnimation* pNUIAnimation = new CNUIAnimation (1000, 0, 0);	
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPropertyRect, KEasingCurve::OutCirc);
		pNUIAnimation->addAnimationData(pAnimationData);

		pNUIAnimation->setLoopType(CNUIAnimation::LOOP_PINGPONG);
		pNUIAnimation->setLoopTimes(100);

		m_ani_thread.clearAnimation();
		m_ani_thread.addAnimation(pNUIAnimation);
		m_ani_thread.Start();
	}
	else
	{
		RERect rect = pItemView->GetBoundRect();
		pItemView->SetClip(rect);

		RERect rectTarget = rect;
		rectTarget.inset(300, 200);



		CPropertyClip * pPropertyClip = new CPropertyClip (pItemView, rectTarget);
		CNUIAnimation* pNUIAnimation = new CNUIAnimation (2000, 0, 0);	
		CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPropertyClip );
		pNUIAnimation->addAnimationData(pAnimationData);

		pNUIAnimation->setLoopType(CNUIAnimation::LOOP_PINGPONG);
		pNUIAnimation->setLoopTimes(10000);

		m_ani_thread.clearAnimation();
		m_ani_thread.addAnimation(pNUIAnimation);
		m_ani_thread.Start();

	}





	 
}
