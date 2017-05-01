//  **************************************
//  File:        KProfile.cpp
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


#include <string.h>
#include <stdarg.h>

#include "NE_pubhead.h"
#include "KProfile.h"

// ADD. hzf.2006.11.9.
// --Begin
//lint -e570 -e64 -e668 -e650
// --End

#define MAX_LINE_SIZE	(512)
#define STEP			(16)

/*
* 函数功能：构造函数
* 参    数：无
* 返 回 值：无
*/ 
KProfile::KProfile()
{
    m_ulMark = -1;
}

/*
* 函数功能：析构函数
* 参    数：无
* 返 回 值：无
*/ 
KProfile::~KProfile()
{
	m_File.Close();
}

/*
* 函数功能：打开配置文件
* 参    数：
* 		strFileName [in] ：文件路径
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::Open(const kn_string& strFilename)
{
    if ( m_File.IsOpen() )
	{
		m_File.Close();
	}

//#ifdef _UNICODE
//    m_pFile = _wfopen(strFilename.c_str(), _T("rb"));
//#else
//    m_pFile = fopen(strFilename.c_str(), _T("rb"));
//#endif

	m_File.Open(strFilename, GENERIC_READ, NULL, OPEN_EXISTING, NULL);
	if ( !m_File.IsOpen())
	{
		return -1;
	}

    return 0;
}

/*
* 函数功能：关闭文件
* 参    数：无
* 返 回 值：无
*/ 
void KProfile::Close(void)
{
    if (m_File.IsOpen()) 
	{
		m_File.Close();
	}
}

/*
* 函数功能：读取一行
* 参    数：
* 		pBuff [out] ：缓冲区指针
* 		nCount [in]：缓冲区大小
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetLine(char* pBuff, kn_int nCount)
{
    if (!m_File.IsOpen())
	{
		return -1;
	}

	kn_ulong fileSize = 0;
    m_File.GetSize(fileSize);
    if (static_cast<kn_ulong>(m_File.Tell()) >= fileSize) 
	{
		return -1; 
	}
    
    for (kn_int i = 0; i < nCount; i += STEP)
    {
        kn_ulong ret = 0;
		m_File.Read(&pBuff[i], sizeof(char)*STEP, ret);

        if ((ret == 0) && (i == 0)) 
		{
			return -1;
		}

        for (kn_int j = 0; j < (kn_int)ret; ++j)
        {
            if (pBuff[i+j] == '\n')
            {
                pBuff[i + j - 1 > 0 ? i + j -1 : 0] = 0;
                //fseek(m_pFile, -ret+j+1, SEEK_CUR);
				m_File.Seek(j + 1 - ret, 1);
                return 0;
            }
        }
    }

    return 0;
}

/*
* 函数功能：获取指定名称的对应字符串
* 参    数：
* 		pKey [in] ：指定的名称
* 		pBuff [out]：保存字符串缓冲区指针
*		nSize [in]：缓冲区大小
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetString(const char* pKey, char* pBuff, kn_int nSize)
{
    if (!pKey || !pBuff) 
	{
		return -1;
	}

    const kn_int nLen = strlen(pKey);

    char pchTemp[MAX_LINE_SIZE] = {0};

    while (GetLine(pchTemp, MAX_LINE_SIZE) != -1)
    {
        if (strnicmp2(pchTemp, pKey, nLen) == 0) // key
        {
            kn_int nOutLen = strlen(pchTemp)-nLen;
            if (nOutLen > nSize)
			{
				nOutLen = nSize;
			}
            
            strncpy(pBuff, &pchTemp[nLen+1], nOutLen);

            return 0;
        }
    }

    return -1;
}
    
/*
* 函数功能：比较两个字符串是否相同 不区分大小写
* 参    数：
* 		pStr1 [in] ：字符串1
* 		pStr2 [in]：字符串2
* 		nCount [in]：比较长度
* 返 回 值：相等返回0，否则返回-1
*/ 
kn_int KProfile::strnicmp2(const char* pStr1, const char* pStr2, kn_int nCount) const
{
    for (kn_int i = 0; i < nCount; ++i)
    {
        if (pStr1[i] != pStr2[i] && 
            pStr1[i]+0x20 != pStr2[i] && 
            pStr1[i] != pStr2[i]+0x20)
        {
            return -1;
        }
    }

    return 0;
}

