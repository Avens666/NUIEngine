//  **************************************
//  File:        KStringManager.cpp
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
#include "KStringManager.h"
#include "renderingengine.h"
#include <fstream>
#include <string>
#include "KConfigManager.h"
#include "FilePath.h"
using namespace std;


KStringManager * g_string_m = NULL;
KStringManager* getStringManager()
{
	if (g_string_m == NULL)
	{
		kn_string strPath = GetCurrentPath();
		switch (getConfigManager()->getLanguage() )
		{
		case LAN_CN:
			strPath += _T("config/cn.txt");
			break;
		case LAN_EN:
			strPath  += _T("config/en.txt");
			break;
		case LAN_JP:
			strPath  += _T("config/jp.txt");
			break;
		default:
			strPath  += _T("config/en.txt");
			break;
		}
		g_string_m = new KStringManager(strPath);
	}
	return g_string_m;
}

void releaseStringManager()
{
	SAFE_DELETE(g_string_m);
}

KStringManager::KStringManager(kn_string strFilePath)
{
	LoadStringFromFile(strFilePath);
}


KStringManager::~KStringManager()
{
	m_StringManagerMap.clear();
}

kn_bool KStringManager::LoadStringFromFile (kn_string strFilePath)
{

	if (strFilePath.empty() || strFilePath == _T(""))
	{
		return false;
	}
	else
	{

		string file = ns2s( strFilePath );
		string str;
		kn_string strKey,strValue;
		TiXmlDocument xmldoc;
		if (xmldoc.LoadFile(file.c_str()))
		{
			TiXmlElement* pRootElement = xmldoc.RootElement();
			if ( pRootElement )
			{
				TiXmlElement*  childElement = pRootElement->FirstChildElement("String");

				while (childElement)
				{
					strKey = GetAttributeString(  childElement, ( "name") );;
					strValue = GetAttributeString(  childElement, ( "value") );
					m_StringManagerMap[boost::trim_copy(strKey)] = boost::trim_copy(strValue);

					childElement = childElement->NextSiblingElement();
				}
			}
			else
			{
				return false;
			}
		}

		


	//	ifstream infile( strFilePath.c_str());
	//	kn_string strRead,strKey,strValue;

	//	if (!infile)
	//	{
	//		return false;
	//	}
	//	char c_read[200];
	//	kn_string s = _T("新");
	//	//while(getline(infile,strRead))
	//	while(infile.getline(c_read,200))
	//	{
	//		strRead = (wchar_t*)(c_read);
	//		if (strRead.find('=') != -1)
	//		{
	//			//**取等号左边并去掉两端空格
	//			strKey = strRead.substr(0,strRead.find(_T('=') ));
	//			int Fkeypos = strKey.find_first_not_of(' ');
	//			strKey = strKey.substr(Fkeypos,strKey.size() - Fkeypos);
	//			int Bkeypos = strKey.find_first_of(' ');
	//			strKey = strKey.substr(0,Bkeypos);

	//			//取等号右边并去掉两端空格，右侧有等号则保留等号左边
	//			strValue = strRead.substr(strRead.find('=') + 1,strRead.size());

	////			strValue = strValue.substr(0,strValue.find('='));
	//			int FValuepos = strValue.find_first_not_of(' ');
	//			strValue = strValue.substr(FValuepos,strValue.size() - FValuepos);
	//			int BValuepos = strValue.find_last_not_of(' ');
	//			strValue = strValue.substr(0,BValuepos+1);
	//			

	//			//kn_string nstrKey = s2ns(strKey);
	//			//kn_string nstrValue = s2ns(strValue);

	//			
	//			m_StringManagerMap[strKey] = strValue;
	//		}
	//	}

	//	infile.close();
	//	infile.clear();

	}
	return true;

}

kn_string KStringManager::GetStringByID(kn_string nStringKey)
{
	return m_StringManagerMap[nStringKey];
}

kn_bool KStringManager::AddString(kn_string sKey,kn_string sValue)
{
	mapStringMap::iterator iterMap = m_StringManagerMap.find(sKey);
	if (iterMap != m_StringManagerMap.end())
	{
		return false;
	}
	else
		m_StringManagerMap[sKey] = sValue;
	return true;
}
