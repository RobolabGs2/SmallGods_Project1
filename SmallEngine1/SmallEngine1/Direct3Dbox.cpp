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

	pDevicesBox = new DirectDevicesBox(&sd, createDeviceFlags, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CheckAndThrowIfFailed(hr);
	
	//	������ �����, ����� �� ����� � ������ ��� ��������� TargetView
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = pDevicesBox->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	CheckAndThrowIfFailed(hr);

	hr = pDevicesBox->GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	CheckAndThrowIfFailed(hr);


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

	hr = pDevicesBox->GetDevice()->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	CheckAndThrowIfFailed(hr);


	// ����� �������
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;					//	������ ��� � ��������
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	hr = pDevicesBox->GetDevice()->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
	CheckAndThrowIfFailed(hr);

	pDevicesBox->GetDeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	// ���������� ���������� ������� �� �����
	ID3DBlob* pVSBlob = NULL; // ��������������� ������ - ������ ����� � ����������� ������
	hr = CompileShaderFromFile(szFileName, "VS", "vs_4_0", &pVSBlob);

	CheckAndThrowIfFailed(hr);

	// �������� ���������� �������
	hr = pDevicesBox->GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);
	CheckAndThrowIfFailed(hr);

	// ����������� ������� ������
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	/* ������������� ���, ������������� ������, ������, �������� ���� (0-15),
	����� ������ ������ � ������ ������, ����� ��������� ����� (�� �����), InstanceDataStepRate (�� �����) */
	};
	UINT numElements = ARRAYSIZE(layout);

	// �������� ������� ������
	hr = pDevicesBox->GetDevice()->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &pVertexLayout);
	pVSBlob->Release();
	CheckAndThrowIfFailed(hr);

	// ����������� ������� ������
	pDevicesBox->GetDeviceContext()->IASetInputLayout(pVertexLayout);

	pPixelShader = new ID3D11PixelShader*[PixelShaderTypeLength];
	for (int i = 0; i < PixelShaderTypeLength; i++)
	{
		// ���������� ����������� ������� �� �����
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile(szFileName, ShaderTypeToLPCSTR((PixelShaderType)i), "ps_4_0", &pPSBlob);

		CheckAndThrowIfFailed(hr);

		// �������� ����������� �������
		hr = pDevicesBox->GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &(pPixelShader[i]));
		pPSBlob->Release();
		CheckAndThrowIfFailed(hr);
	}

	D3D11_BUFFER_DESC bd;  // ���������, ����������� ����������� �����
	ZeroMemory(&bd, sizeof(bd));
	// �������� ������������ ������
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);			//	������ ������ = ������� ���������
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;		//	��� - ����������� �����
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