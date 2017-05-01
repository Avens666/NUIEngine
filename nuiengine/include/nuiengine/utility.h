//  **************************************
//  File:        utility.h
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
#ifndef utility_h__
#define utility_h__

#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
//#include <tchar.h>
#include <functional>
#include <boost/token_iterator.hpp>
#include <boost/tokenizer.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "CharSet.h"

//以下错误类型定义与WINERROR.H一致，为保证跨平台移植至此
#ifndef _WINERROR_
#define NO_ERROR                         0L      // dderror
#define ERROR_FILE_NOT_FOUND             2L   
#define ERROR_PATH_NOT_FOUND             3L
#define ERROR_INVALID_PARAMETER          87L    // dderror
#define ERROR_FILE_INVALID               1006L  
#define ERROR_FUNCTION_FAILED            1627L  
#define ERROR_CREATE_FAILED              1631L
#endif

//
using std::vector;
using std::string;
using std::wstring;

#ifdef _UNICODE
typedef wchar_t kn_char;
#else
typedef char kn_char;
#endif // _UNICODE
typedef std::basic_string <kn_char> kn_string;

#ifdef WIN32
 #undef MultiByteToWideChar   //为了能调用系统的MultiByteToWideChar函数
 #undef WideCharToMultiByte   //为了能调用系统的MultiByteToWideChar函数
#endif // WIN32

wstring UTF8ToUnicode( const string& str );

string UnicodeToUTF8( const wstring& str );

//string转换kn_string
kn_string s2ns( const std::string& ansiStr );

//kn_string转换string
string ns2s( const kn_string& str );

std::wstring s2ws( const std::string& ansiStr);

//ptime转换到string 2013-08-07 13:27:18
string ptime2str(const boost::posix_time::ptime& ptm, bool is_iso_format = false );

//获取ptime中的日期字符串
string ptime2datestr( boost::posix_time::ptime& ptm );

//获取ptime中的时间字符串
string ptime2timeStr(const boost::posix_time::ptime& time ) ;

boost::posix_time::ptime timestr2ptime(kn_string& str_time ) ;

bool isTimeValid( kn_string &_time );

//拆分字符串到int类型vector（支持中英文逗号分隔）
vector <int> splitString2IntVec( const char* lpstr );

//拆分字符串（逗号分隔）
vector <kn_string> splitString2Vect( const kn_char* lpstr ) ;

//去除字符串中的换行符
kn_string wipeOutStringLineBreak(const kn_string& str);

kn_string GetAttributeString( TiXmlElement* pElement, const char* name );

kn_string GetXMLNodeValueString(TiXmlElement* pElement);
//从xml节点中获取Int类型的值
int GetAttributeInt( TiXmlElement* pElement, const char* name ) ;

//从xml节点中获取到的String中分解出int类型的Vector
std::vector<int> GetAttribute2IntVector( TiXmlElement* pElement, const char* name );

string IntVector2str( const vector <int>& vec );  

//设置xmstring类型属性值，本方法中会自动从unicode转换为utf-8
void SetAttributeStr( TiXmlElement* pElement, const char* name, const kn_string & value );

void SetAttributeStr( TiXmlElement* pElement, const char* name, const char* value );
//获得xml节点的值
kn_string getXmlElementValue( TiXmlElement* p_element );

//获得当前节点的子节点中的文本
kn_string getChildNodeText(TiXmlElement* p_parent_elem , const char* elem_name);

//获得xml节点中的整数值
int getXmlNodeValue(TiXmlElement* p_element );

//获得当前节点的子节点中的整数值
int getChildNodeValue(TiXmlElement* p_parent_elem , const char* elem_name );

//Excel中日期和时间转换为ptime，excel日期从1900 年 1 月 1 日起。
boost::posix_time::ptime excelTime2ptime(int i_date,double d_time);

//创建新的XML NODE,并返回添加后的NODE对象的指针
TiXmlElement* add_node(TiXmlElement* parent, const char* node_name,  const char* value);

TiXmlElement* add_node(TiXmlElement* parent, const char* node_name, const kn_string & value);
//获得当前节点的文本
kn_string GetTextString( TiXmlElement* pElement );
// TEMPLATE STRUCT less_nprjobject
// 各种类型对象通用的比较函数
template<class _Ty>
struct less_nprjobject
    : public binary_function<_Ty, _Ty, bool>
{	// functor for operator<
    bool operator()(const _Ty& _Left, const _Ty& _Right) const
    {	// apply operator< to operands
        return (_Left.getId() < _Right.getId() );
    }
};

#endif // utility_h__
