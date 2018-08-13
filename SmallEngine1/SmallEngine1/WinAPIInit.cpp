#include "stdafx.h"

#include "WinAPIInit.h"


LRESULT __stdcall WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

WinAPIInit::WinAPIInit(HINSTANCE hInstance, int nCmdShow)
{
	this->hInstance = hInstance;

	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, (LPCWSTR)(IDI_ICON1));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"class";
	RegisterClass(&wc);


	hWnd = CreateWindow(L"class", L"SmallEngine",
		WS_OVERLAPPEDWINDOW,
		0, 0, 1920 / 1, 1200 / 1,
		NULL, NULL, hInstance, NULL);


	ShowWindow(hWnd, 10);
	UpdateWindow(hWnd);
}

HWND WinAPIInit::GetHWnd()
{
	return hWnd;
}

HINSTANCE WinAPIInit::GetHInstance()
{
	return hInstance;
}

bool WinAPIInit::Tick()
{
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

WinAPIInit::~WinAPIInit()
{}

LRESULT __stdcall WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}