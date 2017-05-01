//  **************************************
//  File:        KGlobalFunction.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         3
//  Created:     2017/4/12
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************

#include "KGlobalFunction.h"
#include "KNFile.h"
//#include "IPosition.h"
#ifdef RTIC_DEMO
#include <Tlhelp32.h>
#ifndef Navi_Demo
#define Navi_Demo
#endif
#endif

#ifdef ANDROID_NDK
#define printf LOGI
#endif

// 通过输入物理文件的数据获得物理文件中所在地址
KNAPI kn_uint GetAddress(kn_byte abyddress[])
{
	return GetDWORD(abyddress , 3) * 0x800 + abyddress[3] * 0x20;
}

/*
 * 函数功能： 把字节数组整合成一个kn_word型变量
 * 参数：
 * 		 temp [in] ：字节数组
 * 		 n [in] ：数组元素个数
 * 返回值：kn_word型变量
 */
KNAPI kn_word GetWORD(const kn_byte temp[], kn_int n)
{
	kn_word wtemp = 0;
	for (kn_int i = 0; i < n - 1; ++i)
	{
		wtemp |= temp[i];
		wtemp <<= 8;
	}
	wtemp |= temp[n - 1];
	return wtemp;
}

/*
* 函数功能： 把字节数组整合成一个kn_dword型变量
* 参数：
* 		value [in] ：字节数组
* 		value [in] ：数组元素个数
* 返回值：kn_dword型变量
*/
KNAPI kn_dword GetDWORD(const kn_byte temp[], kn_int n)
{
	kn_dword dwtemp = 0;
	for (kn_int i = 0; i < n - 1; ++i)
	{
		dwtemp |= temp[i];
		dwtemp <<= 8;
	}
	dwtemp |= temp[n - 1];
	return dwtemp;
}

/*
* 函数功能：将一个字节数组倒序 
* 参    数：
* 		pSourse [in] ：源字节数组的头指针
* 		n [in] ：目标数组的大小
* 		abyResult [out]：值：倒序后的字节数组
* 返 回 值：无
*/ 
/*
inline void ReverseByteArray(kn_byte *pSourse , kn_byte abyResult[] , kn_int n)
{
	for (kn_int i = 0 ; i < n ; i++) 
	{
		abyResult[i] = pSourse[n- 1- i];
	}
	
}
*/

/*
* 函数功能：构造函数
* 参数：无
* 返回值：无
*/
//added by bind 2005/06/08/16:19
KNAPI void* DumpFile ( IN LPCTSTR strFileName, 
					   IN DWORD	dwMapStart,
					   IN DWORD	dwBuffSize )
{	
	return NULL;

/*
	SYSTEM_INFO SysInfo;
	DWORD		dwSysGran;
	DWORD		dwFileMapStart;
	DWORD		dwMapViewSize;
	DWORD		dwFileMapSize;
	LPVOID		lpFileBase;
	HANDLE  hFileMap = NULL;	
	HANDLE  hFile = NULL;
	
	// open the existed file
	hFile = CreateFile( filename, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		0 );
	
	if ( hFile == INVALID_HANDLE_VALUE ) 
	{
		MessageBox (NULL, "The file is not exist!", NULL, MB_OK);
		return NULL;
	}
	
	// Get the system allocation granularity
	GetSystemInfo (&SysInfo);
	dwSysGran = SysInfo.dwAllocationGranularity;
	
	// To calculate where to start the file mapping
	dwFileMapStart = ( dwMapStart / dwSysGran ) * dwSysGran;	
	
	// Calculate the size of the file mapping view
	dwMapViewSize = ( dwMapStart % dwSysGran ) + dwBuffSize;
	
	// How large will the file mapping object be
 	dwFileMapSize = dwMapStart + dwBuffSize;
	
#ifdef WIN__CE
	hFile = CreateFileForMapping (filename, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);
#endif

#ifdef ZERIN
	hFileMap = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 
		/ *dwFileMapSize* /0x2A61F800 , NULL);// 0x2A61F800为文件大小
#endif
#ifdef KOTEI
	hFileMap = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 
		0/ *0x526561* // *0xA08901* // *dwFileMapSize* /,NULL);// 0xA08901为文件大小
#endif
	
	
	if ( hFileMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS ) 
	{
		CloseHandle(hFile);
		MessageBox (NULL, "error handle of create file mapping!", 
			NULL, MB_OK);
		return NULL;
	}
	
//	lpFileBase = MapViewOfFile (hFileMap, FILE_MAP_READ, 0, 
//								dwFileMapStart, dwMapViewSize);
		lpFileBase = MapViewOfFile (hFileMap, FILE_MAP_READ, 0, 0,0);//映射整个文件
								/ *((int)(dwMapStart/(dwSysGran)))*dwSysGran , dwBuffSize + 2 * dwSysGran);* /
	
	if ( lpFileBase == NULL ) 
	{
		CloseHandle(hFileMap);
		CloseHandle(hFile);
		MessageBox (NULL, "error handle!", NULL, MB_OK);
		return NULL;
	}

	return lpFileBase;

 / *
    char *pResult = (char *)lpFileBase;
 	
 	return pResult + dwMapStart - ((int)(dwMapStart/dwSysGran))*dwSysGran;* / // 返回道路计算region数据的头
 
	//lpFileBase/ *[dwMapStart - ((int)(dwMapStart/dwSysGran))*dwSysGran]* /;*/

}

