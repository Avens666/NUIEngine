//  **************************************
//  File:        KFile.cpp
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:
//  Comments:
//  Rev:         1
//  Created:     2017/4/12
//  Last edit:   2017/4/12
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KFile.cpp
* 摘    要：
*
* 版    本：1.0
* 创建日期：26-Mar-2010 16:42:39
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/
#include <stdio.h>

#include "KFile.h"
//#include "KTime.h"

#define REVERSE2(x) ((((x)&0xff00)>>8)|(((x)&0x00FF)<<8))
#define REVERSE4(x) ((((x)&0xff000000)>>24)|(((x)&0x00ff0000)>>8)|(((x)&0x0000ff00)<<8)|(((x)&0x000000ff)<<24))

//#ifndef QT_WINDOWS_LIB
extern int MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...);
//#endif

//	得到 last error code 对应的信息 
void GetLastErrorMsg (kn_string& strMessage)
{
#ifdef WIN32
	kn_dword dwError = GetLastError();

	LPTSTR lpMsgBuf = NULL;
	kn_char wszTemp[256] = {0};

	BOOL r = FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		0, // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL 
		);

	if(r == 0)
	{
		swprintf (wszTemp , L"last_error = %d", dwError);
		return;
	}



	swprintf (wszTemp , L"last_error = %d, err_message = \n%s", dwError, lpMsgBuf);

	// Free the buffer.
	LocalFree(lpMsgBuf);

	strMessage = wszTemp;

#endif	//	WIN32
}


KFile::~KFile()
{

}

/**
 * 构造函数
 */
KFile::KFile()
{
    m_ulSize = 0;
    m_hFile = NULL;
    m_lCursor = 0;
    m_bIsOpen = false;


	m_lPostion = 0;
	memset(m_szMode, 0, sizeof(m_szMode));
}

/**
 * 打开文件 参考API:CreateFile()
 * 
 * @param strFileName    文件名
 * @param dwDesiredAccess
 * @param dwShareMode    打开模式(share/exclusive)
 * @param dwCreationDisposition
 * @param dwFlagsAndAttributes    
 */
KNRESULT KFile::Open(IN const kn_string& strFileName, IN kn_dword dwDesiredAccess, IN kn_dword dwShareMode, IN kn_dword dwCreationDisposition, IN kn_dword dwFlagsAndAttributes)
{
#ifdef LINUX
    ConvertSlash(const_cast<kn_char*>(strFileName.c_str()));
#endif

#ifdef QT_WINDOWS_LIB
    LOGI("strFileName = %ls", strFileName.c_str());
#else
    LOGI("strFileName = %s", strFileName.c_str());
#endif

    if (m_bIsOpen)
    {
    	LOGI("KFile::Open  error ---------");
        return KNRESULT(EC_ERROR, SC_BASELIB, EF_FILE, KN_ERROR_OPEN_REPEAT);	// 已经有文件被打开,再次打开文件失败
    }

    kn_char szMode[5] = {0};
    int nMode = 0;

    // determine read/write mode depending on CFile mode
    if ((dwDesiredAccess & GENERIC_WRITE) && (dwCreationDisposition & CREATE_ALWAYS))
    {
        szMode[nMode++] = 'w';
    }
    else
    {
        szMode[nMode++] = 'r';
    }

    // add '+' if necessary (when read/write modes mismatched)
    if ((szMode[0] == 'r') && (dwDesiredAccess & GENERIC_WRITE))
    {
        // current szMode mismatched, need to add '+' to fix
        szMode[nMode++] = '+';
    }

    szMode[nMode++] = _T('b');
    szMode[nMode++] = _T('\0');

    // 打开文件
    _tcscat(szMode, _T("c"));
    m_hFile = _tfopen(strFileName.c_str(), szMode);

	memset(m_szMode, 0, sizeof(m_szMode));
	_tcsncpy (m_szMode, szMode, _tcslen(szMode));

	m_strRealFileName = strFileName;

    // CreateFile()调用失败会自动抛出异常
    if (m_hFile == NULL)
    {
        m_hFile = NULL;

        LOGI("KFile::Open  error ---------");
        return KNRESULT(EC_ERROR, SC_BASELIB, EF_FILE, KN_ERROR_OPEN_FAILED);
    }
    else
    {
        m_strName = strFileName;
        m_bIsOpen = true;
		m_lPostion = ftell(m_hFile);

        LOGI("KFile::Open  OK ---------");
        return RESULT_OK;
    }
}


