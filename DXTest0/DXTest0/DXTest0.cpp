// DXTest0.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


#include "DX_Functions.h"

#include <windows.h>
#include "resource.h"


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int __stdcall WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
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


	HWND hWnd = CreateWindow(L"class", L"DirectX Test 0.2",
		WS_OVERLAPPEDWINDOW,
		150, 100, 320, 210, 
		NULL, NULL, hInstance, NULL);
	

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);



	if (FAILED(InitDevice(hWnd)))
	{
		CleanupDevice();
		return 0;
	}


	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();
		}
	}

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
