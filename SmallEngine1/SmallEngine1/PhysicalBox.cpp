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

void PhysicalBox::AddObject()
{
	pMainVox->AddPrev(new Voxel(NULL, NULL, pDXbox, this));
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