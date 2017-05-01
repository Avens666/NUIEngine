//  **************************************
//  File:        KViewGroup.h
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
#ifndef K_PAGE_VIEW_H
#define K_PAGE_VIEW_H

#include "KView.h"
#include "KDrawable.h"
//#include "KButtonView.h"
//#include "KEditView.h"
#include <boost/thread/thread.hpp>
#include "KTextView.h"
#include "KButtonView.h"
#include "KEditView.h"


#define ICON_SENTER 2000

class KTextView;
class KStaticView;
class KEditView;
class KImgButtonView;

//class NUI_API KViewGroup : public KObject
class NUI_API KViewGroup : public KView
{
protected:
	boost::shared_mutex m_lst_view_mutex;
	LSTVIEW m_lst_view;
	IRESurface* m_pSurface;

	LSTVIEW m_del_lst_view; //等待被删除的view，view在自己的消息循环中不能删除自己，到消息循环结束清除

	kn_bool m_b_do_clip_msg;//只响应Clip区域的消息

	//viewSizeChange变化时，是否调用子view的viewSizeChange，有的组合控件（grid list等），自己管理布局，不用通过viewSizeChange机制
	kn_bool m_b_change_child_size;

public:
	KViewGroup();
	virtual ~KViewGroup();
	virtual void Release();

	virtual void Draw(IRESurface* pDstSurface, kn_int x, kn_int y);
	virtual void Draw(IRESurface* pDstSurface, LSTRECT& lst_rect, kn_int x, kn_int y);

	virtual void AddView(KView_PTR v);
	virtual void AddViewToDel(KView_PTR v);

	virtual void SaveOriginalViewRect();

	//在后台时睡眠
	virtual void SleepPage();
	virtual void Wakeup();

	virtual LSTRECT GetInvalidRect();
	kn_bool DoMessage(KMessage* pMsg);

	//是否存在引用计数
//	virtual kn_bool isRef();

	//子view层级操作
	kn_int findViewIndex(KView_PTR v);
	void swapViewLayer(KView_PTR v1, KView_PTR v2);
	void changeViewLayerTop(KView_PTR v);
	void changeViewLayerBottom(KView_PTR v);
	void changeViewLayerUp(KView_PTR v);
	void changeViewLayerDown(KView_PTR v);
	void moveViewLayer(kn_int old_index, kn_int new_index);

	virtual void eraseView(KView_PTR v);

	KView_PTR getViewByIndex(kn_int index);
	kn_int getViewCount();

	virtual void viewSizeChange();
	virtual void resize(RERect);
	RERect getDrawRectBaseOnChild();
	
	//方便应用层快速增加view对象, 对象在内部创建
	void createImgViewHelper(KImgButtonView_PTR* view,const kn_string& normalPath,const kn_string& activePath, const kn_string& focusPath,kn_int x,kn_int y);
	void createImgView9PatchHelper(KImgButtonView_PTR* view,const kn_string& normalPath,const kn_string& activePath, const kn_string& focusPath,kn_int x,kn_int y, kn_int w, kn_int h);
	// icon_x = ICON_SENTER 居中
	void createImgView9PatchIconHelper(KImgButtonView_PTR* view,const kn_string& normalPath,const kn_string& activePath, const kn_string& focusPath,const kn_string& iconPath, kn_int x,kn_int y, kn_int w, kn_int h, kn_int icon_x, kn_int icon_y);
	
	//带背景图的，宽高自动适应背景，背景图获取失败的，使用传入的宽高值
	void createTextViewHelper(KTextView_PTR* view,const kn_string& imgPath,kn_int x,kn_int y,  kn_int w=100, kn_int h=50);

	void createImageHelper(KStaticView_PTR* view,const kn_string& path, kn_int x,kn_int y);
	void createTextHelper(KStaticView_PTR* view,const kn_string& txt, int fontsize, kn_int x,kn_int y, kn_int w, kn_int h);
	void createEditViewHelper(KEditView_PTR* view, int fontsize, kn_int x,kn_int y, kn_int w, kn_int h);
	void EnableClipMessage(kn_bool b);
};

typedef boost::shared_ptr<KViewGroup> KViewGroup_PTR;
//group需要生命弱智能指针 避免父子循环指向
typedef boost::weak_ptr<KViewGroup> KViewGroup_WEAK_PTR;
#endif
