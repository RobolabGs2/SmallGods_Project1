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

	vertices =
	{
		XMVectorSet(0.0f,  -1,  1, 0.0f),
		XMVectorSet(0.0f,  0.0f, -1, 0.0f),
		XMVectorSet(-1 ,  0.0f,  1, 0.0f),
		XMVectorSet(1 ,  0.0f,  1, 0.0f),
	};

	indices =
	{
		1,2,3,
		0,2,1,
		0,3,2,
		0,1,3,
	};

	acceleration = XMVectorSet(0, 0, 0, 0);
	speed = XMVectorSet(0, 0, 0, 0);
	location = XMVectorSet(0, 0, 0, 0);
	Rotation = XMMatrixRotationX(0);
	RecalculatePhisicalParams();
	RecalculateImage();

}
Voxel::Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox,
	std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMVECTOR location)
{
	this->pNext = pNext;
	this->pPrev = pPrev;
	this->pDXbox = pDXbox;
	this->pPhBox = pPhBox;

	this->vertices = vertices;
	this->indices = indices;
	this->location = location;

	acceleration = XMVectorSet(0, 0, 0, 0);
	speed = XMVectorSet(0, 0, 0, 0);
	Rotation = XMMatrixRotationZ(0.0001);
	RecalculatePhisicalParams();
	RecalculateImage();

	if(volume < 15)
		speed = XMVectorSet(-0.3, 0, 0, 0);
	else
		speed = XMVectorSet(0.3, 0, 0, 0);
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
	Voxel* pointer = pNext;
	while (pointer != pPhBox->GetVoxelsQueueEnd())
	{
		XMVECTOR colPoint;
		if (GetCollisionPoint(pointer, &colPoint))
		{
			speed = XMVectorSet(0, 0, 0, 0);
			pointer->speed = XMVectorSet(0, 0, 0, 0);
		}
		pointer = pointer->pNext;
	}
	//acceleration += XMVectorSet(0, -9.8, 0, 0) ;
	pDXbox->Draw(this);
}


void Voxel::Move(DWORD dt)
{
	speed += acceleration * dt / 1000;
	location += speed * dt / 1000;
	acceleration = XMVectorSet(0, 0, 0, 0);
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
		//	Пытался не запутаться, но, кажется, всё стало ещё хуже
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
	O = O / volume;
	this->volume = volume;

	for (int i = 0; i < vertices.size(); i++)
		vertices[i] -= O;

	location += O;

}



