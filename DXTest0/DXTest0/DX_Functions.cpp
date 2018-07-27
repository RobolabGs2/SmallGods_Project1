
#include "stdafx.h"
#include "DX_Functions.h"


D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;			// Устройство (для создания объектов)
ID3D11DeviceContext*    g_pImmediateContext = NULL;		// Контекст устройства (рисование)
IDXGISwapChain*         g_pSwapChain = NULL;			// Цепь связи (буфера с экраном)
ID3D11RenderTargetView* g_pRenderTargetView = NULL;		// Объект заднего буфера

ID3D11VertexShader*     g_pVertexShader = NULL;			// Вершинный шейдер
ID3D11PixelShader*      g_pPixelShader = NULL;			// Пиксельный шейдер
ID3D11InputLayout*      g_pVertexLayout = NULL;			// Описание формата вершин
ID3D11Buffer*			g_pVertexBuffer = NULL;			// Буфер вершин

HRESULT CompileShaderFromFile(WCHAR* , LPCSTR , LPCSTR , ID3DBlob** );

HRESULT InitDevice(HWND hWnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;	// получаем ширину
	UINT height = rc.bottom - rc.top;	// и высоту окна
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
	sd.OutputWindow = hWnd;                               // привязываем к нашему окну
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
	// Сначала отключим контекст устройства
	if (g_pImmediateContext) g_pImmediateContext->ClearState();
	// Потом удалим объекты
	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pVertexLayout) g_pVertexLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pRenderTargetView) g_pRenderTargetView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pd3dDevice) g_pd3dDevice->Release();

}

void Render()
{
	// Просто очищаем задний буфер
	float ClearColor[4] = { 0.0f, 0.4f, 0.0f, 1.0f }; 
	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);
	
	// Подключить к устройству рисования шейдеры
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	// Нарисовать три вершины
	g_pImmediateContext->Draw(3, 0);

	// Выбросить задний буфер на экран
	g_pSwapChain->Present(0, 0);
}


HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		0, 0, ppBlobOut, NULL);


	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();
	
	return S_OK;
}


HRESULT InitGeometry()
{
	HRESULT hr = S_OK;

	WCHAR c[] = L"urok2.fx";

	// Компиляция вершинного шейдера из файла
	ID3DBlob* pVSBlob = NULL; // Вспомогательный объект - просто место в оперативной памяти
	hr = CompileShaderFromFile(c, "VS", "vs_4_0", &pVSBlob);          


	if (FAILED(hr))
	{
		MessageBox(NULL, L"Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", L"Ошибка", MB_OK);
		return hr;
	}

	// Создание вершинного шейдера
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Определение шаблона вершин
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/* семантическое имя, семантический индекс, размер, входящий слот (0-15), 
		адрес начала данных в буфере вершин, класс входящего слота (не важно), InstanceDataStepRate (не важно) */
	};
	UINT numElements = ARRAYSIZE(layout);

	// Создание шаблона вершин
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &g_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr)) return hr;

	// Подключение шаблона вершин
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	// Компиляция пиксельного шейдера из файла
	ID3DBlob* pPSBlob = NULL;
	hr = CompileShaderFromFile(c, "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Невозможно скомпилировать файл FX. Пожалуйста, запустите данную программу из папки, содержащей файл FX.", L"Ошибка", MB_OK);
		return hr;
	}

	// Создание пиксельного шейдера
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr)) return hr;

	// Создание буфера вершин (три вершины треугольника)
	SimpleVertex vertices[3];

	vertices[0].Pos.x = 0.0f;  vertices[0].Pos.y = 0.5f;  vertices[0].Pos.z = 0.5f;
	vertices[1].Pos.x = 0.5f;  vertices[1].Pos.y = -0.5f;  vertices[1].Pos.z = 0.5f;
	vertices[2].Pos.x = -0.5f;  vertices[2].Pos.y = -0.5f;  vertices[2].Pos.z = 0.5f;

	D3D11_BUFFER_DESC bd;  // Структура, описывающая создаваемый буфер
	ZeroMemory(&bd, sizeof(bd));                    // очищаем ее
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3; // размер буфера = размер одной вершины * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;          // тип буфера - буфер вершин
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA InitData; // Структура, содержащая данные буфера
	ZeroMemory(&InitData, sizeof(InitData)); // очищаем ее
	InitData.pSysMem = vertices;               // указатель на наши 3 вершины


											   // Вызов метода g_pd3dDevice создаст объект буфера вершин ID3D11Buffer
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	// Установка буфера вершин:
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Установка способа отрисовки вершин в буфере
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}