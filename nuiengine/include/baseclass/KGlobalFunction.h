//  **************************************
//  File:        KGlobalFunction.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:
//  Rev:         2
//  Created:     2017/4/12
//  Last edit:   2017/4/28
//  Author:      zhifangh
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************


#ifndef _KGLOABALFUNCTIN_
#define _KGLOABALFUNCTIN_

#include <stdlib.h>

#include "NE_pubhead.h"
#include "FilePath.h"
#include "CharSet.h"
#include "KLog.h"

void TraceChars (const char* psz);
void TraceCharsW (const wchar_t* psz);
void TraceBytes (const char* pszTAG, BYTE* pBuffer, int iCount);

// #if defined(ANDROID_NDK) || defined(IOS_DEV)
// inline size_t mbstowcs ( wchar_t * wcstr, const char * mbstr, size_t max )
// {
	// return ConvertGBK2Unicode((char*)mbstr, max, wcstr);
// }

// inline size_t wcstombs ( char * mbstr, const wchar_t * wcstr, size_t max )
// {
	// return ConvertUnicode2GBK((wchar_t*)wcstr, max, mbstr);
// }
// #endif


//#define RTIC_DEMO	//实时交通demo宏

//通过输入物理文件的数据获得物理文件中所在地址
KNAPI kn_uint GetAddress(kn_byte acAddress[]);

/*
 * 函数功能： 把字节数组整合成一个kn_word型变量
 * 参数：
 * 		 temp [in] ：字节数组
 * 		 n [in] ：数组元素个数
 * 返回值：kn_word型变量
 */
KNAPI kn_word GetWORD(const kn_byte temp[], kn_int n);

/*
* 函数功能： 把字节数组整合成一个kn_dword型变量
* 参数：
* 		value [in] ：字节数组
* 		value [in] ：数组元素个数
* 返回值：kn_dword型变量
*/
KNAPI kn_dword GetDWORD(const kn_byte temp[], kn_int n);

/*
* 函数功能：将一个字节数组倒序 
* 参    数：
* 		pSourse [in] ：源字节数组的头指针
* 		n [in] ：目标数组的大小
* 		abyResult [out]：值：倒序后的字节数组
* 返 回 值：无
*/ 

#define ReverseByteArray(pSourse, abyResult, n) \
											{for (kn_int iDefine = 0 ; iDefine < n ; iDefine++) \
											{									\
											abyResult[iDefine] = pSourse[n- 1- iDefine];	\
											}}

#define HIDWORD(l)   ((kn_dword) (((__int64) (l) >> 32) & 0xFFFFFFFF)) 
#define LODWORD(l)   ((kn_dword) (l)) 
#define MAKEINT64(a, b) ((__int64) (((kn_dword) (a)) | ((__int64) ((DWORD) (b))) << 32)) 

KNAPI void* DumpFile ( IN LPCTSTR strFileName, 
					   IN DWORD	dwMapStart,
					   IN DWORD	dwBuffSize );

KNAPI void long_reverse(kn_long &value);

KNAPI void int_reverse(kn_uint &value);

KNAPI void short_reverse(kn_ushort &value);

KNAPI void __int64_Reverse(__int64 &value);

#ifdef RTIC_DEMO
#define  RT_FREQ	60 //实时频率，单位为秒
enum JAME_TYPE
{
	RED_TYPE = 0,	//拥堵，红色
	YELLOW_TYPE,	//缓慢，黄色
	GREEN_TYPE,		//通畅，绿色
};

extern kn_bool g_bRTIC;		//是否启用RTIC
extern kn_bool g_bRPlan;	//是否发生重计算 

JAME_TYPE GetLinkJametype(kn_dword dwLinkId);

//通过执行文件名获得进程ID的方法
kn_dword GetProcessIdFromName(LPCTSTR name);
void KillProgram(LPCTSTR ExeName);
#endif

//	字符串分割函数
void string_split(const string &strSrc, vector<string>&arrStrDst, string::value_type separator);

#endif // _KGLOABALFUNCTIN_
