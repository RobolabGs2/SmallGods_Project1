#include "stdafx.h"
#include "PhysicalBox.h"

PhysicalBox::PhysicalBox(Direct3Dbox* pDXbox)
{
	this->pDXbox = pDXbox;
	pMainVox = new Voxel();
}


void PhysicalBox::Tick(DWORD dt)
{
	
	Voxel* pointer = pMainVox->pNext;
	while (pointer != pMainVox)
	{
		pointer->Tick(dt);
		pointer = pointer->pNext;
	}

	pointer = pointer->pNext;

	while (pointer != pMainVox)
	{
		pointer->Move(dt);
		pointer = pointer->pNext;
	}

	pDXbox->Show();
}

void PhysicalBox::AddObject(Voxel* voxel)
{
	pMainVox->AddPrev(voxel);

}

void PhysicalBox::GenerateVoxels(Direct3Dbox* pDXBox)
{
	float len = 1;

	std::vector<XMVECTOR> vertices =
	{
		XMVectorSet(0.0f,  -len,  len, 0.0f),
		XMVectorSet(0.0f,  0.0f, -len, 0.0f),
		XMVectorSet(-len ,  0.0f,  len, 0.0f),
		XMVectorSet(len ,  0.0f,  len, 0.0f),
	};

	std::vector<WORD> indices =
	{
		1,2,3,
		0,2,1,
		0,3,2,
		0,1,3,
	};

	std::vector<XMVECTOR> verticesTARDIS =
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

	std::vector<WORD> indicesTARDIS =
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

	Voxel* pyramid = new Voxel(NULL, NULL, pDXBox, this, vertices, indices, XMVectorSet(3, 0, 0, 0));
	Voxel* TARDIS = new Voxel(NULL, NULL, pDXBox, this, verticesTARDIS, indicesTARDIS, XMVectorSet(0, 0, 0, 0));
	AddObject(pyramid);
	AddObject(TARDIS);
}

Voxel* PhysicalBox::GetVoxelsQueueEnd()
{
	return pMainVox;
}

void PhysicalBox::RemoveVoxel(Voxel* pVoxel)
{
	pVoxel->pPrev->pNext = pVoxel->pNext;
	pVoxel->pNext->pPrev = pVoxel->pPrev;
	pVoxel->~Voxel();
}

PhysicalBox::~PhysicalBox()
{
} 