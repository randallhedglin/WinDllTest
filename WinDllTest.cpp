// WinDllTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WinDllTest.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>

/*
#include <WinSDKVer.h> // load flags
	#define NTDDI_VERSION NTDDI_WINBLUE
	#define _WIN32_WINNT  _WIN32_WINNT_WINBLUE
	#define WINVER        _WIN32_WINNT_WINBLUE
#include <sdkddkver.h> // apply flags

#define WIN32_LEAN_AND_MEAN // keep it light

#include <windows.h> // basic Windows components
*/

#include "FlashRuntimeExtensions.h"

#define DEBUG_MSG(msg) { MessageDialog^ dialog = ref new MessageDialog(msg); dialog->ShowAsync(); }

#define DECL_LIB_FN_PROTOTYPE(fn) typedef FREObject (__cdecl *fn)(FREContext, void*, uint32_t, FREObject*)

typedef void (__cdecl *_WindowsANEExtensionInitializer)(void**, FREContextInitializer*, FREContextFinalizer*);
typedef void (__cdecl *_WindowsANEContextInitializer)(void*, const uint8_t*, FREContext, uint32_t*, const FRENamedFunction**);
typedef void (__cdecl *_WindowsANEContextFinalizer)(void*);

DECL_LIB_FN_PROTOTYPE(_addFullScreenButton);
DECL_LIB_FN_PROTOTYPE(_messageBox);
DECL_LIB_FN_PROTOTYPE(_testANE);
DECL_LIB_FN_PROTOTYPE(_toggleFullScreen);

//////////////////////////////////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDLLTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDLLTEST));

    MSG msg;

//////////////////////////////////////////////////////////////////////////////////
{
	HMODULE hModule = LoadLibrary(TEXT("C:\\Users\\Randall Hedglin\\Documents\\Adobe Flash Builder\\WindowsANE\\build\\windows\\WindowsANE.dll"));
	
	std::cout << "err = " << GetLastError() << std::endl;
	std::cout << std::endl;

	std::cout << "hm  = " << hModule << std::endl;
	std::cout << std::endl;

	if(hModule)
	{
		_WindowsANEExtensionInitializer WindowsANEExtensionInitializer = (_WindowsANEExtensionInitializer) GetProcAddress(hModule, "WindowsANEExtensionInitializer");
		std::cout << "ei  = " << WindowsANEExtensionInitializer << std::endl;
		_WindowsANEContextInitializer WindowsANEContextInitializer = (_WindowsANEContextInitializer) GetProcAddress(hModule, "WindowsANEContextInitializer");
		std::cout << "ci  = " << WindowsANEContextInitializer << std::endl;
		_WindowsANEContextFinalizer WindowsANEContextFinalizer = (_WindowsANEContextFinalizer) GetProcAddress(hModule, "WindowsANEContextFinalizer");
		std::cout << "cf  = " << WindowsANEContextFinalizer << std::endl;
		_addFullScreenButton addFullScreenButton = (_addFullScreenButton) GetProcAddress(hModule, "addFullScreenButton");
		std::cout << "afs = " << addFullScreenButton << std::endl;
		_messageBox messageBox = (_messageBox) GetProcAddress(hModule, "messageBox");
		std::cout << "mb  = " << messageBox << std::endl;
		_testANE testANE = (_testANE) GetProcAddress(hModule, "testANE");
		std::cout << "ta  = " << testANE << std::endl;
		_toggleFullScreen toggleFullScreen = (_toggleFullScreen) GetProcAddress(hModule, "toggleFullScreen");
		std::cout << "tfs = " << toggleFullScreen << std::endl;
		std::cout << std::endl;

		void* pAppData;
		FREContextInitializer fnCtxInit;
		FREContextFinalizer fnCtxFinal;
		//WindowsANEExtensionInitializer(&pAppData, &fnCtxInit, &fnCtxFinal);

		FreeLibrary(hModule);
	}
}
//////////////////////////////////////////////////////////////////////////////////

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDLLTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDLLTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
