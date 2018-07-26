// DXTest0.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <windows.h>
#include <d3d11.h>

#pragma 

HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;          // Устройство (для создания объектов)
ID3D11DeviceContext*    g_pImmediateContext = NULL;   // Контекст устройства (рисование)
IDXGISwapChain*         g_pSwapChain = NULL;          // Цепь связи (буфера с экраном)
ID3D11RenderTargetView* g_pRenderTargetView = NULL;   // Объект заднего буфера

LRESULT __stdcall WndProc(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam);

HRESULT InitDevice();      // Инициализация устройств DirectX
void CleanupDevice();      // Удаление созданнных устройств DirectX
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);       // Функция окна
void Render();      // Функция рисования



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
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
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
	g_hWnd = hWnd;
	g_hInst = hInstance;



	if (FAILED(InitDevice()))
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


HRESULT InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;           // получаем ширину
	UINT height = rc.bottom - rc.top;   // и высоту окна
	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);


	// Тут мы создаем список поддерживаемых версий DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	// Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
	// которая описывает свойства переднего буфера и привязывает его к нашему окну.
	DXGI_SWAP_CHAIN_DESC sd;            // Структура, описывающая цепь связи (Swap Chain)
	ZeroMemory(&sd, sizeof(sd));    // очищаем ее
	sd.BufferCount = 1;                               // у нас один задний буфер
	sd.BufferDesc.Width = width;                     // ширина буфера
	sd.BufferDesc.Height = height;                          // высота буфера
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
	sd.BufferDesc.RefreshRate.Numerator = 75;         // частота обновления экрана
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
	sd.OutputWindow = g_hWnd;                               // привязываем к нашему окну
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;                               // не полноэкранный режим

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);

		if (SUCCEEDED(hr)) // Если устройства созданы успешно, то выходим из цикла
			break;
	}
	if (FAILED(hr)) return hr;

	// Теперь создаем задний буфер. Обратите внимание, в SDK
	// RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return hr;

	// Я уже упоминал, что интерфейс g_pd3dDevice будет
	// использоваться для создания остальных объектов
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	// Подключаем объект заднего буфера к контексту устройства
	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

		// Настройка вьюпорта
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// Подключаем вьюпорт к контексту устройства
	g_pImmediateContext->RSSetViewports(1, &vp);    
	
	return S_OK;
}

void CleanupDevice()
{
	// Сначала отключим контекст устройства, потом отпустим объекты.
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	// Порядок удаления имеет значение. Обратите внимание, мы удалеям
	// эти объекты порядке, обратном тому, в котором создавали.
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();
}

void Render()
{
	// Просто очищаем задний буфер
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	// Выбросить задний буфер на экран
	g_pSwapChain->Present(0, 0);
}