#pragma once

class Direct3Dbox;
struct Vertex;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "WinAPIInit.h"
#include "Voxel.h"
#include <comdef.h>


using namespace DirectX;

//	�������
struct Vertex
{
	XMFLOAT3 Pos;		//	������� �������
	XMFLOAT3 Normal;	//	�������
};

//	����� - ������ ��� Direct3D
class Direct3Dbox
{
private:
	D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	ID3D11Device*           pd3dDevice;				//	���������� Direct3D
	ID3D11DeviceContext*    pImmediateContext;		//	�������� ����������
	IDXGISwapChain*         pSwapChain;				//	���� �����
	ID3D11RenderTargetView* pRenderTargetView;		//	����������� ��������� ������� ������
	ID3D11Texture2D*        pDepthStencil;			//	�������� ������ ������
	WinAPIInit*				pWinInit;				//	������ ��� ��� ������
	ID3D11DepthStencilView* pDepthStencilView;		//	������ ����, ����� ������
	
	//	������� ���������� ���� hr �� S_OK
	void Exp(HRESULT hr);
public:
	//	�����������
	Direct3Dbox(WinAPIInit* pWinInit);
	//	������ ������ �� ������ �����
	void Draw(Voxel* pVoxel);
	//	������������ � ������� ������ ����� � ����� �������
	void Show();
	//	����������	
	~Direct3Dbox();
};