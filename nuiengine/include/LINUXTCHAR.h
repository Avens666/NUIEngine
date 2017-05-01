//  **************************************
//  File:        LINUXTCHAR.h
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
#ifndef _LINUXTCHAR_H__
#define _LINUXTCHAR_H__

#ifdef KOTEI_LINUX

/**/
#include "NE_type.h"
#include "NE_macro.h"
//#include "NE_pubhead.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>
#include <sys/time.h>
#include <wchar.h>
#include <wctype.h>
#include <stdarg.h>
#include <time.h>
#include <pthread.h>
#include <strings.h>

#if defined(ANDROID_NDK) 

size_t kn_wcstombs(char * dst, const wchar_t * src, size_t len);
size_t kn_mbstowcs(wchar_t * dst, const char * src, size_t len);

#ifndef wcstombs
#define wcstombs kn_wcstombs
#define mbstowcs kn_mbstowcs
#endif


#endif



#define TRUE 1
#define FALSE 0

//
//  Code Page Default Values.
//
#define CP_ACP                    0           // default to ANSI code page
#define CP_OEMCP                  1           // default to OEM  code page
#define CP_MACCP                  2           // default to MAC  code page
#define CP_THREAD_ACP             3           // current thread's ANSI code page
#define CP_SYMBOL                 42          // SYMBOL translations

#define CP_UTF7                   65000       // UTF-7 translation
#define CP_UTF8                   65001       // UTF-8 translation
//
//  MBCS and Unicode Translation Flags.
//
#define MB_PRECOMPOSED            0x00000001  // use precomposed chars
#define MB_COMPOSITE              0x00000002  // use composite chars
#define MB_USEGLYPHCHARS          0x00000004  // use glyph chars, not ctrl chars
#define MB_ERR_INVALID_CHARS      0x00000008  // error for invalid chars

#define WC_COMPOSITECHECK         0x00000200  // convert composite to precomposed
#define WC_DISCARDNS              0x00000010  // discard non-spacing chars
#define WC_SEPCHARS               0x00000020  // generate separate chars
#define WC_DEFAULTCHAR            0x00000040  // replace w/ default char

#define WC_NO_BEST_FIT_CHARS      0x00000400  // do not use best fit chars

#define MINCHAR     0x80        
#define MAXCHAR     0x7f        
#define MINSHORT    0x8000      
#define MAXSHORT    0x7fff      
#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  
#define MAXBYTE     0xff        
#define MAXWORD     0xffff      
#define MAXDWORD    0xffffffff

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

// #define INT_MIN     (-2147483647 - 1) /* minimum (signed) int value */
// #define INT_MAX     2147483647    /* maximum (signed) int value */

typedef unsigned int WPARAM;
typedef long LPARAM;

#define INVALID_FILDES_VALUE -1

#define INFINITE 							0xFFFFFFFF
#define GENERIC_READ 						(0x80000000L)
#define GENERIC_WRITE                    	(0x40000000L)
#define GENERIC_EXECUTE                  	(0x20000000L)
#define GENERIC_ALL                      	(0x10000000L)
#define FILE_SHARE_READ      				0x00000001
#define OPEN_EXISTING       				3
#define FILE_ATTRIBUTE_READONLY             0x00000001
#define FILE_ATTRIBUTE_NORMAL				0x00000080
#define FILE_ATTRIBUTE_ARCHIVE              0x00000020

// #define VK_BACK           0x08

#define VK_UP       0x26
#define VK_DOWN     0x28
#define VK_LEFT     0x25
#define VK_RIGHT    0x27
#define VK_ESCAPE   0x1B
#define VK_RETURN   0x0D
// #define VK_BACK     0x0D
#define VK_BACK     0x08
#define VK_PRIOR    0x21
#define VK_NEXT     0x22

#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SEPARATOR      0x6C
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_F13            0x7C
#define VK_F14            0x7D
#define VK_F15            0x7E
#define VK_F16            0x7F
#define VK_F17            0x80
#define VK_F18            0x81
#define VK_F19            0x82
#define VK_F20            0x83
#define VK_F21            0x84
#define VK_F22            0x85
#define VK_F23            0x86
#define VK_F24            0x87

#define VK_CLEAR          0x0C
#define VK_RETURN         0x0D

#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_PAUSE          0x13
#define VK_CAPITAL        0x14

