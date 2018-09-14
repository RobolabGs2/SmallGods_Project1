#include "stdafx.h"

#include "Voxel.h"

//	Возвращает объём области под линией
float PosVilumeUnderLine(XMFLOAT3 p1, XMFLOAT3 p2);
//	Возвращает объём области под квадратиком
float PosVilumeUnderRect(float x1, float x2, float za1, float za2, float zb1, float zb2, XMFLOAT4 ABCD);
//	Возвращает объём области под квадратиком
float PosVilumeUnderRect(float x1, float x2, float za1, float za2, float zb1, float zb2, float ha1, float ha2, float hb1, float hb2);
//	Сортирует массив f по координате x
inline void mySort(XMFLOAT3* f);
//	Возвращает true если a находится между x и y
inline bool aBetweenXY(float a, float x, float y);

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


	movable = true;

	force = XMVectorSet(0, 0, 0, 0);
	lineMomentum = XMVectorSet(0, 0, 0, 0);
	location = XMVectorSet(0, 0, 0, 0);

	forceMomentum = XMVectorSet(0, 0, 0, 0);
	angularMomentum = XMVectorSet(0, 0, 0, 0);
	Rotation = XMMatrixRotationZ(0);

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

	force = XMVectorSet(0, 0, 0, 0);
	lineMomentum = XMVectorSet(0, 0, 0, 0);
	location = XMVectorSet(0, 0, 0, 0);

	forceMomentum = XMVectorSet(0, 0, 0, 0);
	angularMomentum = XMVectorSet(0, 0, 0, 0);
	Rotation = XMMatrixRotationZ(0);

	movable = true;

	RecalculatePhisicalParams();
	RecalculateImage();

	if (volume < 15)
	{
		lineMomentum = XMVectorSet( -500, 0, 0, 0);
	}
	else
		lineMomentum = XMVectorSet( 0, 0, 0, 0);
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
		float col = GetCollisionPoint(pointer, &colPoint);
		if (abs(col) > 0.00001)
		{
 			XMVECTOR delForce = -XMVector3Normalize(lineMomentum - pointer->lineMomentum);
			//AppForce(colPoint, delForce );
			//pointer->AppForce(colPoint, -delForce);
			//location = colPoint;
		}
		pointer = pointer->pNext;
	}

	pDXbox->Draw(this);
}


void Voxel::Move(DWORD dt)
{
	if (!movable)
	{
		force = XMVectorSet(0, 0, 0, 0);
		forceMomentum = XMVectorSet(0, 0, 0, 0);
		return;
	}

	lineMomentum += force * dt / 1000.0f;
	location += lineMomentum * dt / 1000.0f / GetMass();
	force = XMVectorSet(0, 0, 0, 0);

	angularMomentum += forceMomentum * dt / 1000.0f;
	if(CVMAbs(angularMomentum) > 0)
		EditRotation(XMMatrixRotationAxis(angularMomentum, CVMAbs(angularMomentum) * dt / 1000.0f));
	forceMomentum = XMVectorSet(0, 0, 0, 0);
}