/*
* 函数功能：读取指定名称的整型值
* 参    数：
* 		pKey [in] ：要读取的名称
* 		nValue [out]：返回的值
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetValue(const char* pKey, kn_int& nValue)
{
    char buff[MAX_LINE_SIZE] = {0};

    if (GetString(pKey, buff, MAX_LINE_SIZE) == -1) return -1;

    nValue = atoi(buff);

    return 0;    
}

/*
* 函数功能：读取指定名称的整型值
* 参    数：
* 		pKey [in] ：要读取的名称
* 		nValue [out]：返回的值
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetValue(const char* pKey, kn_ulong& nValue)
{
    char buff[MAX_LINE_SIZE] = {0};

    if (GetString(pKey, buff, MAX_LINE_SIZE) == -1) return -1;

    nValue = atol(buff);

    return 0;    
}

/*
* 函数功能：读取指定名称的双精度值
* 参    数：
* 		pKey [in] ：要读取的名称
* 		dValue [out]：返回的值
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetValue(const char* pKey, kn_double& dValue)
{
    char chBuff[MAX_LINE_SIZE] = {0};

    if (GetString(pKey, chBuff, MAX_LINE_SIZE) == -1)
	{
		return -1;
	}

    dValue = atof(chBuff);

    return 0;  
}

/*
* 函数功能：复位文件指针到文件头
* 参    数：无
* 返 回 值：返回0
*/ 
kn_int KProfile::Reset()
{
	// ADD. hzf.2006.11.9.
	// --Begin
	ASSERT(m_File.IsOpen());
	// --End
	
    //fseek(m_pFile, 0, SEEK_SET);
	m_File.Seek(0, 0);
    return 0;
}

/*
* 函数功能：保存文件当前位置
* 参    数：无
* 返 回 值：返回0
*/ 
kn_int KProfile::Mark()
{
	// ADD. hzf.2006.11.9.
	// --Begin
	ASSERT(m_File.IsOpen());
	// --End
	
    m_ulMark = m_File.Tell();
    return 0;
}

/*
* 函数功能：返回保存的文件位置
* 参    数：无
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::Back()
{
	// ADD. hzf.2006.11.9.
	// --Begin
	ASSERT(m_File.IsOpen());
	// --End
	
    if (m_ulMark != -1)
    {
        //fseek(m_pFile, m_uiMark, SEEK_SET);
		m_File.Seek(m_ulMark, 0);
        return 0;
    }
    
    return -1;
}

/*
* 函数功能：取得Section名称
* 参    数：
* 		pKey [out] ：保存名称
* 返 回 值：成功返回0，失败返回-1
*/ 
kn_int KProfile::GetSection(const char* pKey)
{
	// ADD. hzf.2006.11.9.
	// --Begin
	ASSERT(m_File.IsOpen());
	// --End

    if (NULL == pKey)
	{
		return -1;
	}

    //fseek(m_pFile, 0, SEEK_SET);
	Reset();

    const kn_int nLen = strlen(pKey);

    char pchTemp[MAX_LINE_SIZE] = {0};

    while(GetLine(pchTemp, MAX_LINE_SIZE) != -1)
    {
        // 此处可能会break
        if (pchTemp[0] == '[' && pchTemp[nLen+1] == ']')
        {
            if (strnicmp2(&pchTemp[1], pKey, nLen) == 0)
            {
                return 0;
            }
        }
    } 

    return -1;
}

/*
* 函数功能：拆分字符串
* 参    数：
* 		pBuff [in] ：源字符串
* 返 回 值：无
*/ 
void KProfile::Split(const char* pBuff, ...)
{    
    va_list marker;
    va_start(marker, pBuff);
    
    kn_int begin = 0;

    char* pVar = va_arg(marker, char*);

    for (kn_int i = 0; i <= (kn_int)strlen(pBuff); ++i)
    {
        if (pVar == NULL) 
			break;

        if (pBuff[i] == ',' || pBuff[i] == 0)
        {
            memcpy(pVar, &pBuff[begin], i-begin);
            pVar[i-begin] = 0;
            begin = i+1;

            pVar = va_arg(marker, char*);
        }
    }
    
    va_end(marker);
}

// ADD. hzf.2006.11.9.
// --Begin
//lint +e570 +e64 +e668 +e650
// --End
