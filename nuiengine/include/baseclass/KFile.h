//  **************************************
//  File:        KFile.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:
//  Comments:
//  Rev:         1
//  Created:     2017/4/12
//  Last edit:   2017/4/13
//  Author:      Chen Zhi
//  E-mail:      cz_666@qq.com
//
//  License: APACHE V2.0 (see license file) 
/**
* Copyright (C) 2003-2010，光庭导航数据有限公司(http://www.kotei-navi.com)
* All rights reserved.
* 
* 文件名称：KFile.h
* 摘    要：
*
* 版    本：1.0
* 创建日期：26-Mar-2010 16:42:38
* 作    者：zhifangh
*
* 修改历史：
* [修改序列][修改日期][修改者][修改内容]
**/

#if !defined(EA_500EDE1E_3489_444e_8C79_0F9A07CDB0FC__INCLUDED_)
#define EA_500EDE1E_3489_444e_8C79_0F9A07CDB0FC__INCLUDED_


#include "NE_pubhead.h"
#include "CharSet.h"
#include "NE_error.h"

typedef FILE* KNFILEHANDLE;

/**
 * 文件类
 * @author zhifangh
 * @version 1.0
 * @created 26-Mar-2010 16:42:38
 */
class NUI_API KFile
{

public:

	/**
	 * 构造函数
	 */
	KFile();
	/**
	 * 析构函数
	 */
    virtual ~KFile();

	/**
	 * 打开文件 参考API:CreateFile()
	 * 
	 * @param strFileName    文件名
	 * @param dwDesiredAccess
	 * @param dwShareMode    打开模式(share/exclusive)
	 * @param dwCreationDisposition
	 * @param dwFlagsAndAttributes 
	 */
	virtual KNRESULT Open(IN const kn_string& strFileName, IN kn_dword dwDesiredAccess = GENERIC_READ, IN kn_dword dwShareMode = FILE_SHARE_READ, IN kn_dword dwCreationDisposition = OPEN_EXISTING, IN kn_dword dwFlagsAndAttributes = FILE_ATTRIBUTE_READONLY);
	/**
	 * 关闭文件
	 */
	virtual KNRESULT Close();
	/**
	 * 获取文件句柄
	 */
	virtual const KNFILEHANDLE& GetHandle() const;
	/**
	 * 函数功能：取得文件大小
	 * 
	 * @param ulSize    ulSize
	 */
	virtual KNRESULT GetSize(IN kn_ulong& ulSize);
	/**
	 * 函数功能：取得当前文件指针的位置
	 */
	virtual kn_long Tell();
	/**
	 * 函数功能：移动文件句柄到指定位置
	 * 
	 * @param lOffset    移动大小
	 * @param dwMoveMethod    SEEK_SET-文件头  SEEK_CUR-当前位置  SEEK_END-文件尾
	 */
	virtual KNRESULT Seek(IN kn_long lOffset, IN kn_dword dwMoveMethod);
	/**
	 * 函数功能：刷新缓存
	 */
	virtual KNRESULT Flush();
	/**
	 * 是否打开
	 */
	kn_bool IsOpen() const;
	/**
	 * 是否存在
	 * 
	 * @param strFileName
	 */
	kn_bool IsExist(IN string strFileName);
	/**
	 * 读kn_byte
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_byte& value);
	/**
	 * 读char
	 * 
	 * @param value    存储对象
	 */
	//virtual KNRESULT Read(IN char& value);
	/**
	 * 读kn_char
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_char& value);
	/**
	 * 读kn_word
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_word& value);
	/**
	 * 读kn_dword
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_dword& value);
	/**
	 * 读kn_short
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_short& value);
	/**
	 * 读kn_int
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_int& value);
	/**
	 * 读kn_long
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_long& value);
	/**
	 * 读kn_float
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_float& value);
	/**
	 * 读kn_double
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT Read(IN kn_double& value);
	/**
	 * 从当前位置读取一块内存
	 * 
	 * @param pBuff    已经申请的内存
	 * @param ulSize    读取的数据块的大小
	 */
	virtual KNRESULT Read(IN void* pBuff, IN const kn_ulong ulSize);
	/**
	 * 
	 * @param pBuff
	 * @param ulSizeToRead
	 * @param ulSizeReturn    ulSizeReturn
	 */
	virtual KNRESULT Read(IN void* pBuff, IN const kn_ulong ulSizeToRead, IN kn_ulong& ulSizeReturn);
	/**
	 * 反序读kn_word
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT ReadReverse(IN kn_word& value);
	/**
	 * 反序读kn_dword
	 * 
	 * @param value    存储对象
	 */
	virtual KNRESULT ReadReverse(IN kn_dword& value);
	/**
	 * 写kn_byte
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_byte& value);
	/**
	 * 写char
	 * 
	 * @param value    写入对象
	 */
	//virtual KNRESULT Write(IN char& value);
	/**
	 * 写kn_char
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_char& value);
	/**
	 * 写kn_word
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_word& value);
	/**
	 * 写kn_dword
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_dword& value);
	/**
	 * 写kn_short
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_short& value);
	/**
	 * 写kn_int
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_int& value);
	/**
	 * 写kn_long
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_long& value);
	/**
	 * 写kn_float
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_float& value);
	/**
	 * 写kn_double
	 * 
	 * @param value    写入对象
	 */
	virtual KNRESULT Write(IN kn_double& value);
	/**
	 * 写一块内存
	 * 
	 * @param pBuff    写入缓冲区
	 * @param ulSize    写入大小
	 */
	virtual KNRESULT Write(IN const void* pBuff, IN const kn_ulong ulSize);

