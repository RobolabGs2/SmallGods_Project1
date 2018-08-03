#pragma once

class PhysicalBox;

#include "Direct3Dbox.h"
#include "Voxel.h"


class PhysicalBox
{
private:
	Direct3Dbox*	pDXbox;		//	������ �� ������ DirectX
	Voxel*			pMainVox;	//	������ �� ������-������
public:
	//	�����������
	PhysicalBox(Direct3Dbox* pDXbox);
	//	���� ���� �������� ������
	void Tick(DWORD dt);
	//	����������
	~PhysicalBox();
};