/**
 * 关闭文件
 */
KNRESULT KFile::Close()
{
    if (m_bIsOpen)
    {
        fclose(m_hFile);
        m_hFile = NULL;
        m_bIsOpen = false;
        m_lCursor = 0;

		m_lPostion = 0;


        return RESULT_OK;
    }

    return RESULT_UNKNOW;
}


/**
 * 获取文件句柄
 */
const KNFILEHANDLE& KFile::GetHandle() const 
{
	return m_hFile;
}


/**
 * 函数功能：取得文件大小
 * 
 * @param ulSize    ulSize
 */
KNRESULT KFile::GetSize(IN kn_ulong& ulSize)
{
    ASSERT(m_hFile != NULL);		
    ASSERT(m_hFile != INVALID_HANDLE_VALUE);

    if (m_bIsOpen)
    {
        // 保存当前位置
        kn_long lOldPos = ftell(m_hFile);
        if (lOldPos == -1L)
        {
            return RESULT_UNKNOW;
        }

        // 移至文件尾
        if (fseek(m_hFile, 0, SEEK_END) != 0)
        {
            return RESULT_UNKNOW;
        }

        // 取得前位置
        kn_long lPos = ftell(m_hFile);
        if (lPos == -1L)
        {
            return RESULT_UNKNOW;
        }

        // 恢复先前位置
        if (fseek(m_hFile, lOldPos, SEEK_SET) != 0)
        {
            return RESULT_UNKNOW;
        }

        ulSize = lPos;

        return RESULT_OK;
    }

    return RESULT_UNKNOW;
}


/**
 * 函数功能：取得当前文件指针的位置
 */
kn_long KFile::Tell()
{
    if (m_bIsOpen)
    {
        return m_lCursor;
    }
    else
    {
        ASSERT(0);

        return 0;
    }
}


/**
 * 函数功能：移动文件句柄到指定位置
 * 
 * @param lOffset    移动大小
 * @param dwMoveMethod    0-文件头  1-当前位置  2-文件尾
 */
KNRESULT KFile::Seek(IN kn_long lOffset, IN kn_dword dwMoveMethod)
{
    ASSERT(m_hFile != NULL);		
    ASSERT(m_hFile != INVALID_HANDLE_VALUE);

	if (!m_bIsOpen)
	{
		return RESULT_UNKNOW;	// 文件还没有打开
	}

    kn_long lCursor = 0;
    if (dwMoveMethod == SEEK_SET)		// 文件开始
    {
        lCursor = lOffset;
    }
    else if (dwMoveMethod == SEEK_CUR)	// 当前位置
    {
        lCursor = m_lCursor + lOffset;
    }
    else if (dwMoveMethod == SEEK_END)	// 文件末尾
    {
        lCursor = m_ulSize + lOffset;
    }
    else
    {
        ASSERT(true);
    }

	KNRESULT lRet = RESULT_UNKNOW;

	if (fseek(m_hFile, lOffset, dwMoveMethod) == 0)
	{
		lRet = RESULT_OK;
	}
	else
	{
		//	再次打开
		if (TryReOpenFile() == 0 && fseek(m_hFile, lOffset, dwMoveMethod) == 0)
		{
			lRet = RESULT_OK;
		}
	}


	if (lRet == RESULT_OK)
	{
		m_lCursor = lCursor;
		m_lPostion = ftell(m_hFile);
	}
	else
	{
		kn_string strErrMsg;
		GetLastErrorMsg(strErrMsg);

		//	程序不能读取文件，无法继续运行
#if defined(ANDROID_NDK) || defined(LINUX) || defined(QT_WINDOWS_LIB)
		LOGE("fseek error : %s, m_hFile = %X, pos = %d, lOffset = %d, dwMoveMethod = %d, ferror = %d，feof = %d, %s",
			 m_strName.c_str(), (DWORD)m_hFile, m_lPostion, lOffset, dwMoveMethod, ferror(m_hFile), feof(m_hFile), strErrMsg.c_str());
#elif !defined(IOS_DEV)
		//MessageBoxPrintf (_T("fseek error"), _T("%s, m_hFile = %X, pos = %d, lOffset = %d, dwMoveMethod = %d, ferror = %d，feof = %d, %s"), 
		//				  m_strName.c_str(), (DWORD)m_hFile, m_lPostion, lOffset, dwMoveMethod, ferror(m_hFile), feof(m_hFile), strErrMsg.c_str());
#endif
	}


	return lRet;


    //if (m_bIsOpen)
    //{
    //    // 确定m_lCursor位置
    //    kn_long lCursor = 0;
    //    if (dwMoveMethod == SEEK_SET)		// 文件开始
    //    {
    //        lCursor = lOffset;
    //    }
    //    else if (dwMoveMethod == SEEK_CUR)	// 当前位置
    //    {
    //        lCursor = m_lCursor + lOffset;
    //    }
    //    else if (dwMoveMethod == SEEK_END)	// 文件末尾
    //    {
    //        lCursor = m_ulSize + lOffset;
    //    }
    //    else
    //    {
    //        ASSERT(true);
    //    }
    //    m_lCursor = lCursor;

    //    if (fseek(m_hFile, lOffset, dwMoveMethod) != 0)
    //    {
    //        return RESULT_UNKNOW;
    //    }

    //    return RESULT_OK;
    //}

    //return RESULT_UNKNOW;	// 文件还没有打开
}


