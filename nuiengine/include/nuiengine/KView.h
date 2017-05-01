//  **************************************
//  File:        KView.h
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
#ifndef K_VIEW_H
#define K_VIEW_H

#include "renderingengine.h"
#include "sigslot.h"
#include <boost/thread/thread.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include "REPaint.h"
#include "RERect.h"
#include "REMatrix.h"
#include "KDrawable.h"
#include "KObject.h"
#include "message.h"
#include "KLog.h"
#include "KTimer.h"
#include "KTextDrawable.h"
#include "KColorDrawable.h"
#include "KGestureDetector.h"
using namespace sigslot;
#define NO_TIMER -1
class KViewGroup;
typedef boost::shared_ptr<KViewGroup> KViewGroup_PTR;
typedef boost::weak_ptr<KViewGroup> KViewGroup_WEAK_PTR;
class KMenuBase;
typedef boost::shared_ptr<KMenuBase> KMenuBase_PTR;
enum KVIEWTYPE
{
	KVIEW_BASE = 0,
	KVIEW_WINDOW = 1,
	KVIEW_BUTTON = 2,
	KVIEW_TEXT = 3,
	KVIEW_LIST = 4,
	KVIEW_CHECKBOX = 5,
	KVIEW_RADIOBOX = 6,
	KVIEW_SLIDER = 7,
	KVIEW_PROGRESS = 8,
	KVIEW_IMAGECTRL = 9,
	KVIEW_MOVEABLE = 10,
	KVIEW_STATIC = 11,
	KVIEW_RADIOBOXGROUP = 12,
	KVIEW_GALLERY = 13,
	KVIEW_GALLERY_ITEM = 14,
	KVIEW_LIST_ITEM = 15,
	KVIEW_EDIT = 16,
	KVIEW_MENU = 17,
	KVIEW_GROUP = 18
};

enum VIEW_TRANSFORM_FLAG
{
	VIEW_TRANSFORM_ROTATE = 1,
	VIEW_TRANSFORM_SCALE  = 2,
	VIEW_TRANSFORM_TRANSLATE = 4,
	VIEW_TRANSFORM_SKEW = 8,
	VIEW_TRANSFORM_BOUND = 16,

	VIEW_TRANSFORM_ALL = 0xffff,
};


// 可动画的属性参数定义
#define PropertyPARA_ID_POS_X		1
#define PropertyPARA_ID_POS_Y		2
#define PropertyPARA_ID_Opacity		3
#define PropertyPARA_ID_RotateAngle	4
#define PropertyPARA_ID_ScaleX		5
#define PropertyPARA_ID_ScaleY		6
#define PropertyPARA_ID_View		200
#define PropertyPARA_ID_USER		1000 //应用层从这个开始定义

//模态窗口的返回值
#define KN_REUSLT_OK			0
#define KN_REUSLT_CANCLE		1
#define KN_REUSLT_USER			100

class KScreen;
class KTextView;
KScreen* GetNuiScreen();
class NUI_API KView : public KShowObject, public sigslot::has_slots<>, public boost::enable_shared_from_this<KView>
{
public:
	KView();
	virtual ~KView();
	//zhic 扩展的智能指针机制，在构造函数结束被自动调用，解决构造函数不能获得shared_from_this问题，运行到这个函数时已可获得
	//注意子类如果重载必须要先调用基类函数
	virtual void shared_ptr_inited(); 

	virtual void Release();
	virtual kn_bool Create(kn_int iX, kn_int iY, kn_int iWidth, kn_int iHeight);
    virtual kn_bool Create(const RERect& rect);

	//除了groupview  各子类不要继承draw函数,异步渲染机制下，draw，RefreshSurface是独立线程调用
	virtual void Draw(IRESurface* pDstSurface, kn_int x, kn_int y);
	virtual void Draw(IRESurface* pDstSurface, LSTRECT& rect, kn_int x, kn_int y);
	virtual kn_bool DoMessage(KMessage* pMsg);
	virtual kn_bool DoFocusMessage(KMessage* pMsg);