void Voxel::AppForce(XMVECTOR point, XMVECTOR addForce)
{
	XMVECTOR lineVector = location - point;
	XMVECTOR lineForce = XMVector3Normalize(lineVector)* CVMAbs(addForce) * CVMCosAngle(addForce, lineVector);
	XMVECTOR angularForce = addForce - lineForce;
	XMVECTOR momentumForce = XMVector3Normalize(XMVector3Cross(lineForce, angularForce)) * CVMAbs(angularForce);

	force += lineForce * 1000;
	forceMomentum += momentumForce  ;
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

XMVECTOR GetAbsQVolume(XMFLOAT3& a1, XMFLOAT3& a2, XMFLOAT3* ABC, XMFLOAT4* ABCD)
{
	XMFLOAT4 volume = { 0, 0, 0, 0 };

	a1.z = -(ABC->x * a1.x + ABC->z) / ABC->y;
	a2.z = -(ABC->x * a2.x + ABC->z) / ABC->y;

	volume.w = (
		(a1.x - a2.x) * (
			a2.z * (ABCD->x * (a1.x + 2 * a2.x) + ABCD->z * (a2.z) + 3 * ABCD->w) +
			a1.z * (ABCD->x * (2 * a1.x + a2.x) + ABCD->z * (a1.z + a2.z) + 3 * ABCD->w)
			) / ABCD->y) / 6;

	return XMLoadFloat4(&volume);
}

XMVECTOR GetCollision(XMFLOAT3 cmin, XMFLOAT3 cmax, XMFLOAT3* ABCa, XMFLOAT3* ABCb, XMFLOAT4* ABCDv, XMFLOAT4* ABCDt, XMFLOAT3& ABCvt)
{
	XMFLOAT3 pmina = { cmin.x, 0, -(ABCa->x * cmin.x + ABCa->z) / ABCa->y };
	XMFLOAT3 pminb = { cmin.x, 0, -(ABCb->x * cmin.x + ABCb->z) / ABCb->y };
	XMFLOAT3 pmaxa = { cmax.x, 0, -(ABCa->x * cmax.x + ABCa->z) / ABCa->y };
	XMFLOAT3 pmaxb = { cmax.x, 0, -(ABCb->x * cmax.x + ABCb->z) / ABCb->y };

	if (aBetweenXY(0, (pmina.x + pmaxa.x)* ABCvt.x + (pmina.z + pmaxa.z)* ABCvt.y + ABCvt.z, (pminb.x + pmaxb.x)* ABCvt.x + (pminb.z + pmaxb.z)* ABCvt.y + ABCvt.z))
		return XMVectorSet(0, 0, 0, 0);

	XMVECTOR volume = XMVectorSet(0, 0, 0, 0);

	volume += GetAbsQVolume(cmin, cmax, ABCa, ABCDv);
	volume -= GetAbsQVolume(cmin, cmax, ABCb, ABCDv);
	volume -= GetAbsQVolume(cmin, cmax, ABCa, ABCDt);
	volume += GetAbsQVolume(cmin, cmax, ABCb, ABCDt);

	XMFLOAT4 vol;
	XMStoreFloat4(&vol, volume);

	if (vol.w < 0)
		volume *= -1;
	if((ABCa->y * ABCb->y) < 0)
		volume *= -1;

	return volume;
}

XMVECTOR GetCollision2D(XMFLOAT3& cmin, XMFLOAT3& cmax, float* xPoints, unsigned int pointsCount, XMFLOAT3* ABCa, XMFLOAT3* ABCb, XMFLOAT4* ABCDv, XMFLOAT4* ABCDt, XMFLOAT3& ABCvt)
{
	if (pointsCount == 0)
		return GetCollision(cmin, cmax, ABCa, ABCb, ABCDv, ABCDt, ABCvt);

	XMVECTOR volume = XMVectorSet(0, 0, 0, 0);

	if (aBetweenXY(xPoints[0], cmin.x, cmax.x))
	{
		XMFLOAT3 betweenPoint = { xPoints[0], 0, 0};
		volume += GetCollision2D(cmin, betweenPoint, &(xPoints[1]), pointsCount - 1, ABCa, ABCb, ABCDv, ABCDt, ABCvt);
		volume += GetCollision2D(betweenPoint, cmax, &(xPoints[1]), pointsCount - 1, ABCa, ABCb, ABCDv, ABCDt, ABCvt);
	}
	else
		volume += GetCollision2D(cmin, cmax, &(xPoints[1]), pointsCount - 1, ABCa, ABCb, ABCDv, ABCDt, ABCvt);

	return volume;
}

XMVECTOR GetCollision3D(XMFLOAT3& a1, XMFLOAT3& a2, XMFLOAT3& b1, XMFLOAT3& b2, XMFLOAT3* ABCa, XMFLOAT3* ABCb, XMFLOAT4* ABCDv, XMFLOAT4* ABCDt)
{
	XMFLOAT3 ABCvt = {
		ABCDv->x * ABCDt->y - ABCDt->x * ABCDv->y,
		ABCDv->z * ABCDt->y - ABCDt->z * ABCDv->y,
		ABCDv->w * ABCDt->y - ABCDt->w * ABCDv->y };

	float xOvta = (ABCvt.z * ABCa->y + ABCvt.y * ABCa->z) / (ABCvt.y * ABCa->x + ABCvt.x * ABCa->y);
	float xOvtb = (ABCvt.z * ABCb->y + ABCvt.y * ABCb->z) / (ABCvt.y * ABCb->x + ABCvt.x * ABCb->y);
	float xO = (ABCa->z * ABCb->y + ABCa->y * ABCb->z) / (ABCa->y * ABCb->x + ABCa->x * ABCb->y);

	XMFLOAT3 array[] = { a1, a2, b1, b2 };
	mySort(array);
	XMFLOAT3 cmin = array[1];
	XMFLOAT3 cmax = array[2];

	float xPoints[] = { xOvta , xOvtb , xO };

	XMVECTOR volume = GetCollision2D(cmin, cmax, xPoints, 3, ABCa, ABCb, ABCDv, ABCDt, ABCvt);

	return volume;
}

float Voxel::GetCollisionPoint(Voxel* pTarget, XMVECTOR* pPoint)
{

	XMMATRIX Worldv = GetMatrixWorld();

	std::vector<XMFLOAT3>	vv = std::vector<XMFLOAT3>(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
		XMStoreFloat3(&(vv[i]), XMVector3Transform(vertices[i], Worldv));

	XMMATRIX Worldt = pTarget->GetMatrixWorld();
	std::vector<XMFLOAT3>	vt = std::vector<XMFLOAT3>(pTarget->vertices.size());
	for (int i = 0; i < pTarget->vertices.size(); i++)
		XMStoreFloat3(&(vt[i]), XMVector3Transform(pTarget->vertices[i], Worldt));

	XMVECTOR volume = XMVectorSet(0, 0, 0, 0);

	for (int iv = 0; iv < indices.size(); iv += 3)
	{

		XMFLOAT3 pv[3];
		for (int i = 0; i < 3; i++)
			pv[i] = vv[indices[iv + i]];

		float Av = (pv[1].y - pv[0].y) * (pv[2].z - pv[0].z) - (pv[1].z - pv[0].z) * (pv[2].y - pv[0].y);
		float Bv = (pv[1].z - pv[0].z) * (pv[2].x - pv[0].x) - (pv[1].x - pv[0].x) * (pv[2].z - pv[0].z);
		float Cv = (pv[1].x - pv[0].x) * (pv[2].y - pv[0].y) - (pv[1].y - pv[0].y) * (pv[2].x - pv[0].x);
		float Dv = - (pv[0].x * Av + pv[0].y * Bv + pv[0].z * Cv);

		XMFLOAT4 ABCDv = {Av, Bv, Cv, Dv};

		if (abs(Bv) <= 0.00001)
			continue;
		for (int it = 0; it < pTarget->indices.size(); it += 3)
		{
			XMFLOAT3 pt[3];
			for (int i = 0; i < 3; i++)
				pt[i] = vt[pTarget->indices[it + i]];

			float At = (pt[1].y - pt[0].y) * (pt[2].z - pt[0].z) - (pt[1].z - pt[0].z) * (pt[2].y - pt[0].y);
			float Bt = (pt[1].z - pt[0].z) * (pt[2].x - pt[0].x) - (pt[1].x - pt[0].x) * (pt[2].z - pt[0].z);
			float Ct = (pt[1].x - pt[0].x) * (pt[2].y - pt[0].y) - (pt[1].y - pt[0].y) * (pt[2].x - pt[0].x);
			float Dt = -(pt[0].x * At + pt[0].y * Bt + pt[0].z * Ct);

			XMFLOAT4 ABCDt = { At, Bt, Ct, Dt };

			if (abs(Bt) <= 0.00001)
				continue;

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
					int ii = (i + 1) % 3;
					int jj = (j + 1) % 3;
					XMFLOAT3 a1 = pt[i];
					XMFLOAT3 a2 = pt[ii];
					XMFLOAT3 b1 = pv[j];
					XMFLOAT3 b2 = pv[jj];

					float dx = abs(abs(a1.x - b1.x) + abs(a2.x - b2.x) - abs(a1.x - b2.x) - abs(a2.x - b1.x));

					if (dx <= 0.00001)
						continue;

					float Aa = a2.z - a1.z;
					float Ba = a1.x - a2.x;
					float Ca = -Aa * a1.x - Ba * a1.z;

					float Ab = b2.z - b1.z;
					float Bb = b1.x - b2.x;
					float Cb = -Ab * b1.x - Bb * b1.z;
					
					volume += GetCollision3D(a1, a2, b1, b2, &XMFLOAT3(Aa, Ba, Ca), &XMFLOAT3(Ab, Bb, Cb), &ABCDv, &ABCDt);
				}
		}
	}


	XMFLOAT4 collision;
	XMStoreFloat4(&collision, volume);
	float vol = collision.w;

	pPoint = &((volume *  XMVectorSet(1, 1, 1, 0)) / vol);
	return collision.w;
}

inline void swap(XMFLOAT3* a, XMFLOAT3* b)
{
	XMFLOAT3 c = *a;
	*a = *b;
	*b = c;
}

inline bool aBetweenXY(float a, float x, float y)
{
	return ((a >= x) && (a <= y)) || ((a <= x) && (a >= y));
}

inline void mySort(XMFLOAT3* f)
{
	for (int i = 0; i < 4; i++)
		for (int j = i + 1; j < 4; j++)
			if (f[i].x > f[j].x)
				swap(&f[i], &f[j]);
}

float PosVilumeUnderLine(XMFLOAT3 p1, XMFLOAT3 p2)
{
	float res = abs(p2.x - p1.x) * ((p2.y * p1.z + p1.y * p2.z) / 2 + p1.y * p1.z + p2.y * p2.z) / 3;
	return res;
}

float PosVilumeUnderRect(float x1, float x2, float za1, float za2, float zb1, float zb2, XMFLOAT4 ABCD)
{
	return abs(x2 - x1) * (
		(zb2 - za2) * (ABCD.x * (2 * x2 + x1) + ABCD.z * (za1 + za2 + zb2) + 3 * ABCD.w) +
		(zb1 - za1) * (ABCD.x * (2 * x1 + x2) + ABCD.z * (za1 + zb1 + zb2) + 3 * ABCD.w)) / 
		(6 * ABCD.y);
}

float PosVilumeUnderRect(float x1, float x2, float za1, float za2, float zb1, float zb2, float ha1, float ha2, float hb1, float hb2)
{
	return abs(x2 - x1) * (
		(zb2 - za2) * (ha1 + ha2 + hb2) +
		(zb1 - za1) * (ha1 + hb1 + hb2) ) / 6;
}


float Voxel::GetMass()
{
	return volume * 1000;
}

Voxel::~Voxel()
{
}


Voxel* Voxel::SetMovable(bool newValue)
{
	movable = newValue;
	return this;
}

XMMATRIX Voxel::GetMatrixWorld()
{
	return XMMatrixMultiply(GetRotation(), XMMatrixTranslationFromVector(location));
}

XMMATRIX Voxel::GetRotation()
{
	return Rotation;
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
	Rotation = XMMatrixMultiply(GetRotation(), addRotation);
}