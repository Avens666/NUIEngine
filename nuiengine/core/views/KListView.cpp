//  **************************************
//  File:        KListView.cpp
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
#include "KListView.h"
#include "DataSync.h"
#include "KShapeDrawable.h"
#include "KScreen.h"

//////////////////////////////////////////////////////////////////////////
//	TrackPoint
TrackPoint::TrackPoint(int iCooordY)
{
	y = iCooordY;
	ticks = GetTickCount();
}

TrackPoint::TrackPoint()
{
	y = 0;
	ticks = 0;
}
 
//////////////////////////////////////////////////////////////////////////
// KListview
KListView::KListView()
{
	m_e_viewtype = KVIEW_LIST;

	m_last_press_point.set(0, 0);
	m_first_press_point.set(0, 0);

	m_i_press_top = 0;

	m_rect_viewport = m_rect;
	m_rect_viewport.offsetTo(0, 0);

	m_b_show_scroller = false;


	// 快速滚动条

	m_i_scroll_state = LS_NORMAL;

	// 分隔符的高
	m_i_divider_height = 2;

	// item的高
	m_i_item_height = 50;


	m_i_item_count = 0;

	m_b_enable_drag = true;
}




KListView::~KListView()
{
	 
}

void KListView::shared_ptr_inited()
{
	KViewGroup::shared_ptr_inited();
	m_sign_key_down.connect(this, &KListView::onKeydown);
}

// void KListView::setViewport(RERect rect)
// {
// 	m_rect_viewport = rect;
// 
// 
// }

void KListView::enableDrag(kn_bool b)
{
	m_b_enable_drag = b;
}

void KListView::bindData(vector<kn_string>& vData)
{
	m_vec_data = vData;

	//m_vec_data.swap(vData);
	createListView();	 
}

KTextView_PTR KListView::createListItem(RERect& rect, kn_string& strItem)
{
	KTextView_PTR pView = KTextView_PTR(new KTextView() );
	pView->Create(rect.left(), rect.top(), rect.width(), rect.height());

	pView->SetText(strItem);
	pView->SetFontSize(15);
	pView->SetTextBound(RERect::MakeXYWH(26, 0, rect.width() - 26, rect.height()));
	pView->SetTextColor(RE_ColorWHITE, RE_ColorRED, RE_ColorGREEN, RE_ColorBLUE);

	static KImageDrawable_PTR s_icon_image = KImageDrawable_PTR(new KImageDrawable(_T("ls_icon.png")));

	pView->setIconDrawable(s_icon_image);
	//pView->setIconImage(_T("ls_icon.png"));
	pView->setIconPos(5, 16);

	// static KImageDrawable_PTR s_bkg_image = KImageDrawable_PTR(new KImageDrawable(_T("btn_normal3.png")));
 	static KRectShape* pRectShape = NULL;
	static KShapeDrawable_PTR s_bkg_image;
	if(pRectShape == NULL)
	{
		pRectShape = new KRectShape(RERect::MakeXYWH(0, 0, 400, 50));
		s_bkg_image =  KShapeDrawable_PTR(new KShapeDrawable(pRectShape)); 
		REPaint* paint = s_bkg_image->GetPaint();
		paint->setColor(RE_ColorBLACK);

		s_bkg_image->SetRect(RERect::MakeXYWH(0, 0, 400, 50));
	}
	 
	pView->setBKDrawable(s_bkg_image);
	//pView->setBKGImage(_T("btn_normal2.png"));
	
 	return pView;
}

kn_int KListView::getItemCount()
{
  

	return m_i_item_count;
	 
}

// 添加自定义的View
void KListView::UI_addItemView(KView_PTR pItemView)
{
	//boost::recursive_mutex::scoped_lock  lock(m_message_mutex);

	// 暂不处理分隔符
	//if(m_p_item_groupview == NULL)
	//{
	//	m_p_item_groupview = new KViewGroup;
	//	resetItemGroup();
	//	AddView(m_p_item_groupview);
 //
	//}
	 
	// 自上而下添加
	RERect rectGroup = m_p_item_groupview->GetRect();
	RERect rectItem = pItemView->GetRect();

	  
	pItemView->SetRect(RERect::MakeXYWH(0, rectGroup.height(), rectItem.width(), rectItem.height()));
	m_p_item_groupview->SetRect(RERect::MakeXYWH(0, 0, rectItem.width(), rectGroup.height() +  rectItem.height()));
	  
	m_p_item_groupview->AddView(pItemView);

	m_i_item_count ++;

	// 未超出高度则不显示滚动条
	if(m_p_scroller)
	{
		kn_bool bShow = m_rect.height() < m_p_item_groupview->GetRect().height();
		m_p_scroller->SetShow(bShow);
		m_b_enable_drag = bShow;
	}
	 

   
}


