//  **************************************
//  File:        KLog.cpp
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
#include "KLog.h"
#include "NE_pubhead.h"
#if defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)
#include <qdebug.h>
#endif

void MyTraceA(const char* szFormat, ... )
{
	const size_t BUFFER_SIZE = 1024;

	char szBuffer [BUFFER_SIZE] = {0};

	va_list args;
	va_start(args, szFormat);
	int iFormatLen = _vsnprintf(szBuffer, BUFFER_SIZE, szFormat, args);
	va_end(args);

	strncat(szBuffer, "\r\n", 2);
#ifdef WIN32
#ifndef _WIN32_WCE
	OutputDebugStringA(szBuffer);
#endif
#endif

}

void MyTrace(LPCTSTR lpsz, ...)
{
	va_list args;
	va_start(args, lpsz);
	TCHAR szBuffer[512] = { 0 };
	_vsntprintf(szBuffer, 512, lpsz, args);

	
	
	va_end(args);

	_tcsncat(szBuffer, _T("\r\n"), 2);
	::OutputDebugString(szBuffer);
}


//////////////////////////////////////////////////////////////////////////





static FILE* s_pFile = NULL;
static DWORD s_dwTicks = GetTickCount();

static DWORD s_dwTicksQT = GetTickCount();

void BeginLog()
{

	EndLog();


	wchar_t wszLogFile[MAX_PATH] = {0};

	//	log文件和可执行文件在同一目录
	GetModuleFileName(NULL, wszLogFile, MAX_PATH);
	wchar_t wszExFile[] = L".log";
	wcsncat (wszLogFile, wszExFile, wcslen(wszExFile));

	s_pFile = _wfopen (wszLogFile, L"abc");

	if(s_pFile == NULL)
	{
#ifdef WIN32
		MessageBox (NULL, wszLogFile, L"_wfopen error", 0);
#endif
	}
	else
	{
		WriteLog ("------------------ BEGIN ---------------------");
	}

}

void EndLog()
{
	if (s_pFile != NULL)
	{
		WriteLog ("------------------ END ---------------------");
		fclose (s_pFile);
		s_pFile = NULL;
	}
}

