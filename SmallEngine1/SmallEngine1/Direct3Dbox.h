#pragma once

class Direct3Dbox;
struct Vertex;
enum PixelShaderType;
struct ConstantBuffer;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#include "WinAPIInit.h"
#include "Voxel.h"
#include "Camera.h"
#include "DirectDevicesBox.h"


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
	DirectDevicesBox*		pDevicesBox;			//	��������� � ���������

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
	//	����������� ��� ������� � ��������� �������������
	LPCSTR ShaderTypeToLPCSTR(PixelShaderType stype);
public:
	//	�����������
	Direct3Dbox(WinAPIInit* pWinInit, WCHAR* ShadersFileName, Camera*);
	//	������ ������ �� ������ �����
	void Draw(Voxel* pVoxel);
	//	���������� ��������� �� DirectDevicesBox
	DirectDevicesBox* GetDevicesBox();
	//	������������ � ������� ������ ����� � ����� �������
	void Show();
	//	����������	
	~Direct3Dbox();
};