/*
* 函数功能：reverse Little-Endian Byte Ordering(long type)
* 参数：
*		value [in|out]：要变换的值
* 返回值：无
*/
KNAPI void long_reverse(kn_long& value)
{
	kn_byte BYTE1, BYTE2, BYTE3, BYTE4;
	
	BYTE1 = HIBYTE(HIWORD(value));
	BYTE2 = LOBYTE(HIWORD(value));
	BYTE3 = HIBYTE(LOWORD(value));
	BYTE4 = LOBYTE(LOWORD(value));
	
	value =	MAKELONG(MAKEWORD(BYTE1, BYTE2), MAKEWORD(BYTE3, BYTE4));
}

/*
* 函数功能：reverse Little-Endian Byte Ordering(interger type)
* 参数：
*		value [in|out]：要变换的值
* 返回值：无
*/
KNAPI void int_reverse(kn_uint& value)
{
	kn_byte BYTE1, BYTE2, BYTE3, BYTE4;
	
	BYTE1 = HIBYTE(HIWORD(value));
	BYTE2 = LOBYTE(HIWORD(value));
	BYTE3 = HIBYTE(LOWORD(value));
	BYTE4 = LOBYTE(LOWORD(value));
	
	value =	MAKELONG(MAKEWORD(BYTE1, BYTE2), MAKEWORD(BYTE3, BYTE4));
}

/*
* 函数功能：reverse Little-Endian Byte Ordering(unsigned short type)
* 参数：
*		value [in|out]：要变换的值
* 返回值：无
*/
KNAPI void short_reverse(kn_ushort& value)
{
	kn_byte BYTE1, BYTE2;
	
	BYTE1 = HIBYTE(value);
	BYTE2 = LOBYTE(value);
	
	value =	MAKEWORD(BYTE1, BYTE2);
}


/*
* 函数功能：reverse Little-Endian Byte Ordering(__int64 type)
* 参数：
*		value [in|out]：要变换的值
* 返回值：无
*/
KNAPI void __int64_Reverse(__int64& value)
{
	kn_dword DWORD1,DWORD2;
	
	DWORD1 = HIDWORD(value) ;
	DWORD2 = LODWORD(value);
	
	int_reverse((kn_uint&)DWORD1);
	int_reverse((kn_uint&)DWORD2);
	
	value =	MAKEINT64(DWORD1, DWORD2);
}

void TraceChars (const char* psz)
{
	printf ("TraceChars = %s\n", psz);
	char* p = (char*)psz;
	while (*p != 0)
	{
		printf ("%02X-", (BYTE)*p);
		p++;
	}
	printf ("\n");
}

void TraceCharsW (const wchar_t* psz)
{
	printf ("TraceCharsW = %S\n", psz);
	wchar_t* p = (wchar_t*)psz;
	while (*p != 0)
	{
		BYTE* pByTemp = (BYTE*)p;
		printf ("%02X-%02X-%02X-%02X--", pByTemp[0], pByTemp[1],pByTemp[2],pByTemp[3]);
		p++;
	}
	printf ("\n");
}