#define VK_KANA           0x15
#define VK_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VK_HANGUL         0x15
#define VK_JUNJA          0x17
#define VK_FINAL          0x18
#define VK_HANJA          0x19
#define VK_KANJI          0x19

#define VK_ESCAPE         0x1B

#define VK_CONVERT        0x1C
#define VK_NONCONVERT     0x1D
#define VK_ACCEPT         0x1E
#define VK_MODECHANGE     0x1F

#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_SELECT         0x29
#define VK_PRINT          0x2A
#define VK_EXECUTE        0x2B
#define VK_SNAPSHOT       0x2C
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_HELP           0x2F

#define WM_USER           0x0400

#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101

#define INVALID_HANDLE_VALUE NULL
#define CREATE_NEW				1
#define CREATE_ALWAYS       	2
#define OPEN_EXISTING       	3
#define OPEN_ALWAYS         	4
#define TRUNCATE_EXISTING   	5
#define FILE_BEGIN				0
#define FILE_CURRENT         	1
#define FILE_END             	2

#define O_TEXT         0x4000  /* file mode is text (translated) */

/* Pen Styles */
#define PS_SOLID            0
#define PS_DASH             1       /* -------  */
#define PS_DOT              2       /* .......  */
#define PS_DASHDOT          3       /* _._._._  */
#define PS_DASHDOTDOT       4       /* _.._.._  */
#define PS_NULL             5
#define PS_INSIDEFRAME      6
#define PS_USERSTYLE        7
#define PS_ALTERNATE        8
#define PS_STYLE_MASK       0x0000000F


#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203

typedef  void* HWND;

typedef const char     *LPCSTR;
typedef wchar_t        *LPWSTR;
typedef const wchar_t  *LPCWSTR;
typedef char           *LPSTR;
typedef bool           *LPBOOL;

typedef void           *LPVOID;

typedef void *HINSTANCE;

typedef wchar_t  WCHAR;

#ifdef IOS_DEV

#ifndef OBJC_BOOL_DEFINED  // Typedef redefinition with different types ('signed char' vs 'bool')
typedef  signed  char BOOL;
#endif

#else
typedef int BOOL;
#endif

typedef long COLORREF;

typedef void*           kn_hwnd;
typedef unsigned short 	WORD;
typedef unsigned long 	DWORD;
typedef unsigned char 	BYTE;
typedef unsigned char 	byte;
typedef long 			LONG;
typedef char 			CHAR;
typedef unsigned int    UINT;

#define HIWORD(l)   ((WORD) (((DWORD) (l) >> 16) & 0xFFFF))
#define HIBYTE(w)   ((kn_byte) (((WORD) (w) >> 8) & 0xFF))
#define LOBYTE(w)   ((kn_byte) (w))
#define LOWORD(l)   ((WORD) (l))
#define MAKELONG(a, b) ((LONG) (((WORD) (a)) | ((DWORD) ((WORD) (b))) << 16))
#define MAKEWORD(a, b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))


#define _vsnprintf vsnprintf
#define _vsnwprintf vswprintf
#define MB_OK   0x00000000L

inline kn_bool PostMessage(
                    IN kn_hwnd hWnd,
                    IN kn_uint Msg,
                    IN WPARAM wParam,
                    IN LPARAM lParam)
{
    return false;
}

inline kn_bool SendMessage(
                    IN kn_hwnd hWnd,
                    IN kn_uint Msg,
                    IN WPARAM wParam,
                    IN LPARAM lParam)
{
    return false;
}

inline void OutputDebugString(IN LPCWSTR lpOutputString)
{
    return;
}

typedef pthread_mutex_t CRITICAL_SECTION;
inline void InitializeCriticalSection(CRITICAL_SECTION* lpCriticalSection)
{
	pthread_mutex_init (lpCriticalSection, NULL);
}

inline void DeleteCriticalSection(CRITICAL_SECTION* lpCriticalSection)
{
	pthread_mutex_destroy (lpCriticalSection);
}

inline void EnterCriticalSection(CRITICAL_SECTION* lpCriticalSection)
{
	pthread_mutex_lock (lpCriticalSection);
}

inline void LeaveCriticalSection(CRITICAL_SECTION* lpCriticalSection)
{
	pthread_mutex_unlock (lpCriticalSection);
}

