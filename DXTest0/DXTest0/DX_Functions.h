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
};

// ������������� ��������� DirectX
HRESULT InitDevice(HWND);
// �������� ���������� ��������� DirectX
void CleanupDevice();
// ������� ���������
void Render(); 
// ������������� ������� ����� � ������ ������ 
HRESULT InitGeometry();