void WriteLog( const char *szFormat, ... )
{
	return;
	if (s_pFile == NULL)
	{
		BeginLog();

		if (s_pFile == NULL)
		{
			return;
		}

	}

	char szSystemTime[64] = {0};

	SYSTEMTIME st;
	GetLocalTime(&st);


	//[2008-04-28  10:31:54]
	int iTimeLen = sprintf(szSystemTime, "[%5d][%d-%02d-%02d  %02d:%02d:%02d] ", GetTickCount() - s_dwTicks, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	s_dwTicks = GetTickCount();
	const size_t BUFFER_SIZE = 1024;

	char szBuffer [BUFFER_SIZE] = {0};

	va_list args;
	va_start(args, szFormat);
	int iFormatLen = _vsnprintf(szBuffer, BUFFER_SIZE, szFormat, args);
	va_end(args);

	//	后缀
	const char szPostfix[4] = "\r\n\0";
	const int iPostfixLen = strlen(szPostfix);

	if (iTimeLen < 0)
	{
		iTimeLen = 0;
	}

	if (iFormatLen < 0)
	{
		iFormatLen = 0;
	}


	//	避免超出缓冲区 
	if (iTimeLen + iFormatLen + sizeof(szPostfix) > BUFFER_SIZE)
	{
		iFormatLen = BUFFER_SIZE - iTimeLen - sizeof(szPostfix);
	}

	//	拼接字符串
	char szLogMessage[BUFFER_SIZE] = {0};
	memcpy (szLogMessage, szSystemTime, iTimeLen);
	memcpy (szLogMessage + iTimeLen, szBuffer, iFormatLen);
	memcpy (szLogMessage + iTimeLen + iFormatLen, szPostfix, sizeof(szPostfix));

	// 		string str = szSystemTime;
	// 		str += szBuffer;
	// 		str += "\r\n";

	int iMessageLen = iTimeLen + iFormatLen + iPostfixLen;

	fwrite (szLogMessage, sizeof(char), iMessageLen, s_pFile);
	fflush(s_pFile);
}


//	不带时间
void WriteLogNoTime( const char *szFormat, ... )
{
	if (s_pFile == NULL)
	{
		return;

	}

	const size_t BUFFER_SIZE = 1024;

	char szBuffer [BUFFER_SIZE] = {0};
	va_list args;
	va_start(args, szFormat);
	_vsnprintf(szBuffer, BUFFER_SIZE, szFormat, args);
	va_end(args);

	fwrite (szBuffer, sizeof(char), strlen(szBuffer), s_pFile);

	//fflush(s_pFile);
}


//	判断可执行文件路径下是否存在 assert.txt， 只判断一次
BOOL IsNeedAssertLog ()
{

	static BOOL s_bChecked = FALSE;
	static BOOL s_bFileExist = FALSE;

	if (!s_bChecked)
	{
		s_bFileExist = FALSE;
		wchar_t wszName[MAX_PATH] = {0}; 
		const wchar_t wszAssertFile[] = L"assert.txt";

#ifdef WIN32

		//	win32 & wince 下取全路径
		GetModuleFileName(NULL, wszName, MAX_PATH);

		int len = wcslen(wszName);

		for (int i = len - 1; i > 0; i--)
		{
			if (L'\\' == wszName[i])
			{
				break;
			}
			else
			{
				wszName[i] = 0;
			}

		}
#endif
		wcscat(wszName, wszAssertFile);

		FILE* fs = _wfopen(wszName, L"rb");

		if (NULL != fs)
		{
			s_bFileExist = TRUE;
			fclose(fs);
		}

		s_bChecked = TRUE;
	}

	return s_bFileExist;

}

BOOL IsNeedLog ()
{

	static BOOL s_bChecked = FALSE;
	static BOOL s_bFileExist = FALSE;

	if (!s_bChecked)
	{
		s_bFileExist = FALSE;

		char szLogFlag [] = "./debug.txt";


		FILE* fs = fopen(szLogFlag,  "rb");

		if (NULL != fs)
		{
			s_bFileExist = TRUE;
			fclose(fs);
		}

		s_bChecked = TRUE;
	}

	return s_bFileExist;

}

/***********************************************************************
* Function :	输出断言信息 
*
* Parameter: const char * szFileName	源文件名
* Parameter: const long iLine			行号
* Parameter: const char * szFuncName	函数名
* Parameter: const char * szExpr		断言信息
*
* Returns:   void
***********************************************************************/
void LogAssert(const char* szFileName, const long iLine, const char* szFuncName, const char* szExpr)
{
	if (IsNeedAssertLog())
	{
		WriteLog ("[ASSERT] [%s] [#%d] [%s] [%s]",  szFileName, iLine, szFuncName, szExpr);
	}
}

#if defined(QT_WINDOWS_LIB) || defined(QT_LIB)

void LogInfo(const wchar_t *szFormat, ...)
{
	wchar_t szSystemTime[64] = {0};

	SYSTEMTIME st;
	GetLocalTime(&st);


	//[2008-04-28  10:31:54]
	int iTimeLen = swprintf(szSystemTime,  L"[%d-%02d-%02d  %02d:%02d:%02d] [%d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, GetTickCount());

	s_dwTicks = GetTickCount();
	const size_t BUFFER_SIZE = 1024;

	wchar_t szBuffer [BUFFER_SIZE] = {0};

	va_list args;
	va_start(args, szFormat);
	int iFormatLen = vswprintf(szBuffer, szFormat, args);
	va_end(args);

	//	缀
	const wchar_t szPostfix[4] = L"\r\n\0";
	const int iPostfixLen = wcslen(szPostfix);

	if (iTimeLen < 0)
	{
		iTimeLen = 0;
	}

	if (iFormatLen < 0)
	{
		iFormatLen = 0;
	}


	//	獬?
	if (iTimeLen + iFormatLen + sizeof(szPostfix) > BUFFER_SIZE)
	{
		iFormatLen = BUFFER_SIZE - iTimeLen - sizeof(szPostfix);
	}

	//	拼址
	wchar_t szLogMessage[BUFFER_SIZE] = {0};
	memcpy (szLogMessage, szSystemTime, iTimeLen);
	memcpy (szLogMessage + iTimeLen, szBuffer, iFormatLen);
	memcpy (szLogMessage + iTimeLen + iFormatLen, szPostfix, sizeof(szPostfix));

	// 		string str = szSystemTime;
	// 		str += szBuffer;
	// 		str += "\r\n";


#if defined(QT_WINDOWS_LIB) || defined(QT_LIB)
	//qDebug("%ls",szLogMessage);
#else
	printf(("%ls"), szLogMessage);
#endif
}

#endif



#if defined(QT_WINDOWS_LIB) || defined(QT_LIB) || defined(QT_LINUX_LIB)


#include <pthread.h>

void LogInfo(const char *szFormat, ...)
{
	if(!IsNeedLog())
	{
		return;

	}
	char szSystemTime[64] = {0};

	SYSTEMTIME st;
	GetLocalTime(&st);

#ifdef QT_WINDOWS_LIB
	//[2008-04-28  10:31:54]
	int iTimeLen = sprintf(szSystemTime, "[%d-%02d-%02d  %02d:%02d:%02d] [%d][p%d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
		GetTickCount() - s_dwTicksQT,  GetCurrentThreadId());

#else
	int iTimeLen = sprintf(szSystemTime, "[%d-%02d-%02d  %02d:%02d:%02d] [%d][p%d]", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
		GetTickCount() - s_dwTicksQT,  pthread_self());

#endif
	s_dwTicksQT = GetTickCount();

	const size_t BUFFER_SIZE = 1024;

	char szBuffer [BUFFER_SIZE] = {0};

	va_list args;
	va_start(args, szFormat);
	int iFormatLen = _vsnprintf(szBuffer, BUFFER_SIZE, szFormat, args);
	va_end(args);

	//	缀
	const char szPostfix[4] = "\r\n\0";
	const int iPostfixLen = strlen(szPostfix);

	if (iTimeLen < 0)
	{
		iTimeLen = 0;
	}

	if (iFormatLen < 0)
	{
		iFormatLen = 0;
	}


	//	獬?
	if (iTimeLen + iFormatLen + sizeof(szPostfix) > BUFFER_SIZE)
	{
		iFormatLen = BUFFER_SIZE - iTimeLen - sizeof(szPostfix);
	}

	//	拼址
	char szLogMessage[BUFFER_SIZE] = {0};
	memcpy (szLogMessage, szSystemTime, iTimeLen);
	memcpy (szLogMessage + iTimeLen, szBuffer, iFormatLen);
	// memcpy (szLogMessage + iTimeLen + iFormatLen, szPostfix, sizeof(szPostfix));

	// 		string str = szSystemTime;
	// 		str += szBuffer;
	// 		str += "\r\n";
#ifdef LINUX_ARM
	printf("%s\n", szLogMessage);
#else

	qDebug("%s",szLogMessage);
#endif

}

#endif




//  auto log
AutoLog::AutoLog(const char* szFuncName)
{
	LOGI ("Func %s Begin", szFuncName);
	memset(m_szFuncName, 0, sizeof(m_szFuncName));
	strncpy(m_szFuncName, szFuncName, 256);
}

AutoLog:: ~AutoLog()
{
	LOGI ("Func %s end", m_szFuncName);
}





