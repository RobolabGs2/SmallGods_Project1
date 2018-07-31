#pragma once

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#define MX_SETWORLD 0x101

using namespace DirectX;

// �������
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;       // ������� �������
};

// ����������� �����
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection; 
	XMFLOAT4 vLightDir[2]; // ����������� �����
	XMFLOAT4 vLightColor[2];      // ���� ���������
	XMFLOAT4 vOutputColor; // �������� ���� (��� ������� PSSolid)
};

// ������������� ��������� DirectX
HRESULT InitDevice(HWND);
// �������� ���������� ��������� DirectX
void CleanupDevice();
// ������� ���������
void Render(); 
// ������������� ������� ����� � ������ ������ 
HRESULT InitGeometry();
// ������������� ������
HRESULT InitMatrixes(HWND);
// ���������� ���������� �����
void UpdateLight();
// ���������� ������� ����
void UpdateMatrix(UINT nLightIndex); 