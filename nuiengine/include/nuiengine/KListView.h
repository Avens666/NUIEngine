//  **************************************
//  File:        KLISTVIEW.h
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
#ifndef KLISTVIEW_DEFINED
#define KLISTVIEW_DEFINED

#include "KButtonView.h"
#include "KViewGroup.h"
#include "AnimationThread.h"
#include "KMoveableView.h"
#include "boost/thread/recursive_mutex.hpp"
#include "KShapeDrawable.h"

// 记录鼠标轨迹
struct TrackPoint
{
	kn_int y;	//	Y坐标
	kn_dword ticks;	// 时刻

	TrackPoint(int);
	TrackPoint();

};
 
class NUI_API KListView : public KViewGroup
{
public:

	enum
	{
		LS_NORMAL,
		LS_SCROLLING,	// 滑动中
		LS_ITEMCLICK 	// 点击item项

	};

	KListView() ;
	virtual ~KListView();
	virtual void shared_ptr_inited();
	// 添加自定义的View
	void UI_addItemView(KView_PTR pItemView);

	// 删掉所有Item
	void UI_clearAllItems();

	// 获取ItemView
	KView_PTR getItemView(kn_int index);

 
	// 获取列表总数
	kn_int getItemCount();

	//void setViewport(RERect rect);

	// 使位置移动落在可视范围
	int adjustInViewport(int iPosY);

	void bindData(vector<kn_string>& vData);

	// 带动画的滑动位移
	void scrollByOffsetAnimation(int y);

	// 滑动位移
	void scrollByOffset(int y);
	void scrollByPos(int y);

	// 根据ItemGroup的位置，计算滚动条的位置
	kn_int calcScrollerPos(int iItemsTop);

	void  onKeydown(KMessageKey* pMsg);

	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 

	virtual kn_bool Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight);

	// 是否显示FastScroll 
	void showFastScroller(kn_bool bShow);

	// 设置分隔符
	void setDivider(KDrawable_PTR pDrawable);
	 

	// 设置滚动条
	void setScroller();

	// 动画是否正在运行.此时不建议更新listview
	kn_bool isAnimationRunning();

	// 设置 滚动条移动到底部
	void UI_SetToBottom();

	// 是否可以拖动
	void enableDrag(kn_bool b);
	 
protected:

	void createListView();

	KTextView_PTR createListItem(RERect& rect, kn_string& strItem);

	void dragLastPhyHandler();


	// 设置快速滚动条
	void setFastScroller();

	void  onFastScrollerDown(kn_int x, kn_int y, KMessageMouse* pMsg);
	void  onFastScrollerMove(kn_int x, kn_int y, KMessageMouse* pMsg);
	void  onFastScrollerUp(kn_int x, kn_int y,  KMessageMouse* pMsg);

	void onItemClick(KView_PTR pView);

	void updateFastScrollerPos(kn_float y);

	// 判断滑动状态 ， 滑动/ 点击item项
	kn_int judgeScrollState();


	// 恢复item group 的初始设置
	void resetItemGroup();

protected:

	
	vector<kn_string> m_vec_data;

	CAnimationThread m_ani_thread;

	// 上次按下的坐标
	REPoint m_last_press_point;
	REPoint m_first_press_point;

	// 按下时listview的top
	kn_int m_i_press_top; 

	// listview可以看到的范围, 相对于上级的坐标,同m_rect的坐标
	RERect m_rect_viewport;

	// 操作的坐标点, 只记录Y轴
	vector<TrackPoint> m_vec_touchpoints;

	// 滚动条，仅在滑动时显示
	KView_PTR m_p_scroller;

	// 滚动条 drawable
	KShapeDrawable_PTR m_drawable_slider;

	// 快速滚动条
	KMoveableView_PTR m_p_fast_scroller;

	kn_bool m_b_show_scroller;

	// 列表项的集合,可以超出listview的范围
	KViewGroup_PTR m_p_item_groupview;

	// 滑动状态， 区分 listview 滑动/ Item点击  
	kn_int m_i_scroll_state; 

	// 分隔符
	KDrawable_PTR m_drawable_divider;

	// 分隔符的高
	kn_int m_i_divider_height;

	// item的高
	kn_int m_i_item_height;

	// 项的总数
	kn_int m_i_item_count;

	// 是否可拖动
	kn_bool m_b_enable_drag;

 

};

typedef boost::shared_ptr<KListView> KListView_PTR;


#endif // KLISTVIEW_DEFINED

