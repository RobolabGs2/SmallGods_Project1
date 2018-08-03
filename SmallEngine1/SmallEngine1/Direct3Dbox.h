#pragma once

class Direct3Dbox;
struct Vertex;
enum PixelShaderType;
struct ConstantBuffer;
class  Float3;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "WinAPIInit.h"
#include "Voxel.h"
#include <comdef.h>


using namespace DirectX;


enum PixelShaderType {SHT_BASE, SHT_LIGHT};
const UINT PixelShaderTypeLength = 2;

//	Вершина
struct Vertex
{
	XMFLOAT3 Pos;		//	Позиция вершины
	XMFLOAT3 Normal;	//	Нормаль
};

class  Float3
{
public:
	Float3(float x_inp, float y_inp, float z_inp) :
		x(x_inp), y(y_inp), z(z_inp) {};
	float x;
	float y;
	float z;
};

// Константный буфер
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vOutputColor;
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
	ID3D11VertexShader*     pVertexShader;			//	Вершинный шейдер
	ID3D11PixelShader**     pPixelShader;			//	Массив пиксельных шейдеров
	ID3D11InputLayout*      pVertexLayout;			//	Описание формата вершин
	ID3D11Buffer*           pConstantBuffer;		//	Константный буфер
	XMMATRIX                Projection;				//	Матрица проекции
	XMMATRIX                View;					//	Матрица вида
	//	Бросает исключение если hr не S_OK
	void Exp(HRESULT hr);
	//	Преобразует тип шейдера в строковое представление
	LPCSTR ShaderTypeToLPCSTR(PixelShaderType stype);
	//	Компилирует соответствующий шейдер
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
public:
	//	Конструктор
	Direct3Dbox(WinAPIInit* pWinInit, WCHAR* szFileName);
	//	Рисует объект на задний буфер
	void Draw(Voxel* pVoxel);
	//	Отрисовывает и очищает задний буфер и буфер глубины
	void Show();
	//	Деструктор	
	~Direct3Dbox();
};