#pragma once

#include <windows.h>
#include <d3d11.h>


// ������������� ��������� DirectX
HRESULT InitDevice(HWND);
// �������� ���������� ��������� DirectX
void CleanupDevice();
// ������� ���������
void Render();     
