//  **************************************
//  File:        NE_error.h
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

// 参照WinError.h

#ifndef _NE_ERROR_H_
#define _NE_ERROR_H_

#include "NE_type.h"

// 错误码包含四种信息：信息类别、发生错误的组件、错误的类别、错误码
// #define  RESULT(CLASS, COM, FAC, CODE) (KNRESULT)((class NUI_API NUI_API << 29) | (COM << 25) | (FAC << 16) | CODE) 

// 全0(0x00000000)，表示没有错误
#define  RESULT_OK KNRESULT(EC_SUCCESS, SC_NONE, EF_NONE, KN_ERROR_NONE) 

// 全F(0xFFFFFFFF)，表示错误未知
#define  RESULT_UNKNOW KNRESULT(EC_UNKNOW, SC_UNKNOW, EF_UNKNOW, KN_ERROR_UNKNOW)

//
//  Values are 32 bit values layed out as follows:
//
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-----+-------+-----------------+-------------------------------+
//  |Class|   Com |     Facility    |               Code            |
//  +-----+-------------------------+-------------------------------+
//
//  where
//
//      class NUI_API NUI_API - is the class NUI_API code
//
//          000 - Success
//          001 - Informational
//          010 - Warning
//          011 - Error
//
//      Com - is the component code flag
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//

// 导航系统错误码.
struct KNRESULT
{
    kn_dword dwCode     : 16;  // 特定错误类别的状态.
    kn_dword dwFacility : 9;   // 错误分类.
    kn_dword dwComponent : 4;  // 错误所属的组件.
    kn_dword dwclass  : 3;   // 错误的类别.

    KNRESULT(kn_dword dwResult)	
    {
        dwclass = (dwResult >> 29) & 0x7;
        dwComponent = (dwResult >> 25) & 0xF;
        dwFacility = (dwResult >> 16) & 0x1FF;
        dwCode = dwResult & 0xFFFF;
    }

    KNRESULT(kn_dword A, kn_dword B, kn_dword C, kn_dword D)
    {
        dwclass = A;
        dwComponent = B;
        dwFacility = C;
        dwCode = D;
    }

    KNRESULT()
    {
        dwclass = 0;
        dwComponent = 0;
        dwFacility = 0;
        dwCode = 0;
    }

    operator kn_dword()
    {
        return ((dwclass << 29) | (dwComponent << 25) | (dwFacility << 16) | dwCode);
    }

};

// 取得最后错误码
KNRESULT GetLastErrorCode();

// 设置最后错误码
void SetLastErrorCode(KNRESULT theErrorCode);

kn_bool operator==(const KNRESULT& lhs, const KNRESULT& rhs);

kn_bool operator!=(const KNRESULT& lhs, const KNRESULT& rhs);


// 错误等级. 3 Bits : 可容纳8个等级. << 29
enum KNERRORCLASS
{
    EC_SUCCESS     = 0,		// 没有错误.
    EC_INFORMATION = 1,		// 提示信息.
    EC_WARNING     = 2,		// 警告.
    EC_ERROR       = 3,		// 错误.
    EC_UNKNOW      = 0x7,	// 未知的。
};

// 导航系统组件. 4 Bits : 可容纳16个组件. << 25
enum KNSYSCOM
{
    SC_NONE				= 0,
    SC_UNKNOW			= 0xF,	// 未知的
    SC_NUI				= 1,	// HMI
	SC_BASELIB			= 2
};

// 错误类别. 9 Bits : 可容纳512个种类. << 16
enum KNERRORFACILITY
{
    EF_NONE		  = 0,
    EF_UNKNOW     = 0x1FF,	// 未知的。
    EF_MEMORY     = 1,		// 内存分配.
    EF_FILE       = 2,		// 文件操作.
    EF_SERIALPORT = 3,		// 串口操作.
	EF_PROTOCOL   = 4,		// 网络协议解析.
};

// 错误状态. 16Bits.
enum KNERRORSTATUSCODE
{
    KN_ERROR_NONE								= 0,		// 无错误。

    KN_ERROR_UNKNOW								= 0xFFFF,	// 未知的。

    // Memory.
    KN_ERROR_NOT_ENOUGH_MEMORY					= 0xFFFF,	// 内存不够.

    // File.
    KN_ERROR_OPEN_FAILED						= 110,		// 文件打开失败.
    KN_ERROR_OPEN_REPEAT,                                   // 重复打开文件
    KN_ERROR_READ_FAIL,                                     // 文件读失败
    KN_ERROR_WRITE_FAIL,                                    // 文件写失败
    KN_ERROR_NOT_OPEN,                                      // 文件未打开