// 恢复item group 的初始设置
void KListView::resetItemGroup()
{
	m_p_item_groupview->Create(0, 0, 0, 0);
	m_p_item_groupview->SetClip(RERect::MakeXYWH(0, 0, m_rect.width(), m_rect.height()));
	m_p_item_groupview->SetClipEnable(true);
	m_i_item_count = 0;
}

kn_bool KListView::Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight)
{
	KViewGroup::Create(iX, iY, iWidth, iHeight);

	if (m_p_item_groupview == NULL)
	{
		m_p_item_groupview = KViewGroup_PTR( new KViewGroup );
		resetItemGroup();
		AddView(m_p_item_groupview);
	}

	return TRUE;
}

// 删掉所有Item
void KListView::UI_clearAllItems()
{
	// 被定时器的线程调用
	//boost::recursive_mutex::scoped_lock  lock(m_message_mutex);
	//eraseView(m_p_item_groupview);
 
 
	m_p_item_groupview->enableMessage(false);

	//SAFE_DELETE(m_p_item_groupview) ;


	m_p_item_groupview->Release();  
	resetItemGroup();

	m_p_scroller->SetShow(false);

	 
	m_p_item_groupview->enableMessage(true);
 
	 //AddView(m_p_item_groupview);
 
}

KView_PTR KListView::getItemView(kn_int index)
{
	if(index < m_i_item_count)
	{
		return m_p_item_groupview->getViewByIndex(index);
	}
	KView_PTR p;
	return p;
}


void KListView::createListView()
{
	int ITEM_WIDHT = m_rect.width();
	int ITEM_HEIGHT = 50;

	// 分隔符的高
	m_i_divider_height = 2;

	// item的高
	m_i_item_height = ITEM_HEIGHT;

	// 为了共用分隔符的drawable, 把分隔符放在View里面
	KRoundRectShape* pRRect = new KRoundRectShape(RERect::MakeXYWH(0, 0, ITEM_WIDHT, m_i_divider_height), 1, 1);
	m_drawable_divider = KShapeDrawable_PTR(new KShapeDrawable(pRRect));
	m_drawable_divider->SetRect(RERect::MakeXYWH(0, m_i_item_height, ITEM_WIDHT, m_i_divider_height));
	REPaint* paint = m_drawable_divider->GetPaint();
	paint->setColor(RE_ColorWHITE);
	//RERect rectDivider = m_drawable_divider->GetRect();

//	m_p_item_groupview = new KViewGroup();

	int iItemSize = m_vec_data.size();

	m_p_item_groupview->SetRect( RERect::MakeXYWH( 0, 0, ITEM_WIDHT, m_i_item_height * iItemSize + m_i_divider_height * (iItemSize - 1) ) );

	for(int i = 0; i < iItemSize; i++)
	{
		int iRealItemHeight = m_i_item_height + m_i_divider_height;

		
		if(i == iItemSize - 1)
		{
			iRealItemHeight = m_i_item_height;
		}
		RERect rectView = RERect::MakeXYWH(0, (m_i_item_height + m_i_divider_height) * i, ITEM_WIDHT, iRealItemHeight);
		KTextView_PTR pView = createListItem(rectView, m_vec_data[i]);

		// 最后一个不加分隔符
		if(i != iItemSize - 1)
		{
			pView->addDrawable(m_drawable_divider);
		}
	

		m_p_item_groupview->AddView(pView);
		m_i_item_count++;
		 
		// 暂定为KTextViews
		(pView)->m_clicked_signal.connect(this, &KListView::onItemClick);  


	}

//	AddView(m_p_item_groupview);
 
	setScroller();
	setFastScroller();

}

void KListView::setDivider(KDrawable_PTR pDrawable)
{
	m_drawable_divider = (pDrawable);
}
 

void KListView::onItemClick(KView_PTR pView)
{
	if(m_i_scroll_state == LS_ITEMCLICK)
	{
		kn_string strItem = (VIEW_PTR(KTextView)(pView))->getText();

        //MyTrace(_T("onItemClick pView = %p,  %s"), pView, strItem.c_str());
	}

}

