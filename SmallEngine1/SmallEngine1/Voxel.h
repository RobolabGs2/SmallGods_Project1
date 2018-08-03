#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"


class  Voxel
{
private:
	Direct3Dbox* pDXbox;	//	основной Direct3Dbox
	PhysicalBox* pPhBox;	//	родительский PhysicalBox

public:
	Voxel * pNext;		//	Ссылка на следующий
	Voxel* pPrev;		//	Ссылка на предыдущий
	

	//	Конструктор по умолчанию, создаёт пустой объект со ссылкой на самого себя
	Voxel();
	//	Нормальный конструктор, юзаем его 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	Один такт просчёта физики
	 void Tick(DWORD dt);
	//	Деструктор
	~ Voxel();
};