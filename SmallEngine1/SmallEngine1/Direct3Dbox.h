#pragma once

class Direct3Dbox;
struct Vertex;
enum PixelShaderType;
struct ConstantBuffer;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "WinAPIInit.h"
#include "Voxel.h"
#include "Camera.h"
#include <comdef.h>
#include <vector>


using namespace DirectX;


enum PixelShaderType {SHT_BASE, SHT_LIGHT};
const UINT PixelShaderTypeLength = 2;

//	�������
struct Vertex
{
	XMFLOAT3 Pos;		//	������� �������
	XMFLOAT3 Normal;	//	�������
};

// ����������� �����
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vOutputColor;
	XMFLOAT3 sunPosition;
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
	ID3D11VertexShader*     pVertexShader;			//	��������� ������
	ID3D11PixelShader**     pPixelShader;			//	������ ���������� ��������
	ID3D11InputLayout*      pVertexLayout;			//	�������� ������� ������
	ID3D11Buffer*           pConstantBuffer;		//	����������� �����
	ConstantBuffer			cb;
	Camera*					camera;
	//	������� ���������� ���� hr �� S_OK
	void Exp(HRESULT hr);
	//	����������� ��� ������� � ��������� �������������
	LPCSTR ShaderTypeToLPCSTR(PixelShaderType stype);
	//	����������� ��������������� ������
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	//	�����������
	Direct3Dbox(WinAPIInit* pWinInit, WCHAR* szFileName, Camera*);
	//	������ ������ �� ������ �����
	void Draw(Voxel* pVoxel);
	//	������������ � ������� ������ ����� � ����� �������
	void Show();
	//	����������	
	~Direct3Dbox();
};