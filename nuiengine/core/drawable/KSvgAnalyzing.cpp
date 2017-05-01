//  **************************************
//  File:        KSvgAnalyzing.cpp
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
#include "KSvgAnalyzing.h"
#include <vector>
#include <math.h>
#include "CharSet.h"

 

KSvgAnalyzing::KSvgAnalyzing()
{
	m_scale = 1.0;
	m_bScale = false;
}


KSvgAnalyzing::~KSvgAnalyzing(void)
{
	
}
void KSvgAnalyzing::LoadSvgFile(kn_string& filePath,vector<KShape* >& pShapeList,RERect& rect,bool bScaleFlag)
{
	MyTraceA("~~~~~~~~~~~~~~~~~~~~~~~~~%s",filePath.c_str());
	m_bScale = bScaleFlag;
	RERect viewBox;
	char chSVGPath[128] ={0};
	WideCharToMultiByte(CP_ACP, 0, filePath.c_str(), -1, chSVGPath, MAX_PATH, NULL, NULL);
	TiXmlDocument* myDocument = new TiXmlDocument();
	myDocument->LoadFile(chSVGPath);
	vector<float> dataList;
	TiXmlElement* rootElement = myDocument->RootElement();  //svg 根节点
	MyTraceA("rootName = %s",rootElement->Value());
	TiXmlAttribute* attributeOfPath = rootElement->FirstAttribute();  
	while(attributeOfPath)
	{
		if(strcmp(attributeOfPath->Name(),"viewBox") == 0)
		{
			char* pSize = (char*) (attributeOfPath->Value());
			char* token;
			for(token = strtok(pSize, " "); token != NULL; token = strtok(NULL, " ")) 
			{
				float data = atof(token);
				dataList.push_back(data);
			}
			if (m_bScale)
			{
				viewBox.setXYWH(dataList.at(0),dataList.at(1),dataList.at(2),dataList.at(3));
				float bestScale = 1.0;
				if (viewBox.width()/rect.width() < viewBox.height() / rect.height())
				{
					bestScale = rect.height()  / viewBox.height();
				} 
				else
				{
					bestScale = rect.width() / viewBox.width();
				}
				m_scale = bestScale;
			}
			else
			{
				rect.setXYWH(dataList.at(0),dataList.at(1),dataList.at(2),dataList.at(3));
			}
			break;
		}
		attributeOfPath = attributeOfPath->Next();
	}
	
	TiXmlElement* nodeElement = rootElement->FirstChildElement(); //path、rect、...
	KShape * pShape(NULL);
	while ( nodeElement ) {
		pShape = NULL;
		char* pShapeType = (char*)nodeElement->Value();
		if(strcmp(pShapeType,"rect") == 0)
		{	
			pShape = GetRectInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"circle") == 0)
		{
			pShape = GetCircleInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"ellipse") == 0)
		{
			pShape = GetEllipseInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"line") == 0)
		{
			pShape = GetLineInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"polyline") == 0)
		{
			pShape = GetPolylineInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"polygon") == 0)
		{
			pShape = GetPolygonInfo(nodeElement);
		}
		else if (strcmp(pShapeType,"path") == 0)
		{
			pShape = GetPathInfo(nodeElement);
		}
		
		if (pShape)
		{
			pShapeList.push_back(pShape);
		}
		//取下一结点
		nodeElement = nodeElement->NextSiblingElement();
	}
	}
