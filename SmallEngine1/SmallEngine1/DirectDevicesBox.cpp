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

	//	C����� �������������� ������ DirectX
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

		if (SUCCEEDED(hr))	// ���� ���������� ������� �������, �� ������� �� �����
			break;
	}
	CheckAndThrowIfFailed(hr);


	// ��������� ��������
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SwapChainDesc->BufferDesc.Width;
	vp.Height = (FLOAT)SwapChainDesc->BufferDesc.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// ���������� ������� � ��������� ����������
	pImmediateContext->RSSetViewports(1, &vp);		//	��������, ���� ����������	
	
	// ��������� ������� ��������� ������ � ������
	pImmediateContext->IASetPrimitiveTopology(Topology);
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
