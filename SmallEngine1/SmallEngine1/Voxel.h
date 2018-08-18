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
private:
	Direct3Dbox*			pDXbox;			//	основной Direct3Dbox
	PhysicalBox*			pPhBox;			//	родительский PhysicalBox
	Voxel*					pNext;			//	Ссылка на следующий
	Voxel*					pPrev;			//	Ссылка на предыдущий
	XMMATRIX				Rotation;		//	Матрица поворота
	XMVECTOR				location;		//	Координаты расположения объекта
	float					volume;			//	Объём меша
	XMVECTOR				speed;			//	Скорость
	XMVECTOR				acceleration;	//	Усеорние объекта
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
		std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMVECTOR location);
	//	Добавляет воксель перед данным. Возвращает ссылку на него
	Voxel* AddPrev(Voxel* pVoxel);
	//	Один такт просчёта физики
	virtual  void Tick(DWORD dt);
	//	Перемещает воксель, меняет скорость и обнуляет ускорение
	void Move(DWORD dt);
	//	Пересчитывает массивы для отображения
	void RecalculateImage();
	//	Рекурсивно генерирует неровности поверхности
	void Mound(int depth, float factor);
	//	Обновляет физические параметры (объём и центр масс)
	void RecalculatePhisicalParams();
	//	Если объект пересекается с данным, возвращает true. 
	//	В pPoint заносит координату точки где-то на границе пересечения 
	bool GetCollisionPoint(Voxel* pTarget, XMVECTOR* pPoint);
	//	Деструктор
	virtual ~ Voxel();
	// Абсолютные координаты
	XMMATRIX GetMatrixWorld();
	//	Возвращает матрицу поворота
	XMMATRIX GetRotation();
	//	Возвращает координату 
	XMVECTOR GetLocation();
	XMVECTOR GetVectorCourse();
	void EditLocation(XMVECTOR addVector);
	void EditLocationXZ(XMVECTOR addVector);
	void EditLocationY(XMVECTOR addVector);
	void EditRotation(XMMATRIX addRotation);
	//	Друзья))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};