inline void Sleep(DWORD dwMilliseconds)
{
	usleep(dwMilliseconds * 1000);
}


//-------------------------------------------------------------------
//	待完成 BEGIN
//-------------------------------------------------------------------
inline kn_handle CreateEvent(
				   void* lpEventAttributes,
				   BOOL bManualReset,
				   BOOL bInitialState,
				   const wchar_t* lpName
				   )
{
	return (kn_handle)0;
}

#define WAIT_OBJECT_0 (0)

inline DWORD WaitForSingleObject(
						  kn_handle hHandle,
						  DWORD dwMilliseconds
						  )
{
	return WAIT_OBJECT_0;

}

inline BOOL SetEvent(kn_handle hEvent)
{
	return TRUE;
}

inline BOOL ResetEvent(kn_handle hEvent)
{
	return TRUE;
}

//-------------------------------------------------------------------
//	待完成	END
//-------------------------------------------------------------------
inline long GetTickCount()
{
	timeval tv;
	static long s_basetime = 0;
	//	timezone tz;
	gettimeofday(&tv, NULL);
	if(s_basetime == 0)
	{
		s_basetime = tv.tv_sec;
	}
	return (tv.tv_sec - s_basetime)*1000 + tv.tv_usec/1000;

}

typedef struct tagSIZE {
	int cx;
	int cy;
} SIZE, *PSIZE;