bool Voxel::GetCollisionPoint(Voxel* pTarget, XMVECTOR* pPoint)
{
#define VOX_SIGN_FUNC(x, z) (Bt * (Av * (x) + Cv * (z) + Dv) - Bv * (At * (x) + Ct * (z) + Dt))
#define VOX_CHECK_INTERSECTION(signva, signvb, signta, signtb, p1, p2)	\
	if(((signva < 0) != (signvb < 0)) && (signta < 0) != (signtb < 0))	\
	{																	\
		float kzl = (p2.x - p1.x) / (p2.z - p1.z);						\
		float kzf = (Bv * Ct - Bt * Cv) / (Bt * Av - Bv * At);			\
		float kf = (Bv * Dt - Bt * Dv) / (Bt * Av - Bv * At);			\
		float z = (p1.z * kzl + kf - p1.x) / (kzl - kzf);				\
		float x = z * kzf + kf;											\
		float y = -((Av * x + Cv * z + Dv) / Bv);						\
		*pPoint = XMVectorSet(x, y, z, 0);								\
		return true;													\
	}

	XMMATRIX Worldv = GetMatrixWorld();

	std::vector<XMFLOAT3>	vv = std::vector<XMFLOAT3>(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		XMStoreFloat3(&(vv[i]), XMVector3Transform(vertices[i], Worldv));

	XMMATRIX Worldt = pTarget->GetMatrixWorld();
	std::vector<XMFLOAT3>	vt = std::vector<XMFLOAT3>(pTarget->vertices.size());
	for (int i = 0; i < pTarget->vertices.size(); i++)
		XMStoreFloat3(&(vt[i]), XMVector3Transform(pTarget->vertices[i], Worldt));

	for (int iv = 0; iv < indices.size(); iv += 3)
	{

		XMFLOAT3 pv0 = vv[indices[iv + 0]];
		XMFLOAT3 pv1 = vv[indices[iv + 1]];
		XMFLOAT3 pv2 = vv[indices[iv + 2]];

		float Av = (pv1.y - pv0.y) * (pv2.z - pv0.z) - (pv1.z - pv0.z) * (pv2.y - pv0.y);
		float Bv = (pv1.z - pv0.z) * (pv2.x - pv0.x) - (pv1.x - pv0.x) * (pv2.z - pv0.z);
		float Cv = (pv1.x - pv0.x) * (pv2.y - pv0.y) - (pv1.y - pv0.y) * (pv2.x - pv0.x);
		float Dv = - (pv0.x * Av + pv0.y * Bv + pv0.z * Cv);

		for (int it = 0; it < pTarget->indices.size(); it += 3)
		{
			XMFLOAT3 pt0 = vt[pTarget->indices[it + 0]];
			XMFLOAT3 pt1 = vt[pTarget->indices[it + 1]];
			XMFLOAT3 pt2 = vt[pTarget->indices[it + 2]];

			float At = (pt1.y - pt0.y) * (pt2.z - pt0.z) - (pt1.z - pt0.z) * (pt2.y - pt0.y);
			float Bt = (pt1.z - pt0.z) * (pt2.x - pt0.x) - (pt1.x - pt0.x) * (pt2.z - pt0.z);
			float Ct = (pt1.x - pt0.x) * (pt2.y - pt0.y) - (pt1.y - pt0.y) * (pt2.x - pt0.x);
			float Dt = -(pt0.x * At + pt0.y * Bt + pt0.z * Ct);

			float signv0 = VOX_SIGN_FUNC(pv0.x, pv0.z);
			float signv1 = VOX_SIGN_FUNC(pv1.x, pv1.z);
			float signv2 = VOX_SIGN_FUNC(pv2.x, pv2.z);
			
			float signt0 = VOX_SIGN_FUNC(pt0.x, pt0.z);
			float signt1 = VOX_SIGN_FUNC(pt1.x, pt1.z);
			float signt2 = VOX_SIGN_FUNC(pt2.x, pt2.z);

			VOX_CHECK_INTERSECTION(signv0, signv1, signt0, signt1, pv0, pv1);
			VOX_CHECK_INTERSECTION(signv0, signv1, signt0, signt2, pv0, pv1);
			VOX_CHECK_INTERSECTION(signv0, signv2, signt0, signt1, pv0, pv2);
			VOX_CHECK_INTERSECTION(signv0, signv2, signt0, signt2, pv0, pv2);

		}
	}

	return false;
#undef VOX_SIGN_FUNC
#undef VOX_CHECK_INTERSECTION
}

Voxel::~Voxel()
{
}

XMMATRIX Voxel::GetMatrixWorld()
{
	return XMMatrixMultiply(Rotation, XMMatrixTranslationFromVector(location));
}

XMMATRIX Voxel::GetRotation()
{
	return Rotation;;
}


XMVECTOR Voxel::GetLocation()
{
	return location;
}


XMVECTOR Voxel::GetVectorCourse()
{
	return XMVector3Transform(XMVectorSet(1, 0, 0, 0), this->GetRotation());
}

void Voxel::EditLocation(XMVECTOR addVector)
{
	location += addVector;
}
void Voxel::EditLocationXZ(XMVECTOR addVector)
{
	addVector *= XMVectorSet(1, 0, 1, 0);
	location += addVector;
}
void Voxel::EditLocationY(XMVECTOR addVector)
{
	addVector *= XMVectorSet(0, 1, 0, 0);
	location += addVector;
}

void Voxel::EditRotation(XMMATRIX addRotation)
{
	Rotation = XMMatrixMultiply(Rotation, addRotation);
}