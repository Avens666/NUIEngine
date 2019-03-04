#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <time.h>  
#include <sys/time.h> 
#include "NE_type.h"
#include "NE_pubhead.h"
#include "message.h"
#include "KBufferReader.h"
#include "renderingengine.h"
#include "FilePath.h"
#include "LINUXTCHAR.h"
#include "CharSet.h"

using namespace std;
void SendOSMessage(int message, int wParam, int lParam);

extern kn_int g_iScreenWidth;
extern kn_int g_iScreenHeight;

struct KJavaSurface
{
	int iSurfaceWidth;
	int iSurfaceHeight;
	unsigned char* pBuff;
	int iDeviceWidth;
	int iDeviceHeight;
	KJavaSurface()
	{
		iSurfaceWidth = 0;
		iSurfaceHeight= 0;
		pBuff = NULL;
		iDeviceWidth = 0;
		iDeviceHeight = 0;
	}
};

KJavaSurface g_javaSurface;

void SetJavaSurfaceBuf(unsigned char* pJavaSurfaceBuf)
{
	g_javaSurface.pBuff = pJavaSurfaceBuf;
}

int StartNUIEngine(const char* szAppDataPath, const char* savPath, unsigned char* pJavaSurfaceBuf,
		int iSurfaceWidth, int iSurfaceHeight, int iDeviceWidth, int iDeviceHeight)
{
	LOGI ("StartNUIEngine");
	g_javaSurface.pBuff = pJavaSurfaceBuf;
	g_javaSurface.iSurfaceWidth = iSurfaceWidth;
	g_javaSurface.iSurfaceHeight = iSurfaceHeight;
	g_javaSurface.iDeviceWidth = iDeviceWidth;
	g_javaSurface.iDeviceHeight = iDeviceHeight;

	kn_char sDataPath[MAX_PATH] = {0};
	mbstowcs(sDataPath, szAppDataPath, strlen(szAppDataPath));
	SetDataPath(sDataPath);
	// 设置NUI资源路径
	_tcscat(sDataPath, _T("img/"));
	SetCurrentPath(sDataPath);
	kn_char sPath[MAX_PATH] = {0};
	mbstowcs(sPath, savPath, strlen(savPath));
	SetSavPath(sPath);
	//PreInitNavigation();
	LOGI("InitEngine OK");
	return 0;
}
 
void SendMouseEvent(int msgtype, int x, int y, int iPointCount, int iBufferSize, unsigned char* pBuf)
{
    int wParam = 0;
    int lParam = 0;
	 // 重新映射坐标
    if(g_javaSurface.iDeviceWidth != 0 && g_javaSurface.iDeviceHeight != 0)
    {
    	if(msgtype >= KMSG_TOUCH_UP && msgtype <= KMSG_TOUCH_DOWN)
    	{
    		ASSERT(iPointCount * 8 == iBufferSize);
    		wParam = iPointCount;
    		POINT* points = new POINT[iPointCount];
    		KBufferReader reader (pBuf, iBufferSize);
    		for(int i = 0; i < iPointCount; i++)
    		{
        		int x = 0;
        		int y = 0;
        		reader.Read(x);
        		reader.Read(y);
        		points[i].x = x * g_iScreenWidth / g_javaSurface.iDeviceWidth;
        		points[i].y = y * g_iScreenHeight / g_javaSurface.iDeviceHeight;
    		}
    		lParam = (int)points;
    	}
    	else
    	{
    		// 单点的 wParam, lParam 为 x, y
    		wParam = x * g_iScreenWidth / g_javaSurface.iDeviceWidth;
    		lParam = y * g_iScreenHeight / g_javaSurface.iDeviceHeight;
    	}
    	//LOGI ("g_javaSurface (%d, %d,%d,%d,%d,%d,%d) ", msgtype, g_javaSurface.iDeviceWidth, g_javaSurface.iDeviceHeight, g_iScreenWidth, g_iScreenHeight, wParam, lParam);
    }
    else
    {
    	LOGE ("g_javaSurface (%d,%d) ", g_javaSurface.iDeviceWidth, g_javaSurface.iDeviceWidth);
    }
	//LOGI ("SendMouseEvent msgtype(%d), src(%d,%d) / hmiengine (%d,%d)", msgtype, x, y, msg.mousex, msg.mousey);
	SendOSMessage(msgtype, wParam, lParam);
}


void SendAndroidMessage(int message, int wParam, int lParam, int iPointCount, int iBufferSize, unsigned char* pBuf)
{
	if(message == KMSG_LBBUTTONDOWN || message == KMSG_LBBUTTONUP || message == KMSG_MOUSEMOVE || message == KMSG_LBUTTONDBLCLK
			|| message == KMSG_TOUCH_UP || message == KMSG_TOUCH_MOVE || message == KMSG_TOUCH_DOWN)
	{
		SendMouseEvent(message, wParam, lParam, iPointCount, iBufferSize, pBuf);
	}
	else if(message == KMSG_TIMER)
	{
		SendOSMessage(KMSG_TIMER, 0, 0);
	}
	else if(message == KMSG_TYPE_IME_UPDATE)
	{
		SendOSMessage(KMSG_TYPE_IME_UPDATE, iBufferSize, (int)pBuf);
	}
	else if(message == KMSG_EVENT_USER)
	{
		SendOSMessage(message, wParam, lParam);
	}
	else
	{
		LOGE ("SendAndroidMessage unknown message %d", message);
	}
}
