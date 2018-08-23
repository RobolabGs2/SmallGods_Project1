#pragma once

class DirectDevicesBox;

#include <windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <comdef.h>

using namespace DirectX;

static int CheckAndThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
		throw new _com_error(hr);
}

class DirectDevicesBox
{
private:
	D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;

	ID3D11Device*           pd3dDevice;				//	���������� Direct3D
	ID3D11DeviceContext*    pImmediateContext;		//	�������� ����������
	IDXGISwapChain*         pSwapChain;				//	���� �����

public:
	//	����������� ��������������� ������
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	//	�����������
	DirectDevicesBox(DXGI_SWAP_CHAIN_DESC* SwapChainDesc, UINT createDeviceFlags,
		D3D11_PRIMITIVE_TOPOLOGY Topology);
	//	���������� ID3D11Device
	ID3D11Device* GetDevice();
	//	���������� SwapChain
	IDXGISwapChain* GetSwapChain();
	//	���������� Context ����������
	ID3D11DeviceContext* GetDeviceContext();
	//	���������� driverType
	D3D_DRIVER_TYPE GetDriverType();
	//	���������� featureLevel
	D3D_FEATURE_LEVEL GetFeatureLevel();
	//	����������
	~DirectDevicesBox();
};

