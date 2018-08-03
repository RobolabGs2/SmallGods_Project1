#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"


class  Voxel
{
private:
	Direct3Dbox*	pDXbox;		//	основной Direct3Dbox
	PhysicalBox*	pPhBox;		//	родительский PhysicalBox
	Voxel *			pNext;		//	Ссылка на следующий
	Voxel*			pPrev;		//	Ссылка на предыдущий
	Float3*			location;	//	Координаты расположения объекта
	Vertex*			vertices;
	WORD*			indices;
public:	

	//	Конструктор по умолчанию, создаёт пустой объект со ссылкой на самого себя
	Voxel();
	//	Нормальный конструктор, юзаем его 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	Добавляет воксель перед данным. Возвращает ссылку на него
	Voxel* AddPrev(Voxel* pVoxel);
	//	Один такт просчёта физики
	 void Tick(DWORD dt);
	//	Деструктор
	~ Voxel();

	//	Друзья))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};