/**
 * 函数功能：刷新缓存
 */
KNRESULT KFile::Flush()
{
	return (fflush(m_hFile) == 0) ? RESULT_OK : RESULT_UNKNOW;
}


/**
 * 是否打开
 */
kn_bool KFile::IsOpen() const 
{
    return m_bIsOpen;
}


/**
 * 是否存在
 * 
 * @param strFileName
 * @param FileAtt    FileAtt
 */
kn_bool KFile::IsExist(IN string strFileName)
{
	return  true;
}


/**
 * 读kn_byte
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_byte& value)
{
    return Read(&value, sizeof(value));
}

/**
* 读char
* 
* @param value    存储对象
*/
/*KNRESULT KFile::Read(IN char& value)
{
    return Read(&value, sizeof(value));
}*/

/**
* 读kn_char
* 
* @param value    存储对象
*/
KNRESULT KFile::Read(IN kn_char& value)
{
    return Read(&value, sizeof(value));
}

/**
 * 读kn_word
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_word& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 读kn_dword
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_dword& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 读kn_short
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_short& value)
{
    return Read(&value, sizeof(value));
}

/**
 * 读kn_int
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_int& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 读kn_long
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_long& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 读kn_float
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_float& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 读kn_double
 * 
 * @param value    存储对象
 */
KNRESULT KFile::Read(IN kn_double& value)
{
    return Read(&value, sizeof(value));
}


/**
 * 从当前位置读取一块内存
 * 
 * @param pBuff    已经申请的内存
 * @param ulSize    读取的数据块的大小
 */
KNRESULT KFile::Read(IN void* pBuff, IN const kn_ulong ulSize)
{
    kn_dword dwNumOfBytesRead = 0;

    return Read(pBuff, ulSize, dwNumOfBytesRead);
}


/**
 * 
 * @param pBuff
 * @param ulSizeToRead
 * @param ulSizeReturn    ulSizeReturn
 */
KNRESULT KFile::Read(IN void* pBuff, IN const kn_ulong ulSizeToRead, IN kn_ulong& ulSizeReturn)
{
    ASSERT((m_hFile != NULL) && (m_hFile != INVALID_HANDLE_VALUE));	

	KNRESULT lRet = RESULT_UNKNOW;

	long lSize = ftell(m_hFile);
	if (lSize == -1)
	{
		ASSERT(0);
	}

	ulSizeReturn = fread(pBuff, sizeof(char), ulSizeToRead, m_hFile);

	if ((ulSizeReturn == ulSizeToRead) && !ferror(m_hFile))
	{
		// 未到文件尾 OK
		lRet = RESULT_OK;
	}
	else if ((ulSizeReturn < ulSizeToRead) && !ferror(m_hFile) && feof(m_hFile))
	{
		// 到文件尾 OK
		lRet = RESULT_OK;
	}
	else
	{
		//	处理SD卡短暂松动的情况,重新打开文件 [2010-4-7]
		//	ERROR_CODE = 1106 文件所在的卷已被外部改变，因此打开的文件不再有效。 
		if (TryReOpenFile() == 0)
		{
			//	成功地再次打开

			clearerr(m_hFile);

			ulSizeReturn = fread(pBuff, sizeof(char), ulSizeToRead, m_hFile);

			if (ferror(m_hFile) || ((ulSizeReturn != ulSizeToRead) && !feof(m_hFile)))
			{
				//	error

			}
			else
			{
				//	OK
				lRet = RESULT_OK;

			}
		}


	}

	if(lRet == RESULT_UNKNOW)
	{

		kn_string strErrMsg;
		GetLastErrorMsg(strErrMsg);

	//	程序不能读取文件，无法继续运行
#if !defined(ANDROID_NDK) && !defined(IOS_DEV) && !defined(LINUX) && !defined(QT_WINDOWS_LIB)
		//MessageBoxPrintf (_T("fread error"), _T("%s, pBuff = %p, pos = %d, read = %d, alloc = %d, ferror = %d, feof = %d, %s"), 
		//	m_strName.c_str(), pBuff, m_lPostion, ulSizeReturn, ulSizeToRead, ferror(m_hFile), feof(m_hFile), strErrMsg.c_str());
#else
		LOGE("fseek error : %s, pBuff = %p, pos = %d, read = %d, alloc = %d, ferror = %d, feof = %d, %s",
				m_strName.c_str(), pBuff, m_lPostion, ulSizeReturn, ulSizeToRead, ferror(m_hFile), feof(m_hFile), strErrMsg.c_str());
#endif
	}
	else
	{
		//	OK
		//m_lCursor = ftell(m_hFile);
		m_lPostion = ftell(m_hFile);

		m_lCursor += ulSizeReturn;

	}


	return lRet;

}

//	尝试重新打开文件，恢复文件读取位置
kn_long KFile::TryReOpenFile()
{
	kn_long lRet = -1;

	const int MAX_COUNT = 20;	//	最大重试次数
	int iCount = MAX_COUNT;

	//	fopen 打开方式
	if (m_szMode[0] == 0)
	{
		_tcsncpy(m_szMode, _T("rb"), 2);
	}

	while(iCount > 0)
	{

		if (iCount != MAX_COUNT)
		{
			Sleep(500);	//	间隔0.3秒
		}

		iCount--;

		fclose(m_hFile);

		m_hFile = _tfopen(m_strRealFileName.c_str(), m_szMode);

		if (m_hFile == NULL)
		{
			continue;
		}

		if (fseek(m_hFile, m_lPostion, SEEK_SET) != 0)
		{
			continue;
		}

		lRet = 0;
		break;

	}

	return lRet;

}



/**
 * 反序读kn_word
 * 
 * @param value    存储对象
 */
KNRESULT KFile::ReadReverse(IN kn_word& value)
{
    KNRESULT lRet = Read(value);
    value = REVERSE2(value);

    return lRet;
}


/**
 * 反序读kn_dword
 * 
 * @param value    存储对象
 */
KNRESULT KFile::ReadReverse(IN kn_dword& value)
{
    KNRESULT lRet = Read(value);
    value = REVERSE4(value);

    return lRet;
}


/**
 * 写kn_byte
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_byte& value)
{
    return Write(&value, sizeof(value));
}


/**
* 写char
* 
* @param value    写入对象
*/
/*KNRESULT KFile::Write(IN char& value)
{
    return Write(&value, sizeof(value));
}*/


/**
* 写kn_char
* 
* @param value    写入对象
*/
KNRESULT KFile::Write(IN kn_char& value)
{
    return Write(&value, sizeof(value));
}


/**
 * 写kn_word
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_word& value)
{
    return Write(&value, sizeof(value));
}


/**
 * 写kn_dword
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_dword& value)
{
   return Write(&value, sizeof(value));
}


/**
 * 写kn_short
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_short& value)
{
    return Write(&value, sizeof(value));
}


/**
 * 写kn_int
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_int& value)
{
	return Write(&value, sizeof(value));
}


/**
 * 写kn_long
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_long& value)
{
    return Write(&value, sizeof(value));
}


/**
 * 写kn_float
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_float& value)
{
    return Write(&value, sizeof(value));
}


/**
 * 写kn_double
 * 
 * @param value    写入对象
 */
