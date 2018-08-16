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

	AddObject(new Voxel(NULL, NULL, pDXBox, this));

	Voxel* pyramid = new Voxel(NULL, NULL, pDXBox, this, vertices, indices, XMVectorSet(2, 3, 0, 0));
	//pyramid->Mound(5, 0.2);
	//float volume = pyramid->CalculateVolume();
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