// 根据ItemGroup的位置，计算滚动条的位置
kn_int KListView::calcScrollerPos(int iItemsTop)
{
	RERect rectItems = m_p_item_groupview->GetRect();
 

	kn_int iPosY = m_rect.height() * abs (iItemsTop) / rectItems.height(); 

	return iPosY;
}

// 设置滚动条
void KListView::setScroller()
{
	int iWidth = 4;

	if(m_p_scroller)
	{
		// 重设滚动条的大小
		RERect rectItems = m_p_item_groupview->GetRect();
		int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

		m_p_scroller->SetRect(RERect::MakeXYWH(m_rect.width() - iWidth, 0, iWidth, iHeight));

		KRoundRectShape* pRRectShape = new KRoundRectShape (RERect::MakeXYWH(0, 0, iWidth, iHeight), 2, 2);
		m_drawable_slider->setShape(pRRectShape);

		return;
	}

	RERect rectItems = m_p_item_groupview->GetRect();

	// 把viewport在listview中的位置，映射到滚动条

	// 确认滚动条的长度
	int iHeight = m_rect.height() * m_rect.height() / rectItems.height();

	//iHeight = 20;

	// 滚动条的位置
	int iPosY = calcScrollerPos (rectItems.top());
	 	


	int iPosX = m_rect.width() - iWidth;

	m_p_scroller =KView_PTR(  new KView );
	m_p_scroller->Create(iPosX, iPosY, iWidth, iHeight);


	KRoundRectShape* pRRectShape = new KRoundRectShape (RERect::MakeXYWH(0, 0, iWidth, iHeight), 2, 2);
	m_drawable_slider = KShapeDrawable_PTR(new KShapeDrawable (pRRectShape));

	m_drawable_slider->SetRect(RERect::MakeXYWH(0, 0, iWidth, iHeight));
	REPaint* pRRPaint = m_drawable_slider->GetPaint();
	//pRRPaint->setColor(RE_ColorRED);

	
	pRRPaint->setColor(RE_ColorWHITE);

	m_p_scroller->addDrawable(m_drawable_slider);
	m_p_scroller->SetShow(m_rect.height() < m_p_item_groupview->GetRect().height());
	AddView(m_p_scroller);

 
		
 

	 
}

void KListView::showFastScroller(kn_bool bShow)
{
	if(m_p_fast_scroller)
	{
		m_p_fast_scroller->SetShow(bShow);
	}

}

// 快速滚动条
void KListView::setFastScroller()
{
	if(m_p_fast_scroller)
	{
		return;
	}

	KImageDrawable_PTR pDrawableFastScroller = KImageDrawable_PTR(new KImageDrawable (_T("fast_scroller.png")));
	pDrawableFastScroller->SizeToImage();

	RERect rectFS = pDrawableFastScroller->GetRect();
	RERect rectItems = m_p_item_groupview->GetRect();

	// 滚动条的位置
	int iPosY = calcScrollerPos (rectItems.top());
	int iPosX = m_rect.width() - rectFS.width();
	 

	m_p_fast_scroller = KMoveableView_PTR(new KMoveableView);
	m_p_fast_scroller->Create(iPosX, iPosY, rectFS.width(), rectFS.height());
	m_p_fast_scroller->addDrawable(pDrawableFastScroller);



	// 快速scroller
	m_p_fast_scroller->m_sign_down.disconnect_all();
	m_p_fast_scroller->m_sign_move.disconnect_all();
	m_p_fast_scroller->m_sign_up.disconnect_all();


	//	不能用OnDown,否则也会触发GroupView的OnDown
	m_p_fast_scroller->m_sign_down.connect(this, &KListView::onFastScrollerDown);
	m_p_fast_scroller->m_sign_move.connect(this, &KListView::onFastScrollerMove);
	m_p_fast_scroller->m_sign_up.connect(this, &KListView::onFastScrollerUp);


	AddView(m_p_fast_scroller);
		 
}


