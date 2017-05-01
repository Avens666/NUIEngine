//  **************************************
//  File:        error.cpp
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


#include "NE_pubhead.h"
#include "NE_error.h"

KNRESULT g_LatestErrorCode = RESULT_OK;

// 取得最后错误码
KNRESULT GetLastErrorCode()
{
    return g_LatestErrorCode;
}

// 设置最后错误码
void SetLastErrorCode(KNRESULT theErrorCode)
{
    g_LatestErrorCode = theErrorCode;
}

kn_bool operator==(const KNRESULT& lhs, const KNRESULT& rhs)
{
    if (lhs.dwclass == rhs.dwclass && lhs.dwCode == rhs.dwCode && lhs.dwComponent == rhs.dwComponent && lhs.dwFacility == rhs.dwFacility)
    {
        return true;
    }
    else
    {
        return false;
    }
}

kn_bool operator!=(const KNRESULT& lhs, const KNRESULT& rhs)
{
    return !(lhs == rhs);
}
