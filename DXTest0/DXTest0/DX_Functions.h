#pragma once

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>


using namespace DirectX;

// Вершина
struct SimpleVertex
{
	XMFLOAT3 Pos;
};

// Инициализация устройств DirectX
HRESULT InitDevice(HWND);
// Удаление созданнных устройств DirectX
void CleanupDevice();
// Функция рисования
void Render(); 
// Инициализация шаблона ввода и буфера вершин 
HRESULT InitGeometry();
