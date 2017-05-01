//  **************************************
//  File:        message.h
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
#ifndef K_MESSAGE_H
#define K_MESSAGE_H

#include <boost/thread/thread.hpp>
#include "KObject.h"
void MyTrace(LPCTSTR lpsz, ...);
//消息大类别
enum KMSGTYPE
{
	KMSG_TYPE_NONE, //无效类别,基类使用
	KMSG_TYPE_KEY, //键盘类消息
	KMSG_TYPE_POS, //带有位置信息的消息，如鼠标，触摸屏等有位置坐标的消息，大类别
	KMSG_TYPE_MOUSE, //鼠标消息
	KMSG_TYPE_MOUSE_WHELL, //鼠标消息
	KMSG_TYPE_TOUCH, //触摸消息
	KMSG_TYPE_3DX,	//	3Dx鼠标
	KMSG_TYPE_USER,	//用户消息
	KMSG_TYPE_INPUT,  //输入法，
	KMSG_TYPE_DRAG  //输入法，
	
};

enum KMESSAGETYPE
{
	KMSG_BEGIN,

	KMSG_KEYDOWN,
	KMSG_KEYUP,
	
	KMSG_MOUSE_START,
	KMSG_LBBUTTONDOWN,
	KMSG_LBBUTTONUP,
	KMSG_RBBUTTONDOWN,
	KMSG_RBBUTTONUP,
	KMSG_MBBUTTONDOWN,
	KMSG_MBBUTTONUP,
	KMSG_MOUSEMOVE,
	KMSG_LBUTTONDBLCLK,
	KMSG_MBUTTONDBLCLK,
	KMSG_RBUTTONDBLCLK,
	KMSG_MOUSEWHEEL,

	KMSG_MOUSE_END,
	
	KMSG_TIMER,

	KMSG_TOUCH_UP,
	KMSG_TOUCH_MOVE,
	KMSG_TOUCH_DOWN,

	KMSG_3DX,

	KMSG_SCREEN_DRAW,

	KMSG_TYPE_INPUT_ING,  // 输入中
	KMSG_TYPE_INPUT_END,  // 输入结束
	KMSG_TYPE_CHAR,

	KMSG_TYPE_IME_UPDATE, // 输入法Edit控件更新
	KMSG_DRAG, //拖动移动消息
	KMSG_DRAG_UP, //拖动up消息

	KMSG_END,
	KMSG_USER = 10000

};

enum KNUI_MESSAGETYPE
{//这里的消息是传递给windows层用于分发的自定义消息
	//KNUIMSG_DRAW = 20000,
	//KNUIMSG_DRAG = 20001, //拖动移动消息
	//KNUIMSG_DRAG_UP = 20002, //拖动up消息
	KNUIMSG_USER=30000
};

#define KMSG_KEYFLAG_CONTROL 8
#define KMSG_KEYFLAG_SHIFT	 4
#define KMSG_KEYFLAG_ALT	 2

enum KMESSAGE_RETURN_TYPE
{
	
	KMSG_RETURN_DEFAULT, // 缺省处置
	KMSG_RETURN_DILE,  //消息已被处理
	KMSG_RETURN_CONTINUE, //消息强制继续处理
};
class KView;
typedef boost::shared_ptr<KView> KView_PTR;

class NUI_API KMessage : public KObject
{
public:
	KMessage();
	virtual ~KMessage(){};

	KMSGTYPE m_msg_class_type; //大类别
	KMESSAGETYPE m_msg_type; //小类别
	kn_uint m_wParam;
	kn_uint m_lParam;
	KView_PTR  m_p_view;
	KMESSAGE_RETURN_TYPE m_b_idle;
	static KMessage* getMsgForWin32( HWND hWnd,UINT message, WPARAM wParam, LPARAM lParam);
	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam){};
	//是否带轨迹
	kn_bool HasPos();
	void setIdle(KMESSAGE_RETURN_TYPE idle){m_b_idle = idle;};
	KMESSAGE_RETURN_TYPE getIdle(){return m_b_idle;};
};

class NUI_API KMessageKey : public KMessage
{
public:
	KMessageKey();
	virtual ~KMessageKey(){};

	kn_int m_key_id;
	kn_int m_key_flag;//辅助键状态
	kn_int m_repeat_count;//长按按键的重复次数
	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
};

class NUI_API KMessagePos : public KMessage
{//作为有轨迹的消息的基类，
public:
	KMessagePos();
	virtual ~KMessagePos(){};

	kn_int m_pos_x; 
	kn_int m_pos_y;
};

class NUI_API KMessageMouseWheel : public KMessagePos
{
public:
	KMessageMouseWheel();
	virtual ~KMessageMouseWheel(){};
	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	kn_int m_roll;
	kn_int m_key_flag;//辅助键状态
};


class NUI_API KMessageMouse : public KMessagePos
{
public:
	KMessageMouse();
	virtual ~KMessageMouse(){};

	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
};

class NUI_API KMessageTouch : public KMessagePos
{
public:
	KMessageTouch(HWND hwnd);
	virtual ~KMessageTouch();

	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	KMessageTouch* deepCopy();

public:

	kn_int m_touch_id;

	HWND m_wnd;


	int m_iPointCount;	//	触摸点个数
	int* m_ids;	// 标志point id
	int* m_xs;	// 多点坐标
	int* m_ys;
 

 
};

class NUI_API KMessage3Dx : public KMessage
{
public:
	virtual ~KMessage3Dx(){};

	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	UINT m_message;
};


class NUI_API KMessageInput : public KMessageKey
{
public:
	KMessageInput();
	virtual ~KMessageInput(){};
	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	kn_string m_InputStr;
};

// 用于接收输入法结果的EditText: onTextChange
//TextWatcher::onTextChanged(CharSequence s, int start, int before, int count)
class NUI_API KMessageEditTextIME : public KMessageInput
{
public:
	KMessageEditTextIME();
	virtual ~KMessageEditTextIME(){};
	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	//kn_string m_InputStr;


	 // 添加文字的位置(从0开始)
	kn_int m_i_start;

	// 文字改变的位置
	kn_int m_i_before;

	// 添加的文字总数
	kn_int m_i_count;

};
class NUI_API KMessageDrag : public KMessagePos
{
public:
	KMessageDrag();
	virtual ~KMessageDrag(){};
//	virtual void transMsgForWin32(UINT message, WPARAM wParam, LPARAM lParam);
	KView_PTR   m_p_drag_view;
	kn_int m_drag_type;
	
};

// createwindow 后调用
bool CheckTouchSupport(HWND wnd);



#endif
