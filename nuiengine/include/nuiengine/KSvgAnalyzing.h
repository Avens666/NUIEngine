//  **************************************
//  File:        KSvgAnalyzing.h
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
#ifndef _KSVGANALYZING_H__
#define _KSVGANALYZING_H__

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include "tinyxml.h"

#include "KShapeDrawable.h"
using namespace std;

struct ArcData
{
	RERect rect;
	float startAngle;
	float sweepAngle;
};

class NUI_API KSvgAnalyzing
{
public:
	KSvgAnalyzing();
	~KSvgAnalyzing(void);
	//load文件进行解析
	void LoadSvgFile(kn_string& filePath,vector<KShape* >& pShapeList, RERect& rect,bool bScaleFlag);
private:
	//解析矩形
	KShape* GetRectInfo(TiXmlElement* nodeElement);
	//解析圆形
	KShape* GetCircleInfo(TiXmlElement* nodeElement);
	//解析椭圆形
	KShape* GetEllipseInfo(TiXmlElement* nodeElement);
	//解析直线
	KShape* GetLineInfo(TiXmlElement* nodeElement);
	//解析折线
	KShape* GetPolylineInfo(TiXmlElement* nodeElement);
	//解析多边形
	KShape* GetPolygonInfo(TiXmlElement* nodeElement);
	//解析路径
	KShape* GetPathInfo(TiXmlElement* nodeElement);
	//将路径字符串分段
	void GetPathsData(char* pPoints, REPath& path);
	//将每段路径字符串解析为对应的数据
	void AnalyzingPath(vector<string>& sectionList,REPath& path);
	//将数据转换为skia的path对应数据
	void SvgToSkiaData(vector<PathData>& pathDataList, REPath& path);
	//将珀polygon对应坐标点字符串转为对应数据
	void GetPointsData(char* pPoints,vector<REPoint>& pointList);
	//根据svg Arc 参数计算出skia对应数据
	ArcData ComputeArc(float x0, float y0,float rx, float ry,float angle,bool largeArcFlag,bool sweepFlag,float x, float y);
	//设置填充颜色，填充透明度，边框等基类有属性
	void SetShapePublicAttr(KShape* pShape,char* pName, char* pAttr);
	float m_scale; //图片缩放比例
	bool m_bScale; //是否进行缩放
};
#endif //  _KSVGANALYZING_H__