	//绘制view到指定surface，有cache时是本地surface，无cache是主surface
	virtual void RefreshSurface(IRESurface* pDstSurface, kn_int x, kn_int y);

	virtual void RefreshCache();

	void	enableCache(kn_bool b);
	KScreen* GetScreen();
	void SetScreen(KScreen * p);
	KVIEWTYPE getType();

	//创建drawable的简化函数 根据需要可支持更多类别
    void createTextDrawableHelper(KTextDrawable_PTR* text_da_,kn_string str, int fontsize, kn_int x,kn_int y, kn_int w, kn_int h, REColor r = RE_ColorBLACK, REPaint::Align align= REPaint::kLeft_Align);
	void createColorDrawableHelper(KColorDrawable_PTR* color_da, kn_int x,kn_int y, kn_int w, kn_int h, REColor r = RE_ColorBLACK);

	//通过这个函数可以设置是否让位置消息向下层传递
	void	SetShieldMsg(kn_bool b);

	virtual kn_uint addDrawable(KDrawable_PTR); //返回加入的drawable的索引位置

	void enableMessage(kn_bool b);

	virtual void GetPosition(kn_int& iX, kn_int& iY);
	virtual REPoint GetPosition();
	virtual void GetScreenXY(kn_int &x, kn_int &y);
	virtual void SetScreenPosition(kn_int iX, kn_int iY);
	virtual void SetPosition(kn_int iX, kn_int iY);
	virtual void SetPositionX(kn_int iX);
	virtual void SetPositionY(kn_int iY);
	virtual kn_bool HasInvalidRect();
	virtual LSTRECT GetInvalidRect();
	virtual void SetParent(KViewGroup_WEAK_PTR pPageView);
	virtual KViewGroup_PTR GetParent();

	//////////// 针对异步渲染提供的异步设置参数函数 更安全
	void requestSetPosition(kn_int iX, kn_int iY);
	///////////////

	virtual void UpdateUI ();
	// 设置局部刷新区域
	//传入的坐标相对于view本身
	virtual void InvalidateRect(RERect r);
	//刷新控件自身
	virtual void InvalidateView(kn_bool bRefresh=TRUE);
	//传入的坐标相对于view上级
	virtual void InvalidateForParent(RERect& r);
	virtual void clearInvalidRect();

	// 检查点是否在view内部，除了view范围，还有裁剪区处理，应统一使用这个函数,坐标为parent坐标系
	virtual kn_bool isPointInView(int x, int y);
	// 获取位置大小
//	virtual RERect GetRect();
	virtual void SetRect(const RERect& rect);
	// 变换操作
	// 获取变换后的包围盒
	virtual RERect GetBoundRect();
	////////变形函数统一在绘制时处理矩阵，这些函数仅仅只是设置对应变换值，因为要在绘制时转为屏幕坐标

	// 平移, 非累加
	virtual void SetTranslate(REScalar dx, REScalar dy);
	// 缩放, 非累加
	virtual void SetScale(REScalar sx, REScalar sy, REScalar px = 0, REScalar py = 0);
	virtual void SetScalePoint( REScalar px, REScalar py );
	virtual void SetScaleX( REScalar sx );
	virtual void SetScaleY( REScalar sy );

	// 旋转, 非累加
	virtual void SetRotatePoint( REScalar px = 0, REScalar py = 0);
	virtual void SetRotateAngle( kn_float degrees);
	virtual void SetRotate(kn_float degrees, REScalar px = 0, REScalar py = 0);
	// 倾斜, 非累加
	virtual void SetSkew(kn_float sx, kn_float sy, REScalar px = 0, REScalar py = 0);
	virtual void SetBoundRect(const RERect& rect);

	//根据设置的变形参数获得变形矩阵，传入为view相对坐标系的屏幕坐标
	virtual void getMatrix( REScalar x, REScalar y, REMatrix& mat);


