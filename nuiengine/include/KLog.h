//  **************************************
//  File:        KLog.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2010/04/01
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************

#ifndef _KLOG_H_
#define _KLOG_H_
#include "Platform.h"
void BeginLog();

void EndLog();

void WriteLogNoTime(const char *szFormat, ...);

void WriteLog(const char *szFormat, ...);

void LogAssert(char* szFileName, long iLine, char* szFuncName);

void MyTrace(LPCTSTR lpsz, ...);
//  auto log
class NUI_API AutoLog
{
private:

    char m_szFuncName[256];
public:

    AutoLog(const char* szFuncName);


     ~AutoLog();

};

#if defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)
void LogInfo(const char *szFormat, ...);

void LogInfo(const wchar_t *szFormat, ...);
#endif

#define LOG WriteLog
#define LOG_ASSERT LogAssert
#define LOGNT WriteLogNoTime
#define LOG_FUNC AutoLog logAutoFuncc (__FUNCTION__) ;

#endif // _KLOG_H_
