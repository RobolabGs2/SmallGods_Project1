#pragma once

class Direct3Dbox;
struct Vertex;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "WinAPIInit.h"
#include "Voxel.h"
#include <comdef.h>


using namespace DirectX;

//	Вершина
struct Vertex
{
	XMFLOAT3 Pos;		//	Позиция вершины
	XMFLOAT3 Normal;	//	Нормаль
};

//	Класс - обёртка над Direct3D
class Direct3Dbox
{
private:
	D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	ID3D11Device*           pd3dDevice;				//	Устройство Direct3D
	ID3D11DeviceContext*    pImmediateContext;		//	Контекст устройство
	IDXGISwapChain*         pSwapChain;				//	Цепь связи
	ID3D11RenderTargetView* pRenderTargetView;		//	Поверхность рисования заднего буфера
	ID3D11Texture2D*        pDepthStencil;			//	Текстура буфера глубин
	WinAPIInit*				pWinInit;				//	Объект она для вывода
	ID3D11DepthStencilView* pDepthStencilView;		//	Объект вида, буфер глубин
	
	//	Бросает исключение если hr не S_OK
	void Exp(HRESULT hr);
public:
	//	Конструктор
	Direct3Dbox(WinAPIInit* pWinInit);
	//	Рисует объект на задний буфер
	void Draw(Voxel* pVoxel);
	//	Отрисовывает и очищает задний буфер и буфер глубины
	void Show();
	//	Деструктор	
	~Direct3Dbox();
};