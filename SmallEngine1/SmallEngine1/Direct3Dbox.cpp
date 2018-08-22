#include "stdafx.h"
#include "Direct3Dbox.h"


Direct3Dbox::Direct3Dbox(WinAPIInit* pWinInit, WCHAR* szFileName, Camera* camera)
{
	HRESULT hr = S_OK;

	this->pWinInit = pWinInit;

	RECT rc;
	GetClientRect(pWinInit->GetHWnd(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;

	//	Дескриптор Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//	один задний буфер
	sd.BufferDesc.Width = width;						//	ширина буфера
	sd.BufferDesc.Height = height;						//	высота буфера
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//	формат пикселя в буфере
	sd.BufferDesc.RefreshRate.Numerator = 75;			//	частота обновления экрана (числитель)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//	частота обновления экрана (знаменатель)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//	назначение буфера - задний буфер
	sd.OutputWindow = pWinInit->GetHWnd();				//	привязываем к нашему окну
	sd.SampleDesc.Count = 1;							//	"Число кратных выборок на пиксель", что бы это ни значило
	sd.SampleDesc.Quality = 0;							//	Уровень качества изображения (Не совсем ясно зачем его обнулять, но здесь так принято)
	sd.Windowed = TRUE;									//	не полноэкранный режим

	pDevicesBox = new DirectDevicesBox(&sd, createDeviceFlags, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CheckAndThrowIfFailed(hr);
	
	//	Задний буфер, нужен не долго и только для получения TargetView
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pDevicesBox->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	CheckAndThrowIfFailed(hr);

	hr = pDevicesBox->GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	CheckAndThrowIfFailed(hr);


	//	Дескриптор буфера глубин (а ещё она текстура)
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;							//	ширина и
	descDepth.Height = height;							//	высота текстуры
	descDepth.MipLevels = 1;							//	уровень интерполяции
	descDepth.ArraySize = 1;							//	1.	(я так и не понял что это)
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//	формат (размер пикселя)
	descDepth.SampleDesc.Count = 1;						//	Число кратных выборок на пиксель
	descDepth.SampleDesc.Quality = 0;					//	Уровень качества изображения
	descDepth.Usage = D3D11_USAGE_DEFAULT;				//	юзаем как обычно
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//	вид - буфер глубин
	descDepth.CPUAccessFlags = 0;						//	Доступ к CPU не нужен, поэтому 0
	descDepth.MiscFlags = 0;							//	Тут всякие дополнительные штуки, мы такие не используем

	hr = pDevicesBox->GetDevice()->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	CheckAndThrowIfFailed(hr);


	// Буфер глубины
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;					//	формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = pDevicesBox->GetDevice()->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	CheckAndThrowIfFailed(hr);

	pDevicesBox->GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// Компиляция вершинного шейдера из файла
	ID3DBlob* pVSBlob = NULL; // Вспомогательный объект - просто место в оперативной памяти
	hr = CompileShaderFromFile(szFileName, "VS", "vs_4_0", &pVSBlob);

	CheckAndThrowIfFailed(hr);

	// Создание вершинного шейдера
	hr = pDevicesBox->GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);
	CheckAndThrowIfFailed(hr);

	// Определение шаблона вершин
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	/* семантическое имя, семантический индекс, размер, входящий слот (0-15),
	адрес начала данных в буфере вершин, класс входящего слота (не важно), InstanceDataStepRate (не важно) */
	};
	UINT numElements = ARRAYSIZE(layout);

	// Создание шаблона вершин
	hr = pDevicesBox->GetDevice()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pVertexLayout);
	pVSBlob->Release();
	CheckAndThrowIfFailed(hr);

	// Подключение шаблона вершин
	pDevicesBox->GetDeviceContext()->IASetInputLayout(pVertexLayout);

	pPixelShader = new ID3D11PixelShader*[PixelShaderTypeLength];
	for (int i = 0; i < PixelShaderTypeLength; i++)
	{
		// Компиляция пиксельного шейдера из файла
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile(szFileName, ShaderTypeToLPCSTR((PixelShaderType)i), "ps_4_0", &pPSBlob);

		CheckAndThrowIfFailed(hr);

		// Создание пиксельного шейдера
		hr = pDevicesBox->GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &(pPixelShader[i]));
		pPSBlob->Release();
		CheckAndThrowIfFailed(hr);
	}

	D3D11_BUFFER_DESC bd;  // Структура, описывающая создаваемый буфер
	ZeroMemory(&bd, sizeof(bd));
	// Создание константного буфера
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);			//	размер буфера = размеру структуры
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//	тип - константный буфер
	bd.CPUAccessFlags = 0;
	hr = pDevicesBox->GetDevice()->CreateBuffer(&bd, NULL, &pConstantBuffer);
	CheckAndThrowIfFailed(hr);

	this->camera = camera;
	cb.sunPosition = XMFLOAT3(0.5, 1, -0.5);

	XMMATRIX Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, width / (FLOAT)height, 0.01f, 100.0f);
	cb.mProjection = XMMatrixTranspose(Projection);
}

Direct3Dbox::~Direct3Dbox()
{
	pDevicesBox->~DirectDevicesBox();

	if (pDepthStencilView) pDepthStencilView->Release();
	if (pDepthStencil) pDepthStencil->Release();
	if (pRenderTargetView) pRenderTargetView->Release();
}