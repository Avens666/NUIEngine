//  **************************************
//  File:        NE_type.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description: This code is part of NUI Engine (NUI Graphics Lib)
//  Comments:	定义基础数据类型
//  Rev:         2
//  Created:     2017/4/12
//  Last edit:   2017/4/28
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//  License: APACHE V2.0 (see license file) 
//  ***************************************

#ifndef _NE_TYPE_H_
#define _NE_TYPE_H_

#include <string>
#include "Platform.h"
typedef unsigned int	    kn_uint;
typedef unsigned short	    kn_ushort;
typedef unsigned long	    kn_ulong;

typedef unsigned char	    kn_byte;
typedef int				    kn_int;
typedef short			    kn_short;
typedef long			    kn_long;
typedef float			    kn_float;
typedef double			    kn_double;
typedef bool			    kn_bool;
typedef unsigned short	    kn_word;
typedef unsigned long	    kn_dword;
typedef void*			    kn_handle;
typedef unsigned long long  kn_uint64;
typedef long long			kn_int64;
//#if defined(WIN32)
//    typedef signed __int64      kn_int64;
//    typedef unsigned __int64    kn_uint64;
//    //typedef kn_handle	        kn_hwnd;
//#endif // WIN32
#include "NE_configure.h"
#ifdef WIN32
#include "Windows.h"
	typedef HWND        kn_hwnd;
#endif // WIN32

#ifdef _UNICODE
    typedef wchar_t             kn_char;
#else
    typedef char			    kn_char;
#endif  // _UNICODE

typedef std::basic_string<kn_char>   kn_string;

#ifdef LINUX
	typedef long long		    kn_int64;
	typedef unsigned long long  kn_uint64;
	typedef kn_int64            LONG64;
	typedef kn_uint             UINT32;
	typedef kn_uint64           UINT64;
	typedef kn_int              INT32;
	typedef long long           __int64;
	typedef __int64             INT64;
	#ifndef IOS_DEV
		typedef kn_char TCHAR;
		typedef const TCHAR* LPCTSTR;
		typedef char 			CHAR;
	#endif
	typedef unsigned short 	WORD;
	typedef unsigned long 	DWORD;
	typedef unsigned char 	BYTE;
	typedef unsigned char 	byte;
	typedef long 			LONG;
	typedef unsigned int UINT;
#endif // LINUX

// 函数返回值
#ifdef WIN32
    typedef kn_uint KN_THREAD_PROC_RETURN_VALUE_TYPE;
#else
    typedef void* KN_THREAD_PROC_RETURN_VALUE_TYPE;
#endif

#endif // _NE_TYPE_H_
