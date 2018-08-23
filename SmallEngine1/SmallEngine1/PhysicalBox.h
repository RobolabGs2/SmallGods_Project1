#pragma once

class PhysicalBox;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#include "Direct3Dbox.h"
#include "DirectDevicesBox.h"
#include "Voxel.h"


class PhysicalBox
{
private:
	DirectDevicesBox *		pDevicesBox;			//	Коробочка с девайсами

	ID3D11ComputeShader*	pPhysicalParamsShader;	//	Шейдер, считающий физические параметры вокселя

	Direct3Dbox*			pDXbox;					//	Ссылка на обёртку DirectX
	Voxel*					pMainVox;				//	Ссылка на объект-стопор
public:
	//	Конструктор
	PhysicalBox(Direct3Dbox* pDXbox, WCHAR* ShadersFileName);
	//	Удаляет воксель. Совсем. Как будто его и не было.
	void RemoveVoxel(Voxel* pVoxel);
	//	Один такт просчёта физики
	void Tick(DWORD dt);
	//	Метод, который должен был называться GenerateWorld, но Слава сказал, что
	//	вдруг у нас будет объект World, тогда получится путаница, так что вот.
	void GenerateVoxels(Direct3Dbox* pDXBox);
	//	Возвращает указатель на воксель - стопор
	Voxel* GetVoxelsQueueEnd();
	//  Добавляет объект на сцену
	void AddObject(Voxel*);
	//	Деструктор
	~PhysicalBox();
};


