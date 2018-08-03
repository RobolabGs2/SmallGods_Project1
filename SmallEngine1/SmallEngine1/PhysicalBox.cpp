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

PhysicalBox::~PhysicalBox()
{
} 