//解析矩形
KShape* KSvgAnalyzing::GetRectInfo(TiXmlElement* nodeElement)
{
	KRoundRectShape* pShape = (KRoundRectShape*)new KRoundRectShape ;
	pShape->setShapeType(SHAPE_RECT);
	float rect_x = 0;
	float rect_y = 0;
	float rect_w = 0;
	float rect_h = 0;
	float rect_rx = 0;
	float rect_ry = 0;

	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"x") == 0)
		{
			rect_x = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"y") == 0)
		{
			rect_y = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"rx") == 0)
		{
			rect_rx = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"ry") == 0)
		{
			rect_ry = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"width") == 0)
		{
			rect_w = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"height") == 0)
		{
			rect_h = atof(pAttr) * m_scale;
		}
		
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	RERect rect;
	rect.setXYWH(rect_x,rect_y,rect_w,rect_h);
	pShape->setRect(rect);
	pShape->SetRX(rect_rx);
	pShape->SetRY(rect_ry);
	return pShape;
}
KShape* KSvgAnalyzing::GetCircleInfo(TiXmlElement* nodeElement)
{
	KCircleShape* pShape =  (KCircleShape*)new KCircleShape ;
	pShape->setShapeType(SHAPE_CIRCLE);
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute(); 
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"cx") == 0)
		{
			float cx = atof(pAttr) * m_scale;
			pShape->SetCX(cx);
		}
		else if (strcmp(pName,"cy") == 0)
		{
			float cy = atof(pAttr) * m_scale;
			pShape->SetCY(cy);
		}
		else if (strcmp(pName,"r") == 0)
		{
			float cr = atof(pAttr) * m_scale;
			pShape->SetCR(cr);
		}
		
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	return pShape;
}
KShape* KSvgAnalyzing::GetEllipseInfo(TiXmlElement* nodeElement)
{
	KOvalShape* pShape = (KOvalShape*)new KOvalShape ;
	pShape->setShapeType(SHAPE_ELLIPSE);
	float cx = 0;
	float cy = 0;
	float rx = 0;
	float ry = 0;
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"cx") == 0)
		{
			cx = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"cy") == 0)
		{
			cy = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"rx") == 0)
		{
			rx = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"ry") == 0)
		{
			ry = atof(pAttr) * m_scale;
		}
		
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	RERect rect;
	rect.setXYWH(cx - rx,cy - ry,2*rx,2*ry);
	pShape->setRect(rect);
	return pShape;
}
KShape* KSvgAnalyzing::GetLineInfo(TiXmlElement* nodeElement)
{
	KLineShape* pShape = (KLineShape*)new KLineShape;
	pShape->setShapeType(SHAPE_LINE);
	REPoint p1;
	REPoint p2;
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"x1") == 0)
		{
			p1.fX = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"y1") == 0)
		{
			p1.fY = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"x2") == 0)
		{
			p2.fX = atof(pAttr) * m_scale;
		}
		else if (strcmp(pName,"y2") == 0)
		{
			p2.fY = atof(pAttr) * m_scale;
		}
		
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	pShape->SetStartPoint(p1);
	pShape->SetEndPoint(p2);
	return pShape;
}
KShape* KSvgAnalyzing::GetPolylineInfo(TiXmlElement* nodeElement)
{
	KPolygonShape* pShape = (KPolygonShape*)new KPolygonShape;
	pShape->setShapeType(SHAPE_POLYGON);
	vector<REPoint> pointList;
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"points") == 0)
		{
			GetPointsData(pAttr, pointList);
		}
	
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	pShape->SetCloseFlag(false);
	pShape->SetPoints(pointList);
	return pShape;
}
KShape* KSvgAnalyzing::GetPolygonInfo(TiXmlElement* nodeElement)
{
	KPolygonShape* pShape = (KPolygonShape*)new KPolygonShape ;
	pShape->setShapeType(SHAPE_POLYGON);
	vector<REPoint> pointList;
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"points") == 0)
		{
			GetPointsData(pAttr, pointList);
		}
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	pShape->SetCloseFlag(true);
	pShape->SetPoints(pointList);
	
	return pShape;
}
KShape* KSvgAnalyzing::GetPathInfo(TiXmlElement* nodeElement)
{
	KPathShape* pShape = (KPathShape*)new KPathShape ;
	pShape->setShapeType(SHAPE_PATH);
	TiXmlAttribute* attributeOfPath = nodeElement->FirstAttribute();  
	REPath* path = new REPath;

	while ( attributeOfPath ) {
		char* pName = (char*)(attributeOfPath->Name());
		char* pAttr = (char*)(attributeOfPath->Value());
		if (strcmp(pName,"d") == 0)
		{
			path->reset();
			GetPathsData(pAttr,*path);
			pShape->SetPath(*path);
		}
		
		SetShapePublicAttr(pShape, pName, pAttr);
		attributeOfPath = attributeOfPath->Next();
	}
	
	delete path;
	return pShape;
}

