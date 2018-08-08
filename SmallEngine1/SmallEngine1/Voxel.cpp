#include "stdafx.h"

#include "Voxel.h"

Voxel::Voxel()
{
	pPrev = this;
	pNext = this;
}

Voxel::Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox)
{
	this->pNext = pNext;
	this->pPrev = pPrev;
	this->pDXbox = pDXbox;
	this->pPhBox = pPhBox;
	
	//	ѕока делаем кубик, потом надо сделат какой-нибудь генератор
	img_vertices =
	{  /* координаты X, Y, Z                          нормаль X, Y, Z     */
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 1.0f),		XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -2.0f, -1.0f),	XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -2.0f, -1.0f),		XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -2.0f, 1.0f),		XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -2.0f, 1.0f),		XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -2.0f, 1.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -2.0f, -1.0f),	XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 1.0f),		XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -2.0f, 1.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -2.0f, -1.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 1.0f),		XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -2.0f, -1.0f),	XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -2.0f, -1.0f),		XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, -1.0f),		XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, -1.0f),		XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -2.0f, 1.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -2.0f, 1.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 2.0f, 1.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 2.0f, 1.0f),		XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};
	
	img_indices =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};

	location = XMFLOAT3(0, 0, 0);

	Rotation = XMMatrixRotationX(0);
}
Voxel::Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox,
	std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMFLOAT3 location)
{
	this->pNext = pNext;
	this->pPrev = pPrev;
	this->pDXbox = pDXbox;
	this->pPhBox = pPhBox;

	this->vertices = vertices;
	this->indices = indices;
	this->location = location;

	Rotation = XMMatrixRotationX(0);
	RecalculateImage();
}

Voxel* Voxel::AddPrev(Voxel* pVoxel)
{
	pVoxel->pPrev = pPrev;
	pVoxel->pNext = this;
	pVoxel->pPrev->pNext = pVoxel;
	pVoxel->pNext->pPrev = pVoxel;
	return pVoxel;
}

void Voxel::Tick(DWORD dt)
{
	
	/*
		ћј√»я (физика)
		ћј√»я (физика)
		ћј√»я (физика)
		ћј√»я (физика)
	*/
	Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	pDXbox->Draw(this);
}

void Voxel::RecalculateImage()
{
	img_vertices = std::vector<Vertex>(indices.size());
	img_indices = std::vector<WORD>(indices.size() );

	for (int i = 0; i < indices.size(); i += 3)
	{
		XMVECTOR v0 = vertices[(indices[i + 0])];
		XMVECTOR v1 = vertices[(indices[i + 1])];
		XMVECTOR v2 = vertices[(indices[i + 2])];

		XMFLOAT3 normal;
		XMStoreFloat3(&normal, XMVector3Normalize(
			XMVector3Cross(XMVectorSubtract(v0, v1), XMVectorSubtract(v0, v2))));

		XMFLOAT3 f0;
		XMFLOAT3 f1;
		XMFLOAT3 f2;

		XMStoreFloat3(&f0, v0);
		XMStoreFloat3(&f1, v1);
		XMStoreFloat3(&f2, v2);

		img_vertices[i + 0] = { f0, normal };
		img_vertices[i + 1] = { f1, normal };
		img_vertices[i + 2] = { f2, normal };

		img_indices[i + 0] = i + 0;
		img_indices[i + 1] = i + 1;
		img_indices[i + 2] = i + 2;

	}
}


void Voxel::Mound(int depth)
{
	if (depth <= 0)
		return;

	std::vector<WORD> newIndices = std::vector<WORD>(indices.size() * 3);

	for (int i = 0; i < indices.size(); i += 3)
	{
		XMVECTOR v = XMVectorScale(XMVectorAdd(XMVectorAdd(vertices[indices[i]], vertices[indices[i + 1]]), 
			 vertices[indices[i + 2]]), 1.0f / 3.0f);
		v = XMVectorAdd(v, XMVectorSet(0.0, 0.2, 0.0, 0.0));
		WORD index = vertices.size();
		vertices.push_back(v);

		newIndices[i * 3 + 0] = index;
		newIndices[i * 3 + 1] = indices[i + 1];
		newIndices[i * 3 + 2] = indices[i + 2];

		newIndices[i * 3 + 3] = indices[i + 0];
		newIndices[i * 3 + 4] = index;
		newIndices[i * 3 + 5] = indices[i + 2];

		newIndices[i * 3 + 6] = indices[i + 0];
		newIndices[i * 3 + 7] = indices[i + 1];
		newIndices[i * 3 + 8] = index;
	}

	indices = newIndices;
	Mound(depth - 1);
}

Voxel::~Voxel()
{
}

XMMATRIX Voxel::GetMatrixWorld()
{
	return XMMatrixMultiply(Rotation, XMMatrixTranslation(location.x, location.y, location.z));
}

XMMATRIX Voxel::GetRotation()
{
	return Rotation;;
}


XMFLOAT3 Voxel::GetLocation()
{
	return location;
}