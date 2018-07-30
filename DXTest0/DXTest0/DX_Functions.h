#pragma once

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>


using namespace DirectX;

// �������
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

// ����������� �����
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
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
// ���������� ������� ����
void SetMatrixes();        