void KListView::scrollByOffsetAnimation(int y)
{
	if(m_ani_thread.getRunState() != CAnimationThread::STOP)
	{

		MyTraceA("scrollByOffsetAnimation m_ani_thread stop....");
		return;
		m_ani_thread.Stop();
	}
	 
	CNUIAnimation* pNUIAnimation = new CNUIAnimation (1000, 0, 0);	

	// 设置group的位置
	int iNewPos = adjustInViewport(m_p_item_groupview->GetRect().top() + y);
	CPropertyPos* pPos = new CPropertyPos(m_p_item_groupview, m_p_item_groupview->GetRect().left(), iNewPos);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPos, KEasingCurve::OutCirc);
	pNUIAnimation->addAnimationData(pAnimationData);
 
	// 设置滚动条位置
	int iScrollY = calcScrollerPos(iNewPos);
	CPropertyPos* pPosScroller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iScrollY);
	CPropertyAnimationData* pAnimationDataScroller = new CPropertyAnimationData(pPosScroller, KEasingCurve::OutCirc);
	pNUIAnimation->addAnimationData(pAnimationDataScroller);

	pNUIAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
	pNUIAnimation->setLoopTimes(1);
	 
	// 必须先清空, 否则多次启动线程会挂掉
	m_ani_thread.clearAnimation();
	kn_bool bOK = m_ani_thread.addAnimation(pNUIAnimation);
	//ASSERT(bOK);
	if(bOK)
	{
		m_ani_thread.Start();
	}
	else
	{
		SAFE_DELETE(pNUIAnimation);
	}

	 
}

// 确保item_group 在 viewport之内
int KListView::adjustInViewport(int iPosY)
{
	int iNewTop = iPosY;


	if(iNewTop > 0)
	{
		iNewTop = 0;
	}
	else if(iNewTop + m_p_item_groupview->GetRect().height() < m_rect.height())
	{
		iNewTop = m_rect.height() - m_p_item_groupview->GetRect().height();
	}

	return iNewTop;
	 
}

void KListView::scrollByOffset(int y)
{
 
	// 设置group的位置
	int iNewPos = adjustInViewport(m_p_item_groupview->GetRect().top() + y);
	CPropertyPos* pPos = new CPropertyPos(m_p_item_groupview, m_p_item_groupview->GetRect().left(), iNewPos);
 

	// 设置滚动条位置
	int iScrollY = calcScrollerPos(iNewPos);
	CPropertyPos* pPosScroller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iScrollY);
 
	GetScreen()->addProperty(pPos);
	GetScreen()->addProperty(pPosScroller);

	UpdateUI();
}

void KListView::scrollByPos(int y)
{
	if(m_p_item_groupview == NULL || m_p_scroller == NULL)
	{
		return;
	}

	// item_group 新的top位置
	int iNewPosY = y - m_first_press_point.fY + m_i_press_top; 

	int iYPos = adjustInViewport(iNewPosY);
 

 	CPropertyPos* pPos = new CPropertyPos(m_p_item_groupview, m_p_item_groupview->GetRect().left(), iYPos);
 	GetScreen()->addProperty(pPos);

	int iYScoller = calcScrollerPos(iYPos);
	CPropertyPos* pPosScroller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iYScoller);
	GetScreen()->addProperty(pPosScroller);


 	UpdateUI();

	return;

	// 设置各个view, 数据项太多时比较慢
	//KView* pTopView = m_lst_view[0];
	//RERect rectTop = pTopView->GetRect();

	//for(int i = 0; i < m_lst_view.size(); i++)
	//{
	//	KView* pView = m_lst_view[i];
	//	RERect rect = pView->GetRect();

	//	CPropertyPos* pPos = new CPropertyPos(pView, rect.left(), rect.top() - rectTop.top() + dy);
	//	GetScreen()->addProperty(pPos);
 //	}

	//UpdateUI();
}
void KListView::onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	//MyTraceA("-----onDownDirect ");
	if(!m_b_enable_drag)
	{
		return;
	}

	m_last_press_point.set(iScreenX, iScreenY);
	m_first_press_point.set(iScreenX, iScreenY);

	m_vec_touchpoints.clear();
	m_vec_touchpoints.push_back(TrackPoint(iScreenY));

	m_i_scroll_state = LS_NORMAL;

	// 按下时第一item view的top
	if(m_p_item_groupview)
	{
		// 对于各个item view
		//m_i_press_top = m_lst_view[0]->GetRect().top();
	  
		// 对于viewgroup
		//m_i_press_top = m_rect.top();
		m_i_press_top = m_p_item_groupview->GetRect().top();

		// 停止动画
		if(m_ani_thread.getRunState() != CAnimationThread::STOP)
		{		 
			m_ani_thread.Stop();
		}
	}
	
}

// kn_bool KListView::IsMousePicked();
// {
// 	return m_b_mouse_picked;
// }


