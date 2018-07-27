#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

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