void TraceBytes (const char* pszTAG, BYTE* pBuffer, int iCount)
{
	const int LEN = 512;
	
	char szOut[LEN] = {0};
	
	int iLen = iCount ;
	if(iCount * 3  > LEN - 1)
	{
			iLen = LEN - 1;
	}
	

	for (int i = 0; i < iLen; i ++)
	{
		char szTemp[4] = {0};
		sprintf (szTemp, "%02X-", pBuffer[i]);
		strncat(szOut, szTemp, 3);
	}
	
	printf("%s : %s\n", pszTAG, szOut);
	
	
	
}

#ifdef KOTEI_LINUX

//	可执行文件名
char g_szExeFile[MAX_PATH] = {0};

void SetModuleFileName (char* pszFile)
{
	if (g_szExeFile[0] == 0)
	{
		size_t len = strlen(pszFile);
		if (len > MAX_PATH)
		{
			len = MAX_PATH - 1;
		}

		printf ("SetModuleFileName pszFile  = %s, len = %d\n", pszFile, len);
		strncpy (g_szExeFile, pszFile, len);
		printf ("g_szExeFile  = %s\n", g_szExeFile);

	}
}

//	得到EXE路径
DWORD GetModuleFileName(void* hModule,LPWSTR lpFilename, DWORD nSize)
{
	size_t len = strlen(g_szExeFile);
	if (len > nSize)
	{
		len = nSize;
	}

	//	linux目录不能包含中文，linux下的中文路径为uft-8编码，非GBK编码
	mbstowcs(lpFilename, g_szExeFile, len);
	return 0;
}

FILE *_wfopen(const wchar_t* filename, const wchar_t* mode)
{
	char szMode [8] = {0};
	wcstombs(szMode, mode, wcslen(mode));

	char szFileName [MAX_PATH] = {0};

	//	转换成linux的目录格式
	wchar_t* p = (wchar_t*)filename;
	while (*p != 0)
	{
		if (*p == L'\\')
		{
			*p = L'/';
		}

#ifdef C2_SDK
		//	设备上全部转小写
		*p = ::tolower (*p);

#endif

		p++;
	}

	WideCharToMultiByte (936, 0, filename, wcslen(filename), szFileName, MAX_PATH, NULL, NULL);

	FILE* fs = fopen (szFileName, szMode);
	if (fs == NULL)
	{
		printf ("wopen error: %s\n", szFileName);
	}

	return fs;
}

int swprintf_linux( wchar_t *buffer, const wchar_t *format, ... )
{
	const size_t BUFFER_SIZE = 512;

	if (wcslen (format) >= BUFFER_SIZE)
	{
		return 0;
	}

	//	替换%s为%S, format 无法直接修改,szTempFormat为临时转换的区域
	wchar_t szTempFormat[BUFFER_SIZE] = {0};

	wchar_t *pSrc = (wchar_t *)format;
	wchar_t *pDst = (wchar_t *)szTempFormat;

	while ( *pSrc != 0)
	{
        // %s -> %ls
		if (*pSrc == L'%' && *(pSrc+1) == L's')
		{
			*pDst = *pSrc;
            // *(pDst + 1) = L'S';
            *(pDst + 1) = L'l';
            *(pDst + 2) = L's';

			pSrc += 2;
            pDst += 3;
		}
        //  %S -> %s
        else if(*pSrc == L'%' && *(pSrc+1) == L'S')
        {
            *pDst = *pSrc;
            *(pDst + 1) = L's';

            pSrc += 2;
            pDst += 2;
        }
		else
		{
			*pDst = *pSrc;
			pSrc ++;
			pDst ++;
		}

	}


	va_list args;
	va_start(args, format);
	wchar_t szBuffer[BUFFER_SIZE] = {0};
	vswprintf(szBuffer, BUFFER_SIZE, szTempFormat, args);
	va_end(args);

	size_t iRetLen = wcslen(szBuffer);
	wcsncpy (buffer, szBuffer, iRetLen);

	return iRetLen;

}
#endif