void KListView::onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(!m_b_enable_drag)
	{
		return;
	}

	// 快速拖动时不处理鼠标拖动
	if(m_p_fast_scroller != NULL && m_p_fast_scroller->IsMousePicked())
	{
		return;
	}

	if(m_b_mouse_picked)
	{
		// 避免频繁调用
		if(abs((int)m_last_press_point.fY -  iScreenY) > 5)
		{
			//MyTraceA("-----onMoveDirect  MOVING......");
			scrollByPos(iScreenY);
			m_last_press_point.set(iScreenX, iScreenY);
 
			m_vec_touchpoints.push_back(TrackPoint(iScreenY));

			m_i_scroll_state = LS_SCROLLING;
 
		}
	}
}

void KListView::onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg)
{
	if(!m_b_enable_drag)
	{
		return;
	}

	//MyTraceA("-----onUpDirect ");

	m_vec_touchpoints.push_back(TrackPoint(iScreenY));

	m_i_scroll_state = judgeScrollState();

	//	惯性效果 
	dragLastPhyHandler();

}


// 判断滑动状态
kn_int KListView::judgeScrollState()
{
	if(m_vec_touchpoints.size() < 2)
	{
		return LS_NORMAL;
	}

	int dy = 0;
	for(kn_uint i = 0; i < m_vec_touchpoints.size() - 1; i++)
	{
		dy += abs(m_vec_touchpoints[i].y - m_vec_touchpoints[i+1].y);
	}

	kn_int iState = LS_ITEMCLICK;

	// 位移大于10像素即为滑动
	if(dy > 10)
	{
		iState = LS_SCROLLING;
	}
	
	return iState;
}

//	处理拖拽最后的物理效果
void  KListView::dragLastPhyHandler()
{
	int iArraySize = m_vec_touchpoints.size();
	if(iArraySize < 2)
	{
		return;
	}


	TrackPoint p0;
	TrackPoint p1;

	int count = 0;
	int index1 = -1; 
	int index2 = -1;	// 找最后2个时间点
	for(int i = iArraySize - 1; i > 0; i--)
	{
		// 避免重复的时间点
		if(m_vec_touchpoints[i].ticks != m_vec_touchpoints[i-1].ticks)
		{
			count++;
		}

		if(count == 1)
		{
			index1 = i-1;
		}
		else if(count == 2)
		{
			index2 = i-1;
			break;

		}
		
	}

	if(index2 != -1)
	{
		p0 = m_vec_touchpoints[index2];
		p1 = m_vec_touchpoints[iArraySize - 1];

	}
	else if(index1 != -1)
	{
		p0 = m_vec_touchpoints[index1];
		p1 = m_vec_touchpoints[iArraySize - 1];
	}
	else
	{
		//ASSERT(0);
		return;
	}

	// 暂时以像素为位移,后续设备需考虑不同的dpi,使用物理长度为位移
	float v0 = 0; // 初速度
	if(p1.ticks != p0.ticks)
	{
		
		v0 = (float)(p1.y - p0.y) / (p1.ticks - p0.ticks);
	}
	else
	{
		ASSERT(0);
		return;
		
	}

	// float小于1会近似变成0
	if((int)v0  == 0)
	{
 
		return;
	}


	//	与速度反向的加速度 每毫秒移动的像素值
 	float a = 0.001;
	if(v0 > 0)
	{
		a *= -1;
	}

	// 停下所需的时间ms
	float t = -v0 / a; 

  	// 位移像素值
	float s = v0 * t + 0.5 * a * t * t;

	MyTraceA("s = %f, t = %f, v0 = %f, a = %f", s, t, v0, a);

	// 添加动画 
	if(m_ani_thread.getRunState() != CAnimationThread::STOP)
	{		 
		m_ani_thread.Stop();
	}

	CNUIAnimation* pNUIAnimation = new CNUIAnimation (t, 0, 0);	

	int iNewPos = adjustInViewport(m_p_item_groupview->GetRect().top() + s);

	CPropertyPos* pPos = new CPropertyPos(m_p_item_groupview, m_p_item_groupview->GetRect().left(), iNewPos);
	CPropertyAnimationData* pAnimationData = new CPropertyAnimationData(pPos, KEasingCurve::OutQuart);  
	pNUIAnimation->addAnimationData(pAnimationData);


	int iYScoller = calcScrollerPos(iNewPos);
	CPropertyPos* pPosScroller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iYScoller);
	CPropertyAnimationData* pAnimationDataScroller = new CPropertyAnimationData(pPosScroller, KEasingCurve::OutQuart);
	pNUIAnimation->addAnimationData(pAnimationDataScroller);

	pNUIAnimation->setLoopType(CNUIAnimation::LOOP_TIME);
	pNUIAnimation->setLoopTimes(1);

	// 必须先清空, 否则多次启动线程会挂掉
	m_ani_thread.clearAnimation();
	m_ani_thread.addAnimation(pNUIAnimation);
	m_ani_thread.Start();

}