//strutes LARGE_INTEGER
#if defined( MIDL_PASS )
typedef struct _LARGE_INTEGER {
#else  //MIDL_PASS
typedef union _LARGE_INTEGER {
	struct {
		unsigned long LowPart;
		long HighPart;
	};
	struct {
		unsigned long LowPart;
		long HighPart;
	} u;

#endif //MIDL_PASS
	double QuadPart;
} LARGE_INTEGER;

//struct RECT
//typedef struct _RECT {
//	long left;
//	long top;
//	long right;
//	long bottom;
//} RECT, *PRECT;
//struct POINT
typedef struct tagPOINT {
	long x;
	long y;
} POINT, *PPOINT;

typedef long LRESULT;

#define MAX_PATH 260

static void xtoa (
				  unsigned long val,
				  char *buf,
				  unsigned radix,
				  int is_neg
				  )
{
	char *p;                /* pointer to traverse string */
	char *firstdig;         /* pointer to first digit */
	char temp;              /* temp char */
	unsigned digval;        /* value of digit */

	p = buf;

	if (is_neg) {
		/* negative, so output '-' and negate */
		*p++ = '-';
		val = (unsigned long)(-(long)val);
	}

	firstdig = p;           /* save pointer to first digit */

	do {
		digval = (unsigned) (val % radix);
		val /= radix;       /* get next digit */

		/* convert to ascii and store */
		if (digval > 9)
			*p++ = (char) (digval - 10 + 'a');  /* a letter */
		else
			*p++ = (char) (digval + '0');       /* a digit */
	} while (val > 0);

	/* We now have the digit of the number in the buffer, but in reverse
	order.  Thus we reverse them now. */

	*p-- = '\0';            /* terminate string; p points to last digit */

	do {
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;   /* swap *p and *firstdig */
		--p;
		++firstdig;         /* advance to next two digits */
	} while (firstdig < p); /* repeat until halfway */
}

/* Actual functions just call conversion helper with neg flag set correctly,
and return pointer to buffer. */

inline char* _itoa (int val,char *buf,int radix)
{
	if (radix == 10 && val < 0)
		xtoa((unsigned long)val, buf, radix, 1);
	else
		xtoa((unsigned long)(unsigned int)val, buf, radix, 0);
	return buf;
}

inline wchar_t *_ltow( long value, wchar_t* string, int radix)
{
	char sz[128] = {0};

	_itoa (value, sz, radix);

	size_t len = strlen(sz);

	mbstowcs (string, sz, len);

	return string;
}

#define _itow _ltow

inline int _wtoi( const wchar_t *string )
{
#ifdef ANDROID_NDK
	// android 下没有实现wcstof
	char sz[128] = {0};
	wcstombs(sz, string, 60);

	return atoi(sz);
#else
	return wcstol(string, NULL, 10);
#endif
}

inline float _wtof( const wchar_t *string )
{
#ifdef ANDROID_NDK
	// android 下没有wcstof
	char sz[128] = {0};
	wcstombs(sz, string, 60);
	
	return atof(sz);
	
	
#else
    return wcstof(string, NULL);
#endif
}




inline wchar_t *wcstok( wchar_t *strToken, const wchar_t *strDelimit )
{
    wchar_t *context = NULL;
    return wcstok (strToken, strDelimit, &context);
}


int swprintf_linux( wchar_t *buffer, const wchar_t *format, ... );

FILE *_wfopen(const wchar_t* filename, const wchar_t* mode);


//	通过main的参数得到可执行文件名
void SetModuleFileName (char* pszFile);

//	得到EXE路径
DWORD GetModuleFileName(void* hModule,LPWSTR lpFilename, DWORD nSize);


typedef struct _SYSTEMTIME {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} SYSTEMTIME, *LPSYSTEMTIME;

inline void GetLocalTime (LPSYSTEMTIME lpSystemTime)
{
	time_t timep;
	time (&timep);

	struct tm *p = gmtime (&timep);

	lpSystemTime->wYear = 1900 + p->tm_year;
	lpSystemTime->wMonth = 1 + p->tm_mon;
	lpSystemTime->wDay = p->tm_mday;
	lpSystemTime->wHour = p->tm_hour;
	lpSystemTime->wMinute = p->tm_min;
	lpSystemTime->wSecond = p->tm_sec;
	lpSystemTime->wMilliseconds = 0;

}

inline DWORD GetLastError(void)
{
	return 0;
}

inline void SetLastError(DWORD dwErrCode)
{

}

inline void PostQuitMessage(int nExitCode)
{
	exit(nExitCode);
}

inline void ZeroMemory(void* Destination, size_t Length)
{
	memset(Destination, 0 , Length);
}


//	[2009-11-19]
#ifdef _UNICODE

	typedef wchar_t TCHAR;
	typedef const TCHAR* LPCTSTR;

	#define _T(quote) L##quote 
	#define TEXT(quote) L##quote 
    
#ifndef IOS_DEV

	#define swprintf   swprintf_linux
    #define wsprintf   swprintf_linux
    
#endif

	#define _tcscat     wcscat
	#define _tcschr     wcschr
	#define _tcscmp     wcscmp
	#define _tcscpy     wcscpy
	#define _tcscspn    wcscspn
	#define _tcslen     wcslen
	#define _tcsclen    wcslen

	#define _tcsncat    wcsncat
	#define _tcsnccat   wcsncat
	#define _tcsnccmp   wcsncmp
	#define _tcsncmp    wcsncmp
	#define _tcsncpy    wcsncpy
	#define _tcsnccpy   wcsncpy
	#define _tcspbrk    wcspbrk
	#define _tcsrchr    wcsrchr
	#define _tcsspn     wcsspn
	#define _tcsstr     wcsstr
	#define _tcstok     wcstok
	#define _tcsdup     _wcsdup
    #define _tcsicmp    wcscasecmp
	#define _tcsncicmp  _wcsnicmp
	#define _tcsnicmp   _wcsnicmp
	#define _tcsnset    _wcsnset
	#define _tcsncset   _wcsnset
	#define _tcsrev     _wcsrev
	#define _tcsset     _wcsset

	#define _tcslwr     _wcslwr
	#define _tcsupr     _wcsupr

	#define _tprintf    wprintf
	#define _ftprintf   fwprintf
#ifndef IOS_DEV
	#define _stprintf   swprintf
#else
    
#define _stprintf   swprintf_linux
#endif
    
	#define _sntprintf  _snwprintf
	#define _vtprintf   vwprintf
	#define _vftprintf  vfwprintf
	#define _vstprintf  vswprintf
	#define _vsntprintf _vsnwprintf
	#define _tscanf     wscanf
	#define _ftscanf    fwscanf
	#define _stscanf    swscanf
	#define  _tcsstr    wcsstr 
	#define _fgetts		fgetws

	#define _tfopen _wfopen

	#define _tcstod     wcstod
	#define _tcstol     wcstol
	#define _tcstoul    wcstoul

	#define _itot       _itow
	#define _ltot       _ltow
	#define _ultot      _ultow
	#define _ttoi       _wtoi
	#define _ttol       _wtol
	#define _ttoi64     _wtoi64
    #define _tstoi      _wtoi
    #define _tstof      _wtof

	#define IDOK                1
	#define IDCANCEL            2
	#define IDABORT             3
	#define IDRETRY             4
	#define IDIGNORE            5
	#define IDYES               6
	#define IDNO                7
	
	#define MB_OK                       0x00000000L
	#define MB_OKCANCEL                 0x00000001L
	#define MB_ABORTRETRYIGNORE         0x00000002L
	#define MB_YESNOCANCEL              0x00000003L
	#define MB_YESNO                    0x00000004L

	inline int MessageBox(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
	{
		printf ("MessageBox: %ls %ls\n", lpText, lpCaption);
		return 0;

	}

	inline BOOL CreateDirectory(LPCTSTR lpPathName, void* lpSecurityAttributes)
	{
		printf ("CreateDirectory undefined: %ls\n", lpPathName);
		return FALSE;
	}
	
#else
	#define _ttoi 		atoi
	#define _ttof 		atof
	#define _tcsstr		strstr 
	#define _fgetts		fgets
	#define  _tscanf	scanf
	#define _stscanf	sscanf
	#define _ftprintf   fprintf
	#define _ttol       atol
	#define _vsntprintf _vsnprintf
	#define _tcsclen    strlen
typedef char TCHAR;
typedef const TCHAR* LPCTSTR;
//Add by nilesliu  2009-07-01
#define LINUX 1

#define _T(x) x
#define TEXT(x) x

#define _tcsncpy strncpy
#define _stprintf sprintf
#define _tcscpy	 strcpy

#define _tcscat strcat
#define _tfopen fopen
#define _tcslen strlen
#define _tcscmp strcmp

#define _itot _itoa

inline char* strlwr(char* string){
	 char *cp;
     for (cp=string; *cp; ++cp)
     {
        if ('A' <= *cp && *cp <= 'Z')
        *cp += 'a' - 'A';
     }
     return (string);
}

#define _tcslwr strlwr

inline char* strupr(char* string){
	 char *cp;       /* traverses string for C locale conversion */
     for (cp = string; *cp; ++cp)
     {
        if ('a' <= *cp && *cp <= 'z')
        *cp += 'A' - 'a';
     }
     return(string);
}

#define _tcsupr strupr

#define _tcsncmp    strncmp

#define _tcsdup     strdup

#define _wtoi atoi

// extern char* _BACKGROUND_FILE_[];
#endif //_UNICODE

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;
typedef RGBQUAD * LPRGBQUAD;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER, *PBITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO,  *LPBITMAPINFO, *PBITMAPINFO;

typedef void * HBITMAP;
typedef void * HPEN;
typedef void * HDC;

typedef struct _BLENDFUNCTION
{
    BYTE   BlendOp;
    BYTE   BlendFlags;
    BYTE   SourceConstantAlpha;
    BYTE   AlphaFormat;
}BLENDFUNCTION,*PBLENDFUNCTION;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT;



#define   __stdcall

//	for android

#define CREATE_SUSPENDED 4

#define THREAD_BASE_PRIORITY_LOWRT  15U  // value that gets a thread to LowRealtime-1
#define THREAD_BASE_PRIORITY_MAX    2   // maximum thread base priority boost
#define THREAD_BASE_PRIORITY_MIN    (-2)  // minimum thread base priority boost
#define THREAD_BASE_PRIORITY_IDLE   (-15) // value that gets a thread to idle

#define THREAD_PRIORITY_NORMAL          50
#define THREAD_PRIORITY_HIGHEST         THREAD_BASE_PRIORITY_MAX
#define THREAD_PRIORITY_ABOVE_NORMAL    (THREAD_PRIORITY_HIGHEST-1)
#define THREAD_PRIORITY_ERROR_RETURN    (MAXLONG)


// #define TRACER(exp,...)	((void)0)

#ifdef ANDROID_NDK
inline int wcscasecmp(const wchar_t *s1, const wchar_t *s2)
{
	while (*s1 != '\0' && towlower(*s1) == towlower(*s2))
	{
	  s1++;
	  s2++;
	}

  return towlower(*s1) - towlower(*s2);
}
#endif

//Add by nilesliu 2009-07-01
// typedef long long __int64;

#endif // KOTEI_LINUX

#endif /*_LINUXTCHAR_H__*/