void KSvgAnalyzing::GetPathsData(char* pPoints, REPath& path)
{
	char delim[] = "MmLlHhVvCcSsQqTtAaZz"; //需查找的字符
	vector<string>sectionList;
	int len = strlen(pPoints);
	char tempBuff[128]={0};
	int recordIndex = 0; //记录遍历到字母的index
	for (int i=0;i <= len;i++) //遍历到最后的'\0'获取最后一个子串
	{
		if (*(pPoints+i) == '\0' || strrchr(delim,*(pPoints+i)))
		{
			if(i != recordIndex)
			{
				string pathInfo(tempBuff);
				sectionList.push_back(pathInfo);
			}
			//记录当前index，清空buf
			recordIndex = i;
			memset(tempBuff,0,sizeof(tempBuff));
			tempBuff[i-recordIndex] = *(pPoints+i);
		}
		else
		{
			tempBuff[i-recordIndex] = *(pPoints+i);
		}

	}
	//将path段解析成对应数据
	AnalyzingPath(sectionList,path);
	
}
void KSvgAnalyzing::AnalyzingPath(vector<string>& sectionList,REPath& path)
{
	vector<PathData> pathDataList;
	//将数据和字母（类型）分开
	for (int i = 0;i< sectionList.size();i++)
	{	
		char tempBuff[128] = {0};
		strcpy(tempBuff,(char*)sectionList.at(i).c_str());
		char* pChar = tempBuff + 1; //去掉首字母
		char type = tempBuff[0];   //获取类型
		PathData pathData;
		switch (type)
		{
		case 'M':
			pathData.pathType = PATH_ABS_MOVETO;
			break;
		case 'm':
			pathData.pathType = PATH_REL_MOVETO;
			break;
		case 'L':
			pathData.pathType = PATH_ABS_LINETO;
			break;
		case 'l':
			pathData.pathType = PATH_REL_LINETO;
			break;
		case 'H':
			pathData.pathType = PATH_ABS_H_LINETO;
			break;
		case 'h':
			pathData.pathType = PATH_REL_H_LINETO;
			break;
		case 'V':
			pathData.pathType = PATH_ABS_V_LINETO;
			break;
		case 'v':
			pathData.pathType = PATH_REL_V_LINETO;
			break;
		case 'C':
			pathData.pathType = PATH_ABS_CURVETO;
			break;
		case 'c':
			pathData.pathType = PATH_REL_CURVETO;
			break;
		case 'S':
			pathData.pathType = PATH_ABS_S_CURVETO;
			break;
		case 's':
			pathData.pathType = PATH_REL_S_CURVETO;
			break;
		case 'Q':
			pathData.pathType = PATH_ABS_Q_CURVE;
			break;
		case 'q':
			pathData.pathType = PATH_REL_Q_CURVE;
			break;
		case 'T':
			pathData.pathType = PATH_ABS_T_CURVETO;
			break;
		case 't':
			pathData.pathType = PATH_REL_T_CURVETO;
			break;
		case 'A':
			pathData.pathType = PATH_ABS_ARC;
			break;
		case 'a':
			pathData.pathType = PATH_REL_ARC;
			break;
		case 'Z':
			pathData.pathType = PATH_CLOSEPATH;
			break;
		case 'z':
			pathData.pathType = PATH_CLOSEPATH;
			break;
		default:
			break;
		}
		//转换为数据
		char delim[] = " ,-"; //需查找的字符
		int len = strlen(pChar);
		char dataBuff[128]={0};
		int recordIndex = 0; //记录遍历到字母的index
		for (int i=0;i <= len;i++) //遍历到最后的'\0'获取最后一个子串
		{	
			//如果扫描到指定字符或者结束缓存前面一段字符串
			if (*(pChar+i) == '\0' || strrchr(delim,*(pChar+i)))
			{
				//字符串第一个字符是“-”忽略
				float data;
				if(dataBuff[0] == ','&&strlen(dataBuff) > 1)  //保留",x"
				{
					char* pData = dataBuff+1;
					data = atof(pData);
					pathData.PathUnitDataList.push_back(data);
					
				}
				else 
				{
					if (strlen(dataBuff) > 1) 
					{
						data = atof(dataBuff);
						pathData.PathUnitDataList.push_back(data);
					}
					else if (dataBuff[0] > 47&&dataBuff[0] < 58)//数字字符ASCII 范围 48 ~ 57
					{
						data = atof(dataBuff);
						pathData.PathUnitDataList.push_back(data);
					}
					
				}				
				recordIndex = i;
				memset(dataBuff,0,sizeof(dataBuff));
				dataBuff[i-recordIndex] = *(pChar+i);	
				
			}
			else
			{
				dataBuff[i-recordIndex] = *(pChar+i);
			}

		}
		pathDataList.push_back(pathData);
	}
	SvgToSkiaData(pathDataList,path);
	
}
void KSvgAnalyzing::SvgToSkiaData(vector<PathData>& pathDataList, REPath& path)
{
	REPoint lastControl; //记录最后一个控制点
	bool b_arc = false;
	//转换为skiapath对应数据。
	for (int i=0;i<pathDataList.size();i++)
	{
		PATHTYPE type = pathDataList.at(i).pathType;
		switch(type)
		{
		case PATH_ABS_MOVETO:
			{
				REPoint point;
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				point.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				path.moveTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_REL_MOVETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);//获取之前路径最后一个点的坐标
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				point.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				path.moveTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_ABS_LINETO:
			{
				REPoint point;
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				point.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_REL_LINETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				point.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_ABS_H_LINETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				point.fY = lastPt.fY;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_REL_H_LINETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);
				point.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				point.fY = lastPt.fY;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_ABS_V_LINETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);
				point.fX = lastPt.fX;
				point.fY = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_REL_V_LINETO:
			{
				REPoint point;
				REPoint lastPt;
				path.getLastPt(&lastPt);
				point.fX = lastPt.fX;
				point.fY = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fY;
				path.lineTo(point);
				lastControl = point;//没有控制点时默认控制点就是当前点
			}
			break;
		case PATH_ABS_CURVETO:
			{
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				startControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				startControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				endControl.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale;
				endControl.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(4) * m_scale;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(5) * m_scale;
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_REL_CURVETO:
			{
				REPoint lastPt;
				path.getLastPt(&lastPt);
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				startControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				startControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				endControl.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale + lastPt.fX;
				endControl.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale + lastPt.fY;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(4) * m_scale + lastPt.fX;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(5) * m_scale + lastPt.fY;
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_ABS_S_CURVETO:
			{
				//根据最后一个控制点算出当前S路径起始控制点(lastpt点中心对称)
				REPoint lastPt;
				path.getLastPt(&lastPt);
				float tempX = lastPt.fX - lastControl.fX;
				float tempY = lastPt.fY - lastControl.fY;
				REPoint startControl;
				startControl.fX = lastPt.fX + tempX;
				startControl.fY = lastPt.fY + tempY;
				REPoint endControl;
				REPoint endPoint;
				endControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				endControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale;
				path.cubicTo(startControl,endControl,endPoint);
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_REL_S_CURVETO:
			{
				//根据最后一个控制点算出当前S路径起始控制点(lastpt点中心对称)
				REPoint lastPt;
				path.getLastPt(&lastPt);
				float tempX = lastPt.fX - lastControl.fX;
				float tempY = lastPt.fY - lastControl.fY;
				REPoint startControl;
				startControl.fX = lastPt.fX + tempX;
				startControl.fY = lastPt.fY + tempY;
				REPoint endControl;
				REPoint endPoint;
				endControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				endControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale + lastPt.fX;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale + lastPt.fY;
				path.cubicTo(startControl,endControl,endPoint);
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_ABS_Q_CURVE:
			{
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				startControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				startControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				endControl = startControl;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale;
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_REL_Q_CURVE:
			{
				REPoint lastPt;
				path.getLastPt(&lastPt);
				float tempX = lastPt.fX - lastControl.fX;
				float tempY = lastPt.fY - lastControl.fY;
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				startControl.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				startControl.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				endControl = startControl;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(2) * m_scale + lastPt.fX;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(3) * m_scale + lastPt.fY;
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_ABS_T_CURVETO:
			{
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				REPoint lastPt;
				bool ret = path.getLastPt(&lastPt);
				float tempX = lastPt.fX - lastControl.fX;
				float tempY = lastPt.fY - lastControl.fY;
				startControl.fX = lastPt.fX + tempX;
				startControl.fY = lastPt.fY + tempY;
				endControl = startControl;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				path.moveTo(lastPt);
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_REL_T_CURVETO:
			{
				REPoint startControl;//开始控制点
				REPoint endControl;//结束控制点
				REPoint endPoint;
				REPoint lastPt;
				bool ret = path.getLastPt(&lastPt);
				float tempX = lastPt.fX - lastControl.fX;
				float tempY = lastPt.fY - lastControl.fY;
				startControl.fX = lastPt.fX + tempX;
				startControl.fY = lastPt.fY + tempY;
				endControl = startControl;
				endPoint.fX = pathDataList.at(i).PathUnitDataList.at(0) * m_scale + lastPt.fX;
				endPoint.fY = pathDataList.at(i).PathUnitDataList.at(1) * m_scale + lastPt.fY;
				path.moveTo(lastPt);
				path.cubicTo(startControl,endControl,endPoint);	
				lastControl = endControl;//记录最后的一个控制点
			}
			break;
		case PATH_ABS_ARC:
			{
				REPoint lastPt;
				path.getLastPt(&lastPt);
				float x0 = lastPt.fX;
				float y0 = lastPt.fY;
				float rx = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				float ry = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				float angle = pathDataList.at(i).PathUnitDataList.at(0);
				float largeArcFlag = pathDataList.at(i).PathUnitDataList.at(0);
				float sweepFlag = pathDataList.at(i).PathUnitDataList.at(0);
				float x = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				float y = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				ArcData newArc = ComputeArc(x0, y0,rx, ry,angle,largeArcFlag,sweepFlag,x,y);
				RERect rect = newArc.rect;
				float startAngle = newArc.startAngle;
				float sweepAngle = newArc.sweepAngle;
				path.addArc(rect,startAngle,sweepAngle);
				lastControl.fX = x;
				lastControl.fX = y;
				b_arc = true;
			}
			break;
		case PATH_REL_ARC:
			{
				REPoint lastPt;
				path.getLastPt(&lastPt);
				MyTraceA("lastPt.fx =%f,lastPt.fy=%f\n",lastPt.fX,lastPt.fY);
				float x0 = lastPt.fX;
				float y0 = lastPt.fY;
				float rx = pathDataList.at(i).PathUnitDataList.at(0) * m_scale;
				float ry = pathDataList.at(i).PathUnitDataList.at(1) * m_scale;
				float angle = pathDataList.at(i).PathUnitDataList.at(2);
				float largeArcFlag = pathDataList.at(i).PathUnitDataList.at(3);
				float sweepFlag = pathDataList.at(i).PathUnitDataList.at(4);
				float x = pathDataList.at(i).PathUnitDataList.at(5) * m_scale + lastPt.fX;
				float y = pathDataList.at(i).PathUnitDataList.at(6) * m_scale + lastPt.fY;
				ArcData newArc = ComputeArc(x0, y0,rx, ry,angle,largeArcFlag,sweepFlag,x,y);
				RERect rect = newArc.rect;
				float startAngle = newArc.startAngle;
				float sweepAngle = newArc.sweepAngle;
				path.addArc(rect,startAngle,sweepAngle);
				path.getLastPt(&lastPt);
				lastControl.fX = x;
				lastControl.fX = y;
				b_arc = true;
			}
			break;
		case PATH_CLOSEPATH:
			{
				if (b_arc)
				{
					REPoint firstPoint = path.getPoint(0);
					path.lineTo(firstPoint);
				}
				path.close();
			}
			break;
		default:
			break;

		}	
	}	
};
//获取polygon、ployline 坐标点数据
void KSvgAnalyzing::GetPointsData(char* pPoints,vector<REPoint>& pointList)
{
	char pTemp[1024] ={0}; 
	strcpy(pTemp,pPoints);
	char *pTemp2 = NULL; 
	vector<char*> pCharList;
	pTemp2 = strtok(pTemp," ");
	while( pTemp2 != NULL)
	{
		pCharList.push_back(pTemp2);
		pTemp2 = strtok(NULL," ");		
	}
	for(int i= 0;i< pCharList.size();i++)
	{
		//std::cout<<*pCharList.at(i)<<std::endl;
		char pTemp[1024] ={0}; 
		strcpy(pTemp,pCharList.at(i));
		char *pTemp2 = NULL; 
		pTemp2 = strtok(pTemp,",");
		REPoint point;
		point.fX = atof(pTemp2) * m_scale;
		while( pTemp2 != NULL)
		{
			pTemp2 = strtok(NULL,",");
			if(pTemp2 != NULL)
			{
				point.fY = atof(pTemp2) * m_scale;
			}
		}
		pointList.push_back(point);
	}
}
void KSvgAnalyzing::SetShapePublicAttr(KShape* pShape,char* pName, char* pAttr)
{
	 if (strcmp(pName,"fill") == 0)
	{
		if (strcmp(pAttr,"none") == 0)
		{
			pShape->setFillOpacity(0.0);
		}
		else
		{
			char* pRGB = pAttr;
			char* pNewRGB = pRGB+1;
			char rRGBA[12] = "0xFF";
			strcat(rRGBA,pNewRGB);
			unsigned int fill = strtoul(rRGBA,NULL,16);
			pShape->setFillColor(fill);
		}
		
	}
	else if (strcmp(pName,"fill-opacity") == 0)
	{
		float fill_opacity = atof(pAttr);
		if (pShape->getFillOpacity() > fill_opacity)
		{
			pShape->setFillOpacity(fill_opacity);
		}	
	}
	else if (strcmp(pName,"stroke") == 0)
	{
		if (strcmp(pAttr,"none") == 0)
		{
			pShape->setStrokeOpacity(0.0);
		}
		{
			char* pRGB = pAttr;
			char* pNewRGB = pRGB+1;
			char rRGBA[12] = "0xFF";
			strcat(rRGBA,pNewRGB);
			unsigned int stroke = strtoul(rRGBA,NULL,16);
			pShape->setStrokeColor(stroke);
		}
		
	}
	else if (strcmp(pName,"stroke-width") == 0)
	{
		int stroke_width = atoi(pAttr) * m_scale;
		pShape->setStrokeWidth(stroke_width);
	}
	else if (strcmp(pName,"stroke-miterlimit") == 0)
	{
		int stroke_miterlimit = atoi(pAttr);
		pShape->setStrokeMiterlimit(stroke_miterlimit);
	}
	else if (strcmp(pName,"stroke-opacity") == 0)
	{
		float stroke_opacity = atof(pAttr);
		if (pShape->getStrokeOpacity() > stroke_opacity)
		{
			pShape->setStrokeOpacity(stroke_opacity);
		}	
	}
	else if (strcmp(pName,"opacity") == 0)
	{
		float opacity = atof(pAttr);
		pShape->setOpacity(opacity);

	}
}
ArcData  KSvgAnalyzing::ComputeArc(float startX, float startY,float rx, float ry,float angle,bool largeArcFlag,bool sweepFlag,float endX, float endY)
{
        // 基于svg数据实现椭圆弧

        // 计算起点和终点之间一半的位置坐标
        float dx2 = (startX - endX) / 2.0;
        float dy2 = (startY - endY) / 2.0;
        //将角度转换为弧度 弧度 = 角度*PI/180
        angle = ((int)angle) % 360 *(PI / 180);
        float cosAngle = cos(angle);
        float sinAngle = sin(angle);

        //
        // Step 1 : Compute (x1, y1)
        //
        float x1 = (cosAngle * dx2 + sinAngle * dy2);
        float y1 = (-sinAngle * dx2 + cosAngle * dy2);
        // 确保半径足够大
        rx = fabs(rx);//取绝对值
        ry = fabs(ry);
        float Prx = rx * rx;
        float Pry = ry * ry;
        float Px1 = x1 * x1;
        float Py1 = y1 * y1;
        // 检查半径是否足够大
        float radiiCheck = Px1/Prx + Py1/Pry;
        if (radiiCheck > 1) {
            rx = sqrt(radiiCheck) * rx;
            ry = sqrt(radiiCheck) * ry;
            Prx = rx * rx;
            Pry = ry * ry;
        }

        //
        // Step 2 : Compute (cx1, cy1)
        //
        float sign = (largeArcFlag == sweepFlag) ? -1 : 1;
        float sq = ((Prx*Pry)-(Prx*Py1)-(Pry*Px1)) / ((Prx*Py1)+(Pry*Px1));
        sq = (sq < 0) ? 0 : sq;
        float coef = (sign * sqrt(sq));
        float cx1 = coef * ((rx * y1) / ry);
        float cy1 = coef * -((ry * x1) / rx);

        //
        // Step 3 : Compute (cx, cy) from (cx1, cy1)
        //
        float sx2 = (startX + endX) / 2.0;
        float sy2 = (startY + endY) / 2.0;
        float cx = sx2 + (cosAngle * cx1 - sinAngle * cy1);
        float cy = sy2 + (sinAngle * cx1 + cosAngle * cy1);

        //
        // Step 4 : Compute the angleStart (angle1) and the angleExtent (dangle)
        //
        float ux = (x1 - cx1) / rx;
        float uy = (y1 - cy1) / ry;
        float vx = (-x1 - cx1) / rx;
        float vy = (-y1 - cy1) / ry;
        float p, n;
        // 计算开始角度
        n = sqrt((ux * ux) + (uy * uy));
        p = ux; // (1 * ux) + (0 * uy)
        sign = (uy < 0) ? -1.0 : 1.0;
		//弧度转角度
        float angleStart = (sign * acos(p / n))*(180/PI);

        // 计算结束角度
        n = sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
        p = ux * vx + uy * vy;
        sign = (ux * vy - uy * vx < 0) ? -1.0 : 1.0;
        float angleExtent = (sign * acos(p / n)) *(180/PI);
        if(!sweepFlag && angleExtent > 0) {
            angleExtent -= 360.0;
        } else if (sweepFlag && angleExtent < 0) {
            angleExtent += 360.0;
        }
        angleExtent = (int)angleExtent % 360;
        angleStart = (int)angleStart % 360;

        ArcData newArc;
		float rect_left = cx - rx;
		float rect_top = cy - ry;
		float rect_width = rx * 2.0;
		float rect_height = ry * 2.0;
		newArc.rect.setXYWH(rect_left,rect_top,rect_width,rect_height);
		newArc.startAngle = angleStart;
		newArc.sweepAngle = angleExtent;
		return newArc;
    }