void KListView::onWheelDirect(KMessageMouseWheel* pMsg)
{
	//MyTraceA("-----onUpDirect ");

	kn_int iOffsetY = (pMsg->m_roll / 120) * 50;
	scrollByOffset(iOffsetY);
}

void KListView::onKeydown(KMessageKey* pMsg)
{
 
	if(!m_b_enable_drag)
	{
		return;
	}

	// 避免ctrl 或 alt按下
	if(pMsg->m_key_flag == 0)
	{
		if(pMsg->m_key_id == VK_NEXT) // page down
		{
			scrollByOffsetAnimation(-m_rect.height());
		}
		else if(pMsg->m_key_id == VK_PRIOR)	// page up
		{
			scrollByOffsetAnimation(m_rect.height());
		}
		else if(pMsg->m_key_id == VK_END)
		{
			scrollByOffsetAnimation(-m_p_item_groupview->GetRect().height());
		}
		else if(pMsg->m_key_id == VK_HOME)
		{
			scrollByOffsetAnimation(m_p_item_groupview->GetRect().height());
		}
	}
}

void KListView::onFastScrollerDown(kn_int x, kn_int y,KMessageMouse* pMsg)
{
	MyTraceA("onFastScrollerDown y = %d", y);
}

void KListView::onFastScrollerMove(kn_int x, kn_int y,KMessageMouse* pMsg)
{
	MyTraceA("onFastScrollerMove y = %d", y);

	if(m_p_fast_scroller->IsMousePicked())
	{
		updateFastScrollerPos(y);

	}
}

void KListView::onFastScrollerUp(kn_int x, kn_int y,KMessageMouse* pMsg)
{
	MyTraceA("onFastScrollerUp y = %d", y);
}


// 更新位置, Y 为slider的中心位置
void KListView::updateFastScrollerPos(kn_float y)
{
	RERect rectSlider = m_p_fast_scroller->GetRect();

	// 处理边界
	int YMin = rectSlider.height() / 2;
	int YMax = m_rect.height() - rectSlider.height()/2;

	if(y <  YMin)
	{
		y = YMin;
	}

	if(y > YMax)
	{
		y = YMax;
	}

	// 重置fast scroller的位置
	float fSliderTop = y - (float)rectSlider.height() / 2;
	if(fSliderTop < 0)
	{
		fSliderTop = 0;
	}
	int iSliderTop =  fSliderTop;
	CPropertyPos* pFastScroller = new CPropertyPos(m_p_fast_scroller, rectSlider.left(), iSliderTop);

	// 重置ItemGroup的位置
	RERect rectItems = m_p_item_groupview->GetRect();
	int iItemPosY = - (y - (float)rectSlider.height()/2)  * (rectItems.height() - m_rect.height()) / (m_rect.height() - rectSlider.height());
	CPropertyPos* pItems = new CPropertyPos(m_p_item_groupview, rectItems.left(), iItemPosY);
	
	// 重置scroller的位置
	int iScollerY = calcScrollerPos(iItemPosY);
	CPropertyPos* pScoller = new CPropertyPos(m_p_scroller, m_p_scroller->GetRect().left(), iScollerY);

	GetScreen()->addProperty(pFastScroller);
	GetScreen()->addProperty(pItems);
	GetScreen()->addProperty(pScoller);
	UpdateUI();
}



kn_bool KListView::isAnimationRunning()
{
	return (m_ani_thread.getRunState() != CNUIAnimation::STOP);
}


// 设置 滚动条移动到底部, 仅供UI_DoSyncDataList() 使用
void KListView::UI_SetToBottom()
{
	RERect rectItemGroup = m_p_item_groupview->GetRect();

	if(rectItemGroup.height() > m_rect.height())
	{
		float dy = m_rect.height() - rectItemGroup.height();
		RERect rectNew = rectItemGroup;
		rectNew.offset(0, dy);

		m_p_item_groupview->SetRect(rectNew);

		if(m_p_scroller)
		{
			RERect rectScroller =  m_p_scroller->GetRect();
			int iPos = m_rect.height() - rectScroller.height();
			rectScroller.offsetTo(rectScroller.left(), iPos);
			m_p_scroller->SetRect(rectScroller);

		}
	}
}
