#include "stdafx.h"
#include "DirectDevicesBox.h"


DirectDevicesBox::DirectDevicesBox(DXGI_SWAP_CHAIN_DESC* SwapChainDesc, UINT createDeviceFlags, 
	D3D11_PRIMITIVE_TOPOLOGY Topology)
{

	HRESULT hr = S_OK;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE ,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	//	Cписок поддерживаемых версий DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);


	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, SwapChainDesc, &pSwapChain, &pd3dDevice, &featureLevel, &pImmediateContext);

		if (SUCCEEDED(hr))	// Если устройства созданы успешно, то выходим из цикла
			break;
	}
	CheckAndThrowIfFailed(hr);


	// Настройка вьюпорта
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SwapChainDesc->BufferDesc.Width;
	vp.Height = (FLOAT)SwapChainDesc->BufferDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// Подключаем вьюпорт к контексту устройства
	pImmediateContext->RSSetViewports(1, &vp);		//	Возможно, надо переделать	
	
	// Установка способа отрисовки вершин в буфере
	pImmediateContext->IASetPrimitiveTopology(Topology);
}

HRESULT DirectDevicesBox::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{

	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;

	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		0, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
		{
			char* r = (char*)pErrorBlob->GetBufferPointer();
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		}
		if (pErrorBlob) pErrorBlob->Release();
		MessageBox(NULL, szFileName, L"Ошибка. Не найден или не компилируется файл.", MB_OK);
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

ID3D11Device* DirectDevicesBox::GetDevice()
{
	return pd3dDevice;
}

IDXGISwapChain* DirectDevicesBox::GetSwapChain()
{
	return	pSwapChain;
}

ID3D11DeviceContext* DirectDevicesBox::GetDeviceContext()
{
	return pImmediateContext;
}

D3D_DRIVER_TYPE DirectDevicesBox::GetDriverType()
{
	return driverType;
}

D3D_FEATURE_LEVEL DirectDevicesBox::GetFeatureLevel()
{
	return featureLevel;
}

DirectDevicesBox::~DirectDevicesBox()
{
	if (pImmediateContext) pImmediateContext->ClearState();

	if (pSwapChain) pSwapChain->Release();
	if (pd3dDevice) pd3dDevice->Release();
	if (pImmediateContext) pImmediateContext->Release();
}
