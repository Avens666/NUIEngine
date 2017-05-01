//  **************************************
//  File:        NE_debug.h
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

#ifndef _NE_DEBUG_H_
#define _NE_DEBUG_H_

#include <assert.h>

// Define ASSERT
#ifndef ASSERT
    
    #ifdef NDEBUG // for release
//         #define ASSERT(expr) \
//             if (!(expr)) LOG_ASSERT(__FILE__, __LINE__, __FUNCTION__, #expr);
			#if defined(QT_LINUX_LIB)
				#define  ASSERT(X) Q_ASSERT(X)
			#else
				#define  ASSERT(X)
			#endif
    #else
        #ifdef KOTEI_VC
            #include <crtdbg.h>
            #define ASSERT(X) _ASSERT(X)
        #else
			#define _ASSERT(X) assert(X) 
            #define ASSERT(X) assert(X)
        #endif // KOTEI_VC
    #endif	//	NDEBUG
#endif	//	ASSERT

// DEFINE _SYSTEMMONITOR
//#ifdef _DEBUG
//    #ifdef WIN32
//		#ifndef _SYSTEMMONITOR
//		#define _SYSTEMMONITOR // 启用监测系统
//		#endif
//    #endif
//#endif

#endif // _NE_DEBUG_H_