    // Serial Port.
    KN_ERROR_EXECUTE_EXTENT_COMMAND_INCOMPLETE	= 800,		// 未能完全解析并执行外部请求命令.

    KN_ERROR_WRITE_SERIAL_PORT_EXCEPTION		= 801,		// 通过串口输出内容时发生异常.

	// Protocol.
	KN_ERROR_PROTOCOL_PARM_ERROR				= 900,		// 参数错误.
	KN_ERROR_PROTOCOL_DB_ERROR					= 901,		// DB错误.
	KN_ERROR_PROTOCOL_UNKNOW_ERROR				= 902,		// 未知错误.
	KN_ERROR_PROTOCOL_DBCONNECT_ERROR			= 980,		// DB连接错误.
	KN_ERROR_PROTOCOL_DBGET_ERROR				= 981,		// DB获取错误.
	KN_ERROR_PROTOCOL_DBADD_ERROR				= 982,		// DB追加错误.
	KN_ERROR_PROTOCOL_DBUPDATE_ERROR			= 983,		// DB更新错误.
	KN_ERROR_PROTOCOL_DBDELETE_ERROR			= 984,		// DB删除错误.
	KN_ERROR_PROTOCOL_SESSIONDISCONNECTED_ERROR	= 910,		// session 断开了.
	KN_ERROR_PROTOCOL_ACCOUNTPASSWORD_ERROR		= 911,		// account/password错误.
	KN_ERROR_PROTOCOL_AIDDISMATCHDID_ERROR		= 912,		// account和端末ID不一致.
	KN_ERROR_PROTOCOL_ACCOUNTLOGICDELETE_ERROR	= 913,		// account已被逻辑删除.
	KN_ERROR_PROTOCOL_ACCOUNTEXIST_ERROR		= 915,		// 已有相同account名存在.
	KN_ERROR_PROTOCOL_DEVICEIDEXIST_ERROR		= 916,		// 已有相同端末ID存在.
	KN_ERROR_PROTOCOL_ACCOUNTLENGTH_ERROR		= 917,		// account名的字符数error.
	KN_ERROR_PROTOCOL_PASSWORDLENGTH_ERROR		= 918,		// password的字符数error.
	KN_ERROR_PROTOCOL_NICKNAMELENGTH_ERROR		= 919,		// Nick name的字符数error.
	KN_ERROR_PROTOCOL_ACCOUNTROLE_ERROR			= 920,		// account名的命名规则error.
	KN_ERROR_PROTOCOL_PASSWORDROLE_ERROR		= 921,		// password的命名规则error.
	KN_ERROR_PROTOCOL_NICKNAMEROLE_ERROR		= 922,		// Nick name的命名规则error.
	KN_ERROR_PROTOCOL_NICKNAMEEXIST_ERROR		= 923,		// 已有Nick name存在.
	KN_ERROR_PROTOCOL_NOFRIEND_ERROR			= 930,		// 没有friend.
	KN_ERROR_PROTOCOL_NOTFINDFRIEND_ERROR		= 931,		// 检索不到friend.
	KN_ERROR_PROTOCOL_OVERFRIENDSLENGTH_ERROR	= 932,		// friend注册数超过最大值.
	KN_ERROR_PROTOCOL_NOLOCATION_ERROR			= 933,		// 因为最终位置信息不明，所以无法共享.
	KN_ERROR_PROTOCOL_FRIENDALREADYIN_ERROR		= 934,		// 已注册.
	KN_ERROR_PROTOCOL_FRIENDALREADYAPPLY_ERROR	= 935,		// 已申请.
	KN_ERROR_PROTOCOL_FRIENDISMYSELF_ERROR		= 936,		// 是自己.
	KN_ERROR_PROTOCOL_OVERGROUPFRIENDSNUM_ERROR	= 940,		// 超过Group中可注册的friend数.
	KN_ERROR_PROTOCOL_CANNOTDELETEMANAGER_ERROR	= 941,		// 管理者无法删除.
	KN_ERROR_PROTOCOL_FRIENDALREADYINGROUP_ERROR= 942,		// 已注册.
	KN_ERROR_PROTOCOL_GROUPFRIENDOVERNUMS_ERROR	= 943,		// 超过Group中可注册的friend数.
	KN_ERROR_PROTOCOL_MESSAGELENGTH_ERROR		= 960,		// 字符数超过规定值.
	KN_ERROR_PROTOCOL_MESSAGENOTFRIEND_ERROR	= 961,		// 不是friend.
};

#endif // _NE_ERROR_H_
