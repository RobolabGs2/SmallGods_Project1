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

	ID3D11Device*           pd3dDevice;				//	Устройство Direct3D
	ID3D11DeviceContext*    pImmediateContext;		//	Контекст устройство
	IDXGISwapChain*         pSwapChain;				//	Цепь связи

public:
	//	Компилирует соответствующий шейдер
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	//	Конструктор
	DirectDevicesBox(DXGI_SWAP_CHAIN_DESC* SwapChainDesc, UINT createDeviceFlags,
		D3D11_PRIMITIVE_TOPOLOGY Topology);
	//	Возвращает ID3D11Device
	ID3D11Device* GetDevice();
	//	Возвращает SwapChain
	IDXGISwapChain* GetSwapChain();
	//	Возвращает Context устройство
	ID3D11DeviceContext* GetDeviceContext();
	//	Возвращает driverType
	D3D_DRIVER_TYPE GetDriverType();
	//	Возвращает featureLevel
	D3D_FEATURE_LEVEL GetFeatureLevel();
	//	Деструктор
	~DirectDevicesBox();
};

