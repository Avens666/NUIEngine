//  **************************************
//  File:        NE_macro.h
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


#ifndef _NE_MACRO_H_
#define _NE_MACRO_H_

#define KNAPI               // API调用协议

#define IN      // 输入参数
#define OUT     // 输出参数
#define INOUT   // 输入、输出参数

// 安全删除一个需调用delete释放的对象
#define SAFE_DELETE(pObject) if(pObject) {delete (pObject); (pObject) = NULL;}
// 安全删除一组需调用delete[]释放的对象
#define SAFE_DELETE_GROUP(pObject) if(pObject) {delete[] (pObject); (pObject) = NULL;}

#ifndef PI
    #define PI		(3.14159265358979323846f)
#endif





#endif // _NE_MACRO_H_
