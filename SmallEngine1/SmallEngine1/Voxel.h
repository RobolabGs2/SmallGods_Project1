#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include "CustomVectorMath.h"
#include <DirectXMath.h>
#include <stdlib.h>
#include <vector>

using namespace DirectX;

class  Voxel
{
protected:
	Direct3Dbox*			pDXbox;			//	основной Direct3Dbox
	PhysicalBox*			pPhBox;			//	родительский PhysicalBox
	Voxel*					pNext;			//	Ссылка на следующий
	Voxel*					pPrev;			//	Ссылка на предыдущий
	XMMATRIX				Rotation;		//	Матрица поворота
	XMFLOAT3				location;		//	Координаты расположения объекта
	std::vector<Vertex>		img_vertices;	//	Массив вершин
	std::vector<XMVECTOR>	vertices;		//	Массив вершин
	std::vector<WORD>		indices;		//	Массив индексов
public:	

	//	Конструктор по умолчанию, создаёт пустой объект со ссылкой на самого себя
	Voxel();
	//	Конструктор с дефолтным мешем(тардис) 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	Конструктор с задаваемым мешем	 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox,
		std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMFLOAT3 location);
	//	Добавляет воксель перед данным. Возвращает ссылку на него
	Voxel* AddPrev(Voxel* pVoxel);
	//	Один такт просчёта физики
	virtual  void Tick(DWORD dt);
	//	Пересчитывает массивы для отображения
	void RecalculateImage();
	//	Рекурсивно генерирует неровности поверхности
	void Mound(int depth, float factor);
	//	Считает объём меша
	float CalculateVolume();
	//	Деструктор
	~ Voxel();
	// Абсолютные координаты
	XMMATRIX GetMatrixWorld();
	//	Возвращает матрицу поворота
	XMMATRIX GetRotation();
	//	Возвращает координату 
	XMFLOAT3 GetLocation();
	//	Друзья))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};