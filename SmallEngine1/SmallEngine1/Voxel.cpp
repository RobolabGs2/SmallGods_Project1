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
	vertices =
	{
		XMVectorSet(-1.0f, -2.0f, -1.0f, 0.0f),
		XMVectorSet(1.0f, -2.0f, -1.0f, 0.0f),
		XMVectorSet(1.0f, 2.0f, -1.0f, 0.0f),
		XMVectorSet(-1.0f, 2.0f, -1.0f, 0.0f),

		XMVectorSet(-1.0f, -2.0f, 1.0f, 0.0f),
		XMVectorSet(1.0f, -2.0f, 1.0f, 0.0f),
		XMVectorSet(1.0f, 2.0f, 1.0f, 0.0f),
		XMVectorSet(-1.0f, 2.0f, 1.0f, 0.0f),
	};
	
	indices =
	{
		0,3,2,
		0,2,1,
		1,2,6,
		1,6,5,
		5,6,7,
		5,7,4,
		4,7,3,
		4,3,0,
		3,7,6,
		3,6,2,
		4,0,1,
		4,1,5,
	};

	location = XMFLOAT3(0, 0, 0);
	Rotation = XMMatrixRotationX(0);
	RecalculatePhisicalParams();

	RecalculateImage();

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
	RecalculatePhisicalParams();
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
	if(volume < 15)
		Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	pDXbox->Draw(this);
}

void Voxel::RecalculateImage()
{
	img_vertices = std::vector<Vertex>(indices.size());

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
	}
}


void Voxel::Mound(int depth, float factor)
{
	if (depth <= 0)
		return;

	std::vector<WORD> newIndices = std::vector<WORD>(0);

	float multiplier = factor / (float)RAND_MAX;

	for (int i = 0; i < indices.size(); i += 3)
	{
		XMVECTOR v = XMVectorScale(XMVectorAdd(XMVectorAdd(vertices[indices[i]], vertices[indices[i + 1]]),
			vertices[indices[i + 2]]), 1.0f / 3.0f);

		XMVECTOR a = XMVectorSubtract((vertices[indices[i + 1]]), (vertices[indices[i]]));
		XMVECTOR b = XMVectorSubtract((vertices[indices[i + 2]]), (vertices[indices[i]]));
		if(CVMSinAngle(a, b) > 0.6)
		{
			float area = sqrt(CVMArea(a, b));
			v = XMVectorAdd(v, area * (std::rand() ) * multiplier * XMVector3Normalize(XMVector3Cross(a, b)));
			/*
			v = XMVectorAdd(v, XMVectorSet(area * std::rand() * multiplier,
				area * std::rand() * multiplier, area * std::rand() * multiplier, 0.0));*/

			WORD index = vertices.size();
			vertices.push_back(v);

			newIndices.push_back(index);
			newIndices.push_back(indices[i + 1]);
			newIndices.push_back(indices[i + 2]);

			newIndices.push_back(indices[i + 0]);
			newIndices.push_back(index);
			newIndices.push_back(indices[i + 2]);

			newIndices.push_back(indices[i + 0]);
			newIndices.push_back(indices[i + 1]);
			newIndices.push_back(index);
		}
		else
		{
			newIndices.push_back(indices[i + 0]);
			newIndices.push_back(indices[i + 1]);
			newIndices.push_back(indices[i + 2]);
		}
	}

	indices = newIndices;
	Mound(depth - 1, factor);
}


void Voxel::RecalculatePhisicalParams()
{
	std::vector<XMFLOAT3>	fv = std::vector<XMFLOAT3>(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		XMStoreFloat3(&(fv[i]), vertices[i]);

	float volume = 0;

	XMVECTOR O = XMVectorSet(0, 0, 0, 0);
	XMVECTOR mult = XMVectorSet(1, 0, 1, 1);

	for (int i = 0; i < indices.size(); i += 3)
	{
		float h0 = (fv[indices[i + 0]].y) / 3;
		float h01 = (fv[indices[i + 0]].y + fv[indices[i + 1]].y) / 3;
		float h012 = (fv[indices[i + 0]].y + fv[indices[i + 1]].y + fv[indices[i + 2]].y) / 3;
		float s = (
			fv[indices[i + 2]].z * (fv[indices[i + 0]].x - fv[indices[i + 1]].x) +
			fv[indices[i + 1]].z * (fv[indices[i + 2]].x - fv[indices[i + 0]].x) +
			fv[indices[i + 0]].z * (fv[indices[i + 1]].x - fv[indices[i + 2]].x)) / 2;
		//	ѕыталс€ не запутатьс€, но, кажетс€, всЄ стало ещЄ хуже
		float v0 = s * h0;
		float v01 = s * h01;
		float v012 = s * h012;
		float v1 = v01 - v0;
		float v2 = v012 - v01;

		XMVECTOR A0 = vertices[indices[i + 0]];
		XMVECTOR A1 = vertices[indices[i + 1]];
		XMVECTOR A2 = vertices[indices[i + 2]];
		XMVECTOR B0 = A0 * mult;
		XMVECTOR B1 = A1 * mult;
		XMVECTOR B2 = A2 * mult;

		XMVECTOR O0 = (B0 + B1 + B2 + A0) / 4;
		XMVECTOR O1 = (B1 + B2 + A0 + A1) / 4;
		XMVECTOR O2 = (B2 + A0 + A1 + A2) / 4;

		O += O0 * v0 + O1 * v1 + O2 * v2;

		volume += v012;
	}
	O = - O / volume;
	this->volume = volume;

	for (int i = 0; i < vertices.size(); i++)
		vertices[i] += O;
	XMFLOAT3 Of3;
	XMStoreFloat3(&Of3, O);

	location.x -= Of3.x;
	location.y -= Of3.y;
	location.z -= Of3.z;

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
