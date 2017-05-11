//  **************************************
//  File:        DataSync.h
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

#ifndef __DATA_SYNC_H
#define __DATA_SYNC_H
#include "KObject.h"
#include "renderingengine.h"
#include "KEasingCurve.h"
#include <boost/thread/thread.hpp>
#include "KView.h"

class NUI_API CPropertyBase: public KObject 
{
public:
	CPropertyBase(void);
	~CPropertyBase(void);
	enum PropertyTYPE
	{
		ATTR_NONE,
		ATTR_POS,
		ATTR_OPACITY,
		ATTR_BOUNDRECT,
		ATTR_ENUM_SIZE
	};
	virtual void DoChange()=0;
	CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, int iEasingCurveType);
	//根据目标值返回一个推算状态，用于动画，放在PropertyBase中主要考虑的是底层封装，上层方便统一接口调用
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve)=0;
	virtual CPropertyBase* copyProperty(KView_PTR p)=0;
	//从自己的view copy
	virtual CPropertyBase* copyProperty()=0;
	KView_PTR getView();
	void setView(KView_PTR p);
	//view的属性是否同当前值不同，相同可以忽略
	virtual kn_bool isChange() ;
private:
	KView_PTR m_p_view;

};

typedef  vector<CPropertyBase*> PropertyBaseVector;

// View位置移动
class NUI_API CPropertyPos: public CPropertyBase
{
public:
	CPropertyPos(KView_PTR p, kn_int x, kn_int y);
	~CPropertyPos(void);

	void getProperty( kn_int &x, kn_int &y);
	void setProperty(KView_PTR p, kn_int x, kn_int y);
	void DoChange();
	virtual kn_bool isChange();
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
private:
	REPoint m_point;
};

// View透明度
class NUI_API CPropertyOpacity: public CPropertyBase
{
public:
	CPropertyOpacity(KView_PTR p, kn_int opacity);
	~CPropertyOpacity(void);

	void getProperty(kn_int &opacity);
	void setProperty(KView_PTR p, kn_int opacity);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();
private:
	int m_i_opacity;
};

///////////////////////////////////////////////////////////////////////////////////
// 通用型动画参数类，使用通用性动画参数控制动画，应用层view如要对一个自定义参数进行动画，
// view只用重载doChange函数进行对应的处理，这避免了每对一个新参数都要新增一个属性类的繁琐编码
class NUI_API CPropertyPara: public CPropertyBase
{
public:
	CPropertyPara(KView_PTR p, kn_int para_id, double v);
	~CPropertyPara(void);

	void getProperty(kn_double &opacity);
	void setProperty(KView_PTR p, kn_double opacity);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();
private:
	//动画值，使用double兼容更多类型
	double m_value;

	//参数id，view根据这个id判断是那个参数需要设置
	kn_int m_para_id;
};

// View缩放
class NUI_API CPropertyRect: public CPropertyBase
{
public:
	CPropertyRect(KView_PTR p, RERect rect);
	~CPropertyRect(void);

	void getProperty(RERect& rect);
	void setProperty(KView_PTR p, RERect rect);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();
private:
	RERect m_rect;
};

// View旋转
class NUI_API CPropertyAngle: public CPropertyBase
{
public:
	CPropertyAngle(KView_PTR p, REScalar degree, REScalar px, REScalar py);
	~CPropertyAngle(void);
	void getProperty(REScalar &a, REScalar &x, REScalar &y);
	void setProperty(KView_PTR p, REScalar degree, REScalar px, REScalar py);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();
private:
	REScalar m_degree;
	REScalar m_px;
	REScalar m_py;
};

// View裁剪区
class NUI_API CPropertyClip: public CPropertyBase
{
public:
	CPropertyClip(KView_PTR p, RERect rect);
	~CPropertyClip(void);

	void getProperty(RERect& rect);
	void setProperty(KView_PTR p, RERect rect);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();
private:
	RERect m_rect;
};


// StaticView文字移动
class NUI_API CPropertyText: public CPropertyBase
{
public:
	CPropertyText(KView_PTR p, RERect rectBound, RERect rectClip);
	~CPropertyText(void);

	void getProperty(RERect& rectBound, RERect& rectClip);
	void setProperty(KView_PTR p, RERect rectBound, RERect rectClip);
	virtual CPropertyBase* getCurrent(CPropertyBase* target, kn_double& dv, KEasingCurve* curve);
	virtual CPropertyBase* copyProperty(KView_PTR p);
	//从自己的view copy
	virtual CPropertyBase* copyProperty();
	virtual kn_bool isChange();
	void DoChange();

private:

	// 相对于上级View的坐标
	RERect m_rect;

	// 相对于View的坐标，不是屏幕坐标, draw的时候需转换成屏幕坐标
	RERect m_rect_clip;
};


class NUI_API CPropertyList: public KObject
{
	PropertyBaseVector m_lst_data;
public:
	CPropertyList(){};
	~CPropertyList();
	void addProperty(CPropertyBase* p);
	void DoChange();
};

class NUI_API CDataSync: public KObject
{
public:
	CDataSync(void);
	~CDataSync(void);

	CPropertyList* getDataList();
	//获得一个新id
	kn_int getNewID();
	//检查ID是否存在
	kn_bool checkID(kn_int&);
	//加入ID标识
	void addID(kn_int&);

	void addProperty(CPropertyBase* p);
	void release();

private:

	//动画属性列表
	CPropertyList* m_p_lst_data;

	//属性动画ID列表
	boost::shared_mutex m_id_mutex;
	vector<kn_int>	m_id_lst;

	//属性动画ID分配标识，递增分配
	//动画ID机制解释: 动画线程提交一帧动画会提交若干属性队列，一个动画线程使用一个id标识，当该属性队列被渲染线程
	//拿走时，id被清空。在动画线程提交帧动画申请时，检查自己的id是否存在，如果存在，代表自己上次的渲染请求还没完成，
	//将等待下次提交申请。该机制避免渲染线程忙不赢时，动画线程放入大量属性修改请求。也有效管理多个动画线程的渲染请求
	kn_int	m_current_id;
	boost::mutex m_data_mutex;
};


#endif // __DATA_SYNC_H