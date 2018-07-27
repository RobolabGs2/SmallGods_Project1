#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

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