KNRESULT KFile::Write(IN kn_double& value)
{
	return  Write(&value, sizeof(value));
}


/**
 * 写一块内存
 * 
 * @param pBuff    写入缓冲区
 * @param ulSize    写入大小
 */
KNRESULT KFile::Write(IN const void* pBuff, IN const kn_ulong ulSize)
{
    ASSERT((m_hFile != NULL) && (m_hFile != INVALID_HANDLE_VALUE));	

    if (fwrite(pBuff, sizeof(char), ulSize, m_hFile) < ulSize)
    {
        m_lCursor += ulSize;

        return KNRESULT(EC_ERROR, SC_BASELIB, EF_FILE, KN_ERROR_WRITE_FAIL);
    }

    m_lCursor += ulSize;

    return RESULT_OK;
}

// 重载>>运算符
KFile& KFile::operator >> (kn_bool &value)
{
    Read(&value, sizeof(value));

    return *this;
}

/*KFile& KFile::operator >> (char &value)
{
    Read(&value, sizeof(value));

    return *this;
}*/

KFile& KFile::operator >> (kn_byte &value)
{
    Read(&value, sizeof(value)) ;

    return *this;
}

KFile& KFile::operator >> (kn_char &value)
{
    Read(&value, sizeof(value)) ;

    return *this;
}

KFile& KFile::operator >> (kn_short &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_ushort &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_int &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_uint &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_long &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_ulong &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_float &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (kn_double &value)
{
    Read(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator >> (string& str)
{
    kn_long len;  // 字符串长度
    Read(&len, sizeof(len));

    char *pBuff = new char[len + 1];
    if(pBuff)
    {
        memset(pBuff, 0, (len + 1) * sizeof(char));
        Read(pBuff, len * sizeof(char));
        str = pBuff;
        SAFE_DELETE_GROUP(pBuff);
    }

    return *this;
}

KFile& KFile::operator >> (wstring& wstr)
{
    kn_long len;  // 字符串长度
    Read(&len, sizeof(len));

    wchar_t *pBuff = new wchar_t[len + 1];
    if(pBuff)
    {
        memset(pBuff, 0, (len + 1) * sizeof(wchar_t));
        Read(pBuff, len * sizeof(wchar_t));
        wstr = pBuff;
        SAFE_DELETE_GROUP(pBuff);
    }

    return *this;
}


KFile& KFile::operator >> (char *&value) // 警惕越界访问，如出现越界访问，处理方案：1. 使用string; 2. 事先分配足够空间给value；
{
    kn_long len;  // 字符串长度
    Read(&len, sizeof(len));

    ASSERT(strlen(value) >= static_cast<size_t>(len));

    Read(value, len * sizeof(char));

    return *this;
}

KFile& KFile::operator >> (wchar_t *&value)  // 警惕越界访问，如出现越界访问，处理方案：1. 使用wstring; 2. 事先分配足够空间给value；
{		
    kn_long len;  // 字符串长度
    Read(&len, sizeof(len));

    ASSERT(wcslen(value) >= static_cast<size_t>(len));

    Read(value, len * sizeof(wchar_t));

    return *this;
}

// 重载<<运算符
KFile& KFile::operator << (const kn_bool value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_byte value)
{
    Write(&value, sizeof(value));

    return *this;
}
/*KFile& KFile::operator << (const char value)
{
    Write(&value, sizeof(value));

    return *this;
}*/

KFile& KFile::operator << (const kn_char value)
{
    Write(&value, sizeof(value));

    return *this; 
}

KFile& KFile::operator << (const kn_short value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_ushort value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_int value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_uint value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_ulong value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_long value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_float value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const kn_double value)
{
    Write(&value, sizeof(value));

    return *this;
}

KFile& KFile::operator << (const string& str)
{
    *this << str.c_str();

    return *this;
}

KFile& KFile::operator << (const wstring& wstr)
{
    *this << wstr.c_str();

    return *this;
}

KFile& KFile::operator << (const char *value)
{
    kn_long len = strlen(value); 
    Write(&len, sizeof(kn_long));
    Write(value, len * sizeof(char));

    return *this;
}

KFile& KFile::operator << (const wchar_t *value)
{
    kn_long len = wcslen(value); 
    Write(&len, sizeof(kn_long));
    Write(value, len * sizeof(wchar_t));

    return *this;
}
