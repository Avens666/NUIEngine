// CalendarDemo.cpp : 定义应用程序的入口点。
//


#include "NUIMainView.h"


#include "mainforwindows.h"

#ifdef WIN32
#include "resource.h"
#include "FilePath.h"
#endif

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
HWND g_hWnd = 0;
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

int g_iScreenWidth = 1024; // 屏幕宽度
int g_iScreenHeight = 768; // 屏幕高度
// bool		g_bMoveWindow = FALSE;
int g_mouse_down_flag = FALSE;
CNUIInstance  g_nui_instance;

#ifdef WIN32
void CreateView();
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NUIApp, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDD_NUIApp_DIALOG));

	// NUI接管主消息循环:
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	// 主消息循环:
	kn_bool bExit = FALSE;
	g_nui_instance.RunEventLoop(bExit);

	g_nui_instance.release();
	return  0;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NUIApp));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HMI);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NUIApp));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd =  CreateWindow(
	   szWindowClass,
	   szTitle,
	   WS_POPUP,
	   (GetSystemMetrics(SM_CXSCREEN) - g_iScreenWidth) / 2,
	   (GetSystemMetrics(SM_CYSCREEN) - g_iScreenHeight) / 2,
	   g_iScreenWidth, g_iScreenHeight,
	   NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   KNUIInitPara para;
   para.m_w = g_iScreenWidth;
   para.m_h = g_iScreenHeight;
   para.m_wnd = hWnd;
   para.m_render_mode = SCREEN_RENDER_MULTI_THREAD;

   para.m_device_type = WIN_DEVICE;


   g_nui_instance.create(para );
   kn_string strPath;
   GetFilePath(strPath);
   strPath += _T("../img/");
   SetCurrentPath(strPath.c_str());
   CreateView();
   g_hWnd = hWnd;

   g_nui_instance.getScreen()->SetRenderFlag(TRUE);
   ShowWindow( hWnd, nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch ( message )
	{
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);
		//	EndPaint(hWnd, &ps);
		//	break;
	case WM_DESTROY:
		//异步渲染，要在此结束渲染线程
		PostQuitMessage( 0 );
		break;
	case WM_KEYDOWN:


		break;
	default:
		break;
	}

	g_nui_instance.NUIWndProc( hWnd, message, wParam, lParam );

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
#endif

void CreateView()
{
	////  创建标题


	KMainView_PTR main_view = KMainView_PTR(new KMainView() );
	main_view->Create(RERect::MakeXYWH(0, 0, (SkScalar)g_iScreenWidth , (SkScalar)g_iScreenHeight));
	g_nui_instance.getScreen()->AddView( main_view );
	main_view->init();

}

//====================================================================
// for  andorid &  ios
void ResizeWindow(void* dst, int w, int h)
{
	g_nui_instance.ResizeWindow(dst, w, h);
	g_iScreenWidth = w;
	g_iScreenHeight = h;
}

void SendOSMessage(int message, int wParam, int lParam)
{
	if(message == KMSG_TIMER)
	{
		g_nui_instance.SendTimerEvent(0);

	}
	else
	{
		g_nui_instance.NUIWndProc(0, message, wParam, lParam);

	}

}
