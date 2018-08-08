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
	
	pDXbox->Show();
}

void PhysicalBox::AddObject(Voxel* voxel)
{
	pMainVox->AddPrev(voxel);

}

void PhysicalBox::GenerateVoxels(Direct3Dbox* pDXBox)
{
	std::vector<XMVECTOR> vertices =
	{
		XMVectorSet(0.0f,  1.5f,  0.0f, 0.0f),
		XMVectorSet(-1.0f,  0.0f, -1.0f, 0.0f),
		XMVectorSet(1.0f,  0.0f, -1.0f, 0.0f),
		XMVectorSet(-1.0f,  0.0f,  1.0f, 0.0f),
		XMVectorSet(1.0f,  0.0f,  1.0f, 0.0f),
	};
	std::vector<WORD> indices =
	{
		0,3,4,
		2,1,0,
		0,1,3,
		0,4,2,
		3,1,2,
		3,2,4,
	};

	AddObject(new Voxel(NULL, NULL, pDXBox, this));

	Voxel* pyramid = new Voxel(NULL, NULL, pDXBox, this, vertices, indices, XMFLOAT3(0, 3, 0));
	pyramid->Mound(2);
	pyramid->RecalculateImage();
	AddObject(pyramid);


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