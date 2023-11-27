// WindowsAPIProject.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsAPIProject.h"

#define MAX_LOADSTRING 100
#define MIN_WINDOW_WIDTH 200;
#define MIN_WINDOW_HEIGHT 150;

// Global Variables:
HINSTANCE hInst;                                  // current instance
WCHAR szTitle[ MAX_LOADSTRING ];                  // The title bar text
WCHAR szWindowClass[ MAX_LOADSTRING ];            // the main window class name

// Forward declarations of functions:
ATOM                MyRegisterClass( HINSTANCE hInstance );
BOOL                InitInstance( HINSTANCE, int );
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK    About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );
	// Existing comment
	// TODO: Place code here.

	// Suggested improvement
	// TODO: If there are any specific initialization steps required at this point, add comments explaining them.

	// Initialize global strings
	// Existing usage of resource IDs
	LoadStringW( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	// Suggested improvement
// Assuming that resource IDs like IDS_APP_TITLE are correctly defined in resource files.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	LoadStringW( hInstance, IDC_WINDOWSAPIPROJECT, szWindowClass, MAX_LOADSTRING );
	MyRegisterClass( hInstance );

	// Perform application initialization:
	// Existing error handling
	if ( !InitInstance( hInstance, nCmdShow ) )
	{
		return FALSE;
	}
	// Suggested improvement
	// Consider providing more detailed error information for debugging purposes.
	if (!InitInstance(hInstance, nCmdShow)) {
		// You may log error details or return GetLastError().
		return GetLastError();
	}

	HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_WINDOWSAPIPROJECT ) );

	MSG msg;

	// Main message loop:
	while ( GetMessage( &msg, nullptr, 0, 0 ) )
	{
		if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_WINDOWSAPIPROJECT ) );
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW( IDC_WINDOWSAPIPROJECT );
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	return RegisterClassExW( &wcex );
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
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
	hInst = hInstance; // Store instance handle in our global variable
	// Inconsistent naming in InitInstance function
	HWND hWnd = CreateWindowW( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr );
	// Suggested improvement
	// Consider using a consistent naming convention, e.g., mainWindowHandle instead of hWnd.
	HWND mainWindowHandle = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if ( !hWnd )
	{
		return FALSE;
	}

	ShowWindow( hWnd, nCmdShow );
	UpdateWindow( hWnd );

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD( wParam );
		// Parse the menu selections:
		switch ( wmId )
		{
		case IDM_ABOUT:
			DialogBox( hInst, MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
			break;
		case IDM_EXIT:
			DestroyWindow( hWnd );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint( hWnd, &ps );
		// Some drawing code that uses hdc here...
		EndPaint( hWnd, &ps );
	}
	break;
	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( message )
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
		{
			EndDialog( hDlg, LOWORD( wParam ) );
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// Existing structure allowing easy extensibility
// Consider commenting on how new features can be integrated seamlessly without major code modifications.
