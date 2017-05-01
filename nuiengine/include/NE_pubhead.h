//  **************************************
//  File:        NE_pubhead.h
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
/**
* 文件名称：pubhead.h
* 摘    要：统一共公头文件，所有头文件均需包含该头文件
    pubhead.h文件为所有头文件均需包含的共公头文件，它进一步包括：
    1. configure.h 编译预处理相关配置
    2. type.h 基础数据类型定义
    3. macro.h 通用宏定义
    4. error.h 错误码定义
    5. debug.h 调试用的相关头文件声明，如assert宏定义，日志宏等
**/

#ifndef _NE_PUBHEAD_H_
#define _NE_PUBHEAD_H_

#include <algorithm>
#include <limits>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#ifdef ANDROID_NDK
#include <android/log.h>
#include <assert.h>
#include <unistd.h>
#include <sys/resource.h>
extern bool g_bLogEnable;	//	是否允许log
#endif
using namespace std;
#include "NE_type.h"
#include "NE_configure.h"
#include "NE_macro.h"
//#include "NE_error.h"
#include "NE_debug.h"

#if defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)
#include "KLog.h"
#include <stdio.h>
#include <limits.h>
#include <qglobal.h>
#endif

#ifndef ANDROID_NDK
#else
#define ASSERT(expr) \
	if (g_bLogEnable && !(expr)) __android_log_print(ANDROID_LOG_WARN, "ASSERT", "[#%d][t%d][%s] [%s]", \
	__LINE__, gettid(), __FUNCTION__, #expr);

#define _ASSERT(X) ASSERT(X)
#endif



//#define  LOG_TAG    "NE"
#ifdef ANDROID_NDK
#define  LOGI(...)  if(g_bLogEnable) __android_log_print(ANDROID_LOG_INFO  , "INFO",__VA_ARGS__)
#define  LOGE(...)  if(g_bLogEnable) __android_log_print(ANDROID_LOG_ERROR  , "ERROR",__VA_ARGS__)
#define  LOGD(...)  if(g_bLogEnable) __android_log_print(ANDROID_LOG_DEBUG  , "DEBUG",__VA_ARGS__)
#elif defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)
#define  LOGI LogInfo
#define  LOGE LogInfo
#define  LOGD LogInfo
#else
#define  LOGI(...)
#define  LOGE(...)
#define  LOGD(...)
#endif //ANDROID_NDK

// export uicomposer dll
//#define  UIC_DLL

#endif // _NE_PUBHEAD_H_