	// Slot
	//传入坐标为相对父group的坐标
	virtual void OnMove(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnRDown(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnRUp(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnRDClick(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg);
	virtual void OnWheel(KMessageMouseWheel* pMsg);

	virtual void OnTouchMove(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg);
	virtual void OnTouchDown(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg);
	virtual void OnTouchUp(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg);

	virtual void OnGesture(KGesture* pGesture);

	//把view内部的内容渲染到一个image上,构造成TextView返回
	IRESurface* renderToSurface();
	KTextView* renderToView();

	// DoMessage时直接调用,不经信号槽传递,供控件内部使用
	virtual void onDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageMouse* pMsg); 
	virtual void onWheelDirect(KMessageMouseWheel* pMsg); 
	virtual void onDragDirect(KMessageDrag* mag);
	virtual kn_bool OnUserMsg(KMessage* pMsg);

	virtual void onTouchDownDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg); 
	virtual void onTouchMoveDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg); 
	virtual void onTouchUpDirect(kn_int iScreenX, kn_int iScreenY, KMessageTouch* pMsg); 

	void recogGesture(KMessageTouch* pMsg);

	void enableGesture(kn_bool b);

	void handleMouseDown(KMessage* pMsg);
	void handleMouseMove(KMessage* pMsg);
	void handleMouseUp(KMessage* pMsg);

	kn_bool IsMousePicked();

	REScalar getRotateAngle();
	void getRotatePoint(REScalar& x, REScalar& y);

	//设为聚焦控件
	virtual void setViewFocus();
	virtual void unSetViewFocus();
	kn_bool bViewFocus();

	virtual void setViewActive();
	virtual void unSetViewActive(kn_bool unsetScreen = TRUE);

	// 模式对话框，暂未实现多层doModal的情况
	kn_int doModal();
	//b_del_view为真 删除view
	void endModal(kn_int iResult, kn_bool b_del_view = FALSE);
	//	模式对话框消息循环,该函数实现与平台相关,windows实现在mainforwindows.cpp,不在KView.cpp 
	int runModalLoop();	

	////用于通用型动画参数，不新增属性类实现动画参数时使用
	virtual kn_bool isChange(kn_int para_id, kn_double v);
	virtual void doChange(kn_int para_id, kn_double v);
	virtual kn_double getParaValue(kn_int para_id);

	RERect GetScreenRect(RERect);

	void showMenu(KMenuBase_PTR p_menu, int x, int y);


	virtual void viewSizeChange();
	//void SetViewChangeType(IN LRMethod LeftType,IN LRMethod RightType,IN BTMethod TopType,IN BTMethod BottomType);
	virtual void SaveOriginalViewRect();
	virtual RERect getDrawRectBaseOnChild();

	//使用智能指针 不再需要引用计数
	//增加引用计数
	//void addRef();
	////降低引用计数
	//void decRef();
	////是否存在引用计数
	//virtual kn_bool isRef();

    void setName(const kn_string& );
	kn_string& getName();

	void setID(kn_int id);
	kn_int getID();

    void setTip(const kn_string& s, kn_int t1 = NO_TIMER, kn_int t2 = NO_TIMER);
	void enableTip(kn_bool b);
	virtual void showTip(int delay_time, int hide_time);//延迟显示时间 自动隐藏时间 -1为不延迟和不隐藏
	void hideTip();
	void showTipCallback(int);
	void hideTipCallback(int);

	// signs
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_move;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_down;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_up;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_dclick;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_rdown;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_rup;
	sigslot::signal3<kn_int, kn_int,KMessageMouse*> m_sign_rdclick;

	// signal
	sigslot::signal3<kn_int, kn_int, KMessageMouse* > m_clicked_pos_signal;
	sigslot::signal1<KView_PTR> m_clicked_signal;	//	参数为sender view

	sigslot::signal0<> m_sign_focus;
	sigslot::signal0<> m_sign_unfocus;

	sigslot::signal0<> m_sign_active;
	sigslot::signal0<> m_sign_unactive;

