//  **************************************
//  File:        KConfigManager.h
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
#ifndef K_CONFIG_MANAGER_H
#define K_CONFIG_MANAGER_H

#include "NE_type.h"
#include "boost/unordered_map.hpp"
#ifdef WIN32
#include <comdef.h>
#endif
#include <vector>
#include <string>
#include <fstream>
#include <string>
#include "KProfile.h"
#include <boost/algorithm/string.hpp>
#include "utility.h"
#include "tinyxml.h"

class NUI_API KConfigManager
{
private:

		TiXmlDocument *m_pXmldoc;
		
		// config的节点
		TiXmlElement* m_oneLayerElement;	//只有一级内容
		TiXmlElement* m_twoLayerElement;	//有两级的内容(目前给历史记录用)
		/**
		* 加载
		*/
		kn_bool loadConfigFromFile(kn_string strFilePath);

		string m_file;
		int m_language;
		//程序的全路径
		kn_string m_app_path;
public:
		KConfigManager(kn_string strFilePath);

		~KConfigManager();

		/**
		* 保存
		*/
		void saveConfig();
		kn_string getAppPath();
		/**
		* 设置
		*/
		kn_bool setConfig(const kn_string strValue,const kn_string str);
		kn_bool setConfig(const kn_string strValue,const kn_int v);

		/**
		* 查询
		*/
		kn_string getConfig(const kn_string str);
		kn_int getIntConfig(const kn_string str);
		kn_int getLanguage();
		kn_string getLanguageImagePath();

		/**
		* 获取一组配置(给历史记录用)
		*/
		void GetVectorConfig(const kn_string& str, vector<kn_string>& vHistory );
		
		/**
		* 添加历史记录
		*/
		kn_bool addHistoryConfig(const kn_string& str, const kn_string& strHistory );
};

 KConfigManager* getConfigManager();
 void releaseConfigManager();

#endif