    // 重载>>运算符
    KFile& operator >> (kn_bool &value);
    KFile& operator >> (kn_byte &value);
    //KFile& operator >> (char &value);
    KFile& operator >> (kn_char &value);
    KFile& operator >> (kn_short &value);
    KFile& operator >> (kn_ushort &value);
    KFile& operator >> (kn_int &value);
    KFile& operator >> (kn_uint &value);
    KFile& operator >> (kn_long &value);
    KFile& operator >> (kn_ulong &value);
    KFile& operator >> (kn_float &value);
    KFile& operator >> (kn_double &value);
    KFile& operator >> (string& str);
    KFile& operator >> (wstring& wstr);
    KFile& operator >> (char *&value);
    KFile& operator >> (wchar_t *&value);

    // 重载<<运算符
    KFile& operator << (const kn_bool value);
    KFile& operator << (const kn_byte value);
    //KFile& operator << (const char value);
    KFile& operator << (const kn_char value);
    KFile& operator << (const kn_short value);
    KFile& operator << (const kn_ushort value);
    KFile& operator << (const kn_int value);
    KFile& operator << (const kn_uint value);
    KFile& operator << (const kn_ulong value);
    KFile& operator << (const kn_long value);
    KFile& operator << (const kn_float value);
    KFile& operator << (const kn_double value);
    KFile& operator << (const string& str);
    KFile& operator << (const wstring& wstr);
    KFile& operator << (const char *value);
    KFile& operator << (const wchar_t *value);

protected:

	/**
	* 尝试重新打开文件，恢复文件读取位置
	*/
	virtual kn_long TryReOpenFile();

protected:
	/**
	 * 文件名
	 */
	kn_string m_strName;
	/**
	 * 文件大小
	 */
	kn_ulong m_ulSize;
	/**
	 * 文件句柄
	 */
	KNFILEHANDLE m_hFile;
	/**
	 * 当前位置
	 */
	kn_long m_lCursor;
	/**
	 * 是否打开
	 */
	kn_bool m_bIsOpen;

	/**
	* fopen mode
	*/	
	kn_char m_szMode[8];

	/**
	* ftell position
	*/		
	kn_long m_lPostion;

	/**
	* 打包文件的实际文件名,
	*/	
	kn_string m_strRealFileName;

};
#endif // !defined(EA_500EDE1E_3489_444e_8C79_0F9A07CDB0FC__INCLUDED_)
