#pragma once

class PhysicalBox;

#include "Direct3Dbox.h"
#include "Voxel.h"


class PhysicalBox
{
private:
	Direct3Dbox*	pDXbox;		//	Ссылка на обёртку DirectX
	Voxel*			pMainVox;	//	Ссылка на объект-стопор
public:
	//	Конструктор
	PhysicalBox(Direct3Dbox* pDXbox);
	//	Удаляет воксель. Совсем. Как будто его и не было.
	void RemoveVoxel(Voxel* pVoxel);
	//	Один такт просчёта физики
	void Tick(DWORD dt);
	//	Создаёт объект и добавляет на сцену
	void AddObject();
	//	Деструктор
	~PhysicalBox();
};


