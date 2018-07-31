#pragma once

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#define MX_SETWORLD 0x101

using namespace DirectX;

// Вершина
struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;       // Нормаль вершины
};

// Константный буфер
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection; 
	XMFLOAT4 vLightDir[2]; // Направление света
	XMFLOAT4 vLightColor[2];      // Цвет источника
	XMFLOAT4 vOutputColor; // Активный цвет (для второго PSSolid)
};

// Инициализация устройств DirectX
HRESULT InitDevice(HWND);
// Удаление созданнных устройств DirectX
void CleanupDevice();
// Функция рисования
void Render(); 
// Инициализация шаблона ввода и буфера вершин 
HRESULT InitGeometry();
// Инициализация матриц
HRESULT InitMatrixes(HWND);
// Обновление параметров света
void UpdateLight();
// Обновление матрицы мира
void UpdateMatrix(UINT nLightIndex); 