	sigslot::signal0<> m_mouse_enter_signal;
	sigslot::signal0<> m_mouse_leave_signal;

	sigslot::signal1<KMessageKey*> m_sign_key_down;
	sigslot::signal1<KMessageKey*> m_sign_key_up;
	sigslot::signal1<KMessageMouseWheel*> m_sign_wheel_down;
	//m_sign_keyboard_input和keydown要分开，一次按键两个消息都产生
	sigslot::signal1<KMessageInput*> m_sign_keyboard_input;

	sigslot::signal1<KMessage3Dx*> m_sign_3dx;

	// 多点触摸
	sigslot::signal3<kn_int, kn_int,KMessageTouch*> m_sign_touchmove;
	sigslot::signal3<kn_int, kn_int,KMessageTouch*> m_sign_touchdown;
	sigslot::signal3<kn_int, kn_int,KMessageTouch*> m_sign_touchup;

	//Gesture
	sigslot::signal1<KGesture*> m_sign_gesture;


protected:
	//给view加一个id, 其大小可以用于标识在view group中的顺序
	kn_int m_id;

	//给view加一个name，可以做标识，用于调试和动态界面
	kn_string m_name;

	////////变形函数统一在绘制时处理矩阵，这些函数仅仅只是设置对应变换值，因为要在绘制时转为屏幕坐标
	int	m_transform_flag;
	REPoint m_translate_point;
	REPoint m_scale_size;
	REPoint m_scale_xy;
	REScalar m_rotate_angle;
	REPoint m_rotate_point;
	RERect m_skew_para;
	RERect m_rect_bound;

	//	父KViewGroup
	KViewGroup_WEAK_PTR m_p_parent;		
	// 所在的screen，顶级view才有值
	KScreen*	m_p_screen;

	KVIEWTYPE m_e_viewtype;

	kn_dword m_dw_touctpoint_count;		// 触摸点个数
	//鼠标是否进入view
	kn_bool	m_b_mouse_in_view;


	boost::shared_mutex m_rectInvalid_mutex;
	LSTRECT m_lst_rect_invalid;		//	需重绘的区域列表

	IRESurface* m_pSurface;

	kn_string   m_tip_txt;
	// 需要绘制的元素列表
	boost::shared_mutex m_lst_drawable_mutex; //drawable的锁，对drawable的操作必须使用读写锁
	VEC_DRAWABLE m_lst_drawable;

	kn_int m_i_modal_result;

	// 屏蔽消息，不允许消息穿透到下层 默认为true
	kn_bool m_shield_msg; 
	// 模式对话框
	kn_bool m_b_modal_exit;
	kn_bool		m_b_cache;
	kn_bool		m_b_update_cache;
	//是否处理消息
	kn_bool m_b_do_msg;
	//是否聚焦
	kn_bool m_b_focus;
	//是否激活(鼠标悬停)
	kn_bool m_b_active;

	// 被鼠标选中，不跟随动画
	kn_bool m_b_mouse_picked;
	kn_bool m_b_rmouse_picked;

	kn_bool m_b_show_tip;
	kn_int	m_tip_id; // tip的编号，标识tip 用于tip管理
	kn_int  m_tip_delay_time; //延迟显示tip时间
	kn_int  m_tip_hide_time; //显示后持续多少时间隐藏
	KTimer_PTR m_delay_timer;
	KTimer_PTR m_hide_timer;

	KGestureDetector* m_p_gesture_detector;

	// 允许识别手势，由于手势识别消耗额外资源，默认不开启
	kn_bool m_b_enable_gesture;
};

//使用智能指针管理view
typedef boost::shared_ptr<KView> KView_PTR;
typedef boost::weak_ptr<KView> KView_WEAK_PTR;
typedef vector<KView_PTR> LSTVIEW;

#define VIEW_PTR(T) boost::dynamic_pointer_cast<T>
//typedef vector<KView*>  LSTVIEW;

void MyTraceA(const char* szFormat, ... );


#endif
