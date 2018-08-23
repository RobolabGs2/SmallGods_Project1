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

//	Вершина
struct Vertex
{
	XMFLOAT3 Pos;		//	Позиция вершины
	XMFLOAT3 Normal;	//	Нормаль
};

// Константный буфер
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vOutputColor;
	XMFLOAT3 sunPosition;
};

//	Класс - обёртка над Direct3D
class Direct3Dbox
{
private:
	DirectDevicesBox*		pDevicesBox;			//	Коробочка с девайсами

	ID3D11RenderTargetView* pRenderTargetView;		//	Поверхность рисования заднего буфера
	ID3D11Texture2D*        pDepthStencil;			//	Текстура буфера глубин
	WinAPIInit*				pWinInit;				//	Объект она для вывода
	ID3D11DepthStencilView* pDepthStencilView;		//	Объект вида, буфер глубин
	ID3D11VertexShader*     pVertexShader;			//	Вершинный шейдер
	ID3D11PixelShader**     pPixelShader;			//	Массив пиксельных шейдеров
	ID3D11InputLayout*      pVertexLayout;			//	Описание формата вершин
	ID3D11Buffer*           pConstantBuffer;		//	Константный буфер
	ConstantBuffer			cb;
	Camera*					camera;
	//	Преобразует тип шейдера в строковое представление
	LPCSTR ShaderTypeToLPCSTR(PixelShaderType stype);
public:
	//	Конструктор
	Direct3Dbox(WinAPIInit* pWinInit, WCHAR* ShadersFileName, Camera*);
	//	Рисует объект на задний буфер
	void Draw(Voxel* pVoxel);
	//	Возвращает указатель на DirectDevicesBox
	DirectDevicesBox* GetDevicesBox();
	//	Отрисовывает и очищает задний буфер и буфер глубины
	void Show();
	//	Деструктор	
	~Direct3Dbox();
};