#ifdef	RTIC_DEMO	

static JAME_TYPE g_jameTable[3][10] = 
{
	GREEN_TYPE,GREEN_TYPE,YELLOW_TYPE,YELLOW_TYPE,RED_TYPE,YELLOW_TYPE,YELLOW_TYPE,GREEN_TYPE,GREEN_TYPE,GREEN_TYPE,
	GREEN_TYPE,YELLOW_TYPE,YELLOW_TYPE,YELLOW_TYPE,RED_TYPE,RED_TYPE,YELLOW_TYPE,YELLOW_TYPE,GREEN_TYPE,GREEN_TYPE,
	GREEN_TYPE,GREEN_TYPE,GREEN_TYPE,YELLOW_TYPE,RED_TYPE,YELLOW_TYPE,YELLOW_TYPE,GREEN_TYPE,GREEN_TYPE,GREEN_TYPE,
};					//jame表

static kn_dword g_oldTick = GetTickCount();	//时间备份
kn_bool g_bRTIC = false;
kn_bool g_bRPlan = false;

static kn_int g_iIndex = 0;
static kn_int g_jIndex = 0;


JAME_TYPE GetLinkJametype(kn_dword dwLinkId)
{
	kn_dword nowTick = GetTickCount();
	//超过RT_FREQ，获取新的实时交通信息
	if (nowTick - g_oldTick > RT_FREQ*1000)
	{
		g_oldTick = nowTick;
		g_iIndex = (nowTick/RT_FREQ/1000)%3;
		g_jIndex = (nowTick/RT_FREQ/1000)%10;
	}

	//IPosition* iPos = GetPositionSingleton();
	//const KNGPSTIME& gpsTime = iPos->GetGPSTime();

	//if (gpsTime.byMonth == 5&&		//5.23日信号
	//	gpsTime.byDay== 23)
	//{
	//	kn_int iSecond = gpsTime.byMinute*60 + gpsTime.bySecond;

	//	if (iSecond == 27*60+3 || 
	//		iSecond == 29*60+20 ||
	//		iSecond == 30*60+56||
	//		iSecond == 32*60+1 ||
	//		iSecond == 33*60+22||
	//		iSecond == 34*60+51)
	//	{
	//		g_bRPlan = true;
	//	}
	//	else
	//	{
	//		g_bRPlan = false;
	//	}

	//	if (iSecond>= 27*60+3 && iSecond <= 29*60+20)
	//	{
	//		switch (dwLinkId)
	//		{
	//			case 23570641:
	//			case 23570642:
	//			case 23570643:
	//			case 23570644:
	//			case 23603797:
	//			case 23603798:
	//			case 23603799:
	//			case 23570652:
	//			case 23603796:
	//				return GREEN_TYPE;
	//			case 23603771:
	//			case 23603770:
	//			case 23604009:
	//			
	//				return RED_TYPE;
	//			break;
	//		}
	//	}
	//	else if (iSecond>=29*60+21 && iSecond <= 30*60+56)
	//	{
	//		switch (dwLinkId)
	//		{
	//		case 23570641:
	//		case 23570642:
	//		case 23570643:
	//		case 23570644:
	//		case 23603797:
	//		case 23603798:
	//		case 23603799:
	//		case 23557846:
	//		case 23558393:
	//			return GREEN_TYPE;
	//		case 23603771:
	//		case 23603770:
	//		case 23604009:
	//		case 23570652:
	//		case 23603796:
	//			return RED_TYPE;
	//			break;
	//		}
	//	}
	//	else if (iSecond>=30*60+57 && iSecond <= 32*60+1)
	//	{
	//		switch (dwLinkId)
	//		{
	//			case 23603797:
	//			case 23603798:
	//			case 23603799:
	//			case 23557847:
	//			case 23557848:
	//				return GREEN_TYPE;
	//			case 23603949:
	//			case 23558393:
	//			case 23557846:
	//				return RED_TYPE;
	//		}
	//	}
	//	else if (iSecond>=32*60+2 && iSecond <= 33*60+22)
	//	{
	//		switch (dwLinkId)
	//		{
	//		case 23557847:
	//		case 23557848:
	//		case 23557850:
	//		case 23557851:
	//		case 23557852:
	//		case 23557853:
	//			return GREEN_TYPE;
	//		case 23557849:
	//		case 23558695:
	//		case 23570673:
	//		case 23603771:
	//		case 23603770:
	//		case 23604009:
	//		case 23570652:
	//		case 23603796:
	//		case 23557846:
	//		case 23558393:
	//			return RED_TYPE;
	//		}
	//	}
	//	else if (iSecond>=33*60+23 && iSecond < 34*60+51)
	//	{
	//		switch (dwLinkId)
	//		{
	//		case 23557850:
	//		case 23557851:
	//		case 23557852:
	//		case 23557853:
	//			return GREEN_TYPE;
	//		case 23557920:
	//		case 23595473:
	//		case 23557872:
	//		case 23595502:
	//		case 23557849:
	//		case 23558695:
	//		case 23570673:
	//		case 23603771:
	//		case 23603770:
	//		case 23604009:
	//		case 23570652:
	//		case 23603796:
	//			return RED_TYPE;
	//		}
	//	}
	//	else if (iSecond>=34*60+51)
	//	{
	//		g_bRTIC = false;
	//		iPos->Reset(false);
	//		//设置车标位置
	//		iPos->SetCarPosition(KNGEOCOORD(447635422, 114918539));
	//	}
	//}

	//if (dwLinkId == 23603686 || 
	//	dwLinkId == 23603687 ||
	//	dwLinkId == 23603688 ||
	//	dwLinkId == 23570638 ||
	//	dwLinkId == 23570639 ||
	//	dwLinkId == 23570640 ||
	//	dwLinkId == 23603797 ||
	//	dwLinkId == 23603798)
	//{
	//	return RED_TYPE;
	//}
	//else if (dwLinkId == 23570641)
	//{
	//	return GREEN_TYPE;
	//}	
	//else
	{
		return g_jameTable[g_iIndex][(dwLinkId + g_jIndex)%10];
	}
	
}

//通过执行文件名获得进程ID的方法
kn_dword GetProcessIdFromName(LPCTSTR name)    
{
	kn_dword id = 0;
#ifdef Navi_Demo
	PROCESSENTRY32 pe;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if( !Process32First(hSnapshot,&pe) )
		return 0;

	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if( Process32Next(hSnapshot,&pe)==FALSE )
			break;
		if(_tcscmp(pe.szExeFile,name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}

	} while(1);

	CloseHandle(hSnapshot);
#endif

	return id;
}

void KillProgram(LPCTSTR ExeName)   
{   
#ifdef Navi_Demo
	LPCTSTR File;   
	HANDLE hProcessSnap;   
	PROCESSENTRY32 pe32;   

	if (!ExeName || !ExeName[0])
		return;   
	File = _tcsrchr(ExeName, _T('\\'));   
	if (File!=0)
		ExeName = File+1;   
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   
	if (hProcessSnap == (HANDLE)-1)  
		return;   
	memset(&pe32, 0, sizeof(pe32));   
	pe32.dwSize = sizeof(PROCESSENTRY32);   
	if (Process32First(hProcessSnap, &pe32))   
	{   
		do {
			File =  _tcsrchr(pe32.szExeFile, _T('\\'));   
			File = File?   File+1   :   pe32.szExeFile;   
			if (_wcsicmp(File,ExeName)==0)  
			{   
				TerminateProcess(OpenProcess(PROCESS_ALL_ACCESS, 0, pe32.th32ProcessID), 0);   
				break;   
			}   
		}   
		while(Process32Next(hProcessSnap,&pe32));   
	}   
	CloseHandle(hProcessSnap);  
#endif
}

#endif

void string_split(const string &strSrc, vector<string>&arrStrDst, string::value_type separator)
{
	arrStrDst.clear();
	string::const_iterator i = strSrc.begin(), last = i;
	for (; i != strSrc.end(); i++)
	{
		if (*i == separator)
		{
			arrStrDst.push_back(string(last, i));
			last = i + 1;
		}
	}
	if (last != strSrc.begin())
	{
		arrStrDst.push_back(string(last, i));
	}
}


