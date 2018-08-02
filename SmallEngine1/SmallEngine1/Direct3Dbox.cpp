#include "stdafx.h"
#include "Direct3Dbox.h"


Direct3Dbox::Direct3Dbox(WinAPIInit* pWinInit)
{
	HRESULT hr = S_OK;

	this->pWinInit = pWinInit;

	RECT rc;
	GetClientRect(pWinInit->GetHWnd(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	UINT createDeviceFlags = 0;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE ,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);


	//	C����� �������������� ������ DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	//	���������� Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;									//	���� ������ �����
	sd.BufferDesc.Width = width;						//	������ ������
	sd.BufferDesc.Height = height;						//	������ ������
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//	������ ������� � ������
	sd.BufferDesc.RefreshRate.Numerator = 75;			//	������� ���������� ������ (���������)
	sd.BufferDesc.RefreshRate.Denominator = 1;			//	������� ���������� ������ (�����������)
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//	���������� ������ - ������ �����
	sd.OutputWindow = pWinInit->GetHWnd();				//	����������� � ������ ����
	sd.SampleDesc.Count = 1;							//	"����� ������� ������� �� �������", ��� �� ��� �� �������
	sd.SampleDesc.Quality = 0;							//	������� �������� ����������� (�� ������ ���� ����� ��� ��������, �� ����� ��� �������)
	sd.Windowed = TRUE;									//	�� ������������� �����

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pImmediateContext);

		if (SUCCEEDED(hr))	// ���� ���������� ������� �������, �� ������� �� �����
			break;
	}

	Exp(hr);

	//	������ �����, ����� �� ����� � ������ ��� ��������� TargetView
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	Exp(hr);

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	Exp(hr);


	//	���������� ������ ������ (� ��� ��� ��������)
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;							//	������ �
	descDepth.Height = height;							//	������ ��������
	descDepth.MipLevels = 1;							//	������� ������������
	descDepth.ArraySize = 1;							//	1.	(� ��� � �� ����� ��� ���)
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//	������ (������ �������)
	descDepth.SampleDesc.Count = 1;						//	����� ������� ������� �� �������
	descDepth.SampleDesc.Quality = 0;					//	������� �������� �����������
	descDepth.Usage = D3D11_USAGE_DEFAULT;				//	����� ��� ������
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//	��� - ����� ������
	descDepth.CPUAccessFlags = 0;						//	������ � CPU �� �����, ������� 0
	descDepth.MiscFlags = 0;							//	��� ������ �������������� �����, �� ����� �� ����������

	hr = pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	Exp(hr);


	// ������ ���� ������� ��� ������ ������ ������
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;            // ��������� � �����������
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         // ������ ��� � ��������
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// ��� ������ ����������� ���������-�������� � �������� ������� ������ ������ ������
	hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	Exp(hr);

	// ���������� ������ ������� ������  � ������ ������ � ��������� ����������
	pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// ��������� ��������
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// ���������� ������� � ��������� ����������
	pImmediateContext->RSSetViewports(1, &vp);

}

//	������� ���������� ���� hr �� S_OK
void Direct3Dbox::Exp(HRESULT hr)
{
	if (FAILED(hr))
		throw std::exception((char*)hr);
}

Direct3Dbox::~Direct3Dbox()
{
	if (pImmediateContext) pImmediateContext->ClearState();

	if (pDepthStencilView) pDepthStencilView->Release();
	if (pDepthStencil) pDepthStencil->Release();
	if (pRenderTargetView) pRenderTargetView->Release();
	if (pSwapChain) pSwapChain->Release();
	if (pImmediateContext) pImmediateContext->Release();
	if (pd3dDevice) pd3dDevice->Release();
}