//  **************************************
//  File:        KConfigManager.cpp
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
#include "KConfigManager.h"
#include "renderingengine.h"
#include "boost/lexical_cast.hpp"
#include "FilePath.h"
#include "KStringManager.h"

#define MAX_LINE_SIZE	(512)

using namespace std;
using namespace boost;


KConfigManager * g_config_m = NULL;
KConfigManager* getConfigManager()
{
	if (g_config_m == NULL)
	{
		kn_string strPath = GetCurrentPath();
		strPath += _T("config/nplan_config.xml");

		g_config_m = new KConfigManager(strPath);
	}
	return g_config_m;
}

void releaseConfigManager()
{

	if (g_config_m)
	{
		g_config_m->saveConfig();
	}
	SAFE_DELETE(g_config_m);
}

KConfigManager::KConfigManager(kn_string strFilePath)
{
	m_oneLayerElement = NULL;
	m_pXmldoc = NULL;
	m_twoLayerElement = NULL;
	loadConfigFromFile(strFilePath);

#ifdef WIN32
	TCHAR szPath[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, szPath);
	m_app_path = szPath;
	m_app_path+=_T("/");
#endif
}


KConfigManager::~KConfigManager()
{
	if (m_pXmldoc)
	{
		delete m_pXmldoc;
	}
}

kn_string KConfigManager::getAppPath()
{
	return m_app_path;
}

kn_bool KConfigManager::loadConfigFromFile (kn_string strFilePath)
{
	//读取XML
	m_file = ns2s( strFilePath );
	m_pXmldoc = new TiXmlDocument(m_file.c_str());
	if (m_pXmldoc->LoadFile())
	{
		TiXmlElement* pRootElement = m_pXmldoc->RootElement();
		if ( pRootElement )
		{
			// 先加载一级的
			m_oneLayerElement = pRootElement->FirstChildElement("onelayer");
			// 再加载两级的 (现在就是给历史记录用)
			m_twoLayerElement = pRootElement->FirstChildElement("twolayer");
		}
		else
		{
			return false;
		}
	}
	else
	{
		//如果文件存在,应该就是XML格式错误.一样删掉重新创建
		m_pXmldoc->Clear();
        knDeleteFile(strFilePath.c_str());

		//如果没有就创建一个
		TiXmlElement* pRootElement = new TiXmlElement("NUIConfig");
		m_oneLayerElement = new TiXmlElement("onelayer");
		m_twoLayerElement = new TiXmlElement("twolayer");
		pRootElement->LinkEndChild(m_oneLayerElement);
		pRootElement->LinkEndChild(m_twoLayerElement);
		TiXmlElement* pHistorys = new TiXmlElement("historys");
		m_twoLayerElement->LinkEndChild(pHistorys);
		m_pXmldoc->LinkEndChild(pRootElement);

		//多语言
		TiXmlElement* plang = new TiXmlElement("language");
		TiXmlText* text = new TiXmlText("0");
		plang->LinkEndChild(text);
		m_oneLayerElement->LinkEndChild(plang);
		return false;
	}

	m_language = getIntConfig(_T("language"));
	return true;
}

kn_int KConfigManager::getLanguage()
{
	return m_language;
}

kn_string KConfigManager::getLanguageImagePath()
{
	kn_string strPath = _T("");
	switch (getConfigManager()->getLanguage() )
	{
	case LAN_CN:
		strPath += _T("cn_image/");
		break;
	case LAN_EN:
		strPath  += _T("en_image/");
		break;
	case LAN_JP:
		strPath  += _T("jp_iamge/");
		break;
	default:
		strPath  += _T("cn_image/");
		break;
	}
	return strPath;
}

//保存config
void KConfigManager::saveConfig()
{
	if (m_pXmldoc)
	{
#ifdef WIN32
		SetFileAttributes(s2ns(m_file).c_str(),0);
#endif
		m_pXmldoc->SaveFile(m_file.c_str());
	}
}

/**
* 根据键和值查找ini里面的内容
*/
kn_string KConfigManager::getConfig(const kn_string str)
{
	kn_string strResult = _T("");

	if (m_oneLayerElement)
	{
		TiXmlElement* childElement = m_oneLayerElement->FirstChildElement(ns2s(boost::to_lower_copy(str)).c_str());
		
		if (childElement)
		{
			strResult = GetTextString(childElement);
		}
		else
		{
			kn_string str_v = _T("0");
			setConfig(str, strResult);
		}
	}
	return strResult;
}
/**
* 根据键和值查找ini里面的内容
*/
kn_int KConfigManager::getIntConfig(const kn_string str)
{
	kn_int iResult = 0;
	

	if (m_oneLayerElement)
	{
		TiXmlElement* childElement = m_oneLayerElement->FirstChildElement(ns2s(boost::to_lower_copy(str)).c_str());

		if (childElement)
		{
			iResult = atoi(childElement->GetText());
		}
		else
		{
			kn_string str_v = _T("0");
			setConfig(str, str_v);
		}
	}
	return iResult;
}

kn_bool KConfigManager::setConfig(const kn_string strValue,const kn_int v)
{
	return setConfig(strValue, lexical_cast<kn_string>(v ) );
}

/**
* 根据键改ini里面的内容
*/
kn_bool KConfigManager::setConfig(const kn_string strValue,const kn_string str)
{
	if (m_oneLayerElement)
	{
		TiXmlElement* childElement = m_oneLayerElement->FirstChildElement(ns2s(boost::to_lower_copy(strValue)).c_str());

		if (childElement)
		{
			TiXmlText* text = new TiXmlText(ns2s(boost::to_lower_copy(str)).c_str());
			childElement->Clear();
			childElement->LinkEndChild(text);
		}
		else
		{
			childElement = new TiXmlElement(ns2s(boost::to_lower_copy(strValue)).c_str());
			TiXmlText* text = new TiXmlText(ns2s(boost::to_lower_copy(str)).c_str());
			childElement->Clear();
			childElement->LinkEndChild(text);

			m_oneLayerElement->LinkEndChild(childElement);

		}
	}
 	return true;
}

 void KConfigManager::GetVectorConfig(const kn_string& str, vector<kn_string>& vHistory )
 {
	 if (m_twoLayerElement)
	 {
		 TiXmlElement* childElement = m_twoLayerElement->FirstChildElement(ns2s(boost::to_lower_copy(str)).c_str());
		 if (childElement)
		 {

			 childElement = childElement->FirstChildElement("history");
			 while (childElement)
			 {
				 vHistory.push_back(GetTextString(childElement));
				 childElement = childElement->NextSiblingElement();
			 }
		 }
	 }
 }

 kn_bool KConfigManager::addHistoryConfig( const kn_string& str, const kn_string& strHistory )
 {
	 if (m_twoLayerElement)
	 {
		 TiXmlElement* childElement = m_twoLayerElement->FirstChildElement(ns2s(boost::to_lower_copy(str)).c_str());

		 if (childElement)
		 {
			 TiXmlElement* reElement = new TiXmlElement ( "history" );

#ifdef _UNICODE 
			 TiXmlText *text = new TiXmlText( UnicodeToUTF8(strHistory).c_str() );
#else
			 TiXmlText *text = new TiXmlText( strHistory.c_str() );
#endif
			 reElement->Clear();
			 reElement->LinkEndChild(text);

			 childElement->LinkEndChild( reElement );

		 }
	 }
	 return true;
 }
