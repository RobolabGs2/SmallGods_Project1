#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"


class  Voxel
{
private:
	Direct3Dbox* pDXbox;	//	�������� Direct3Dbox
	PhysicalBox* pPhBox;	//	������������ PhysicalBox

public:
	Voxel * pNext;		//	������ �� ���������
	Voxel* pPrev;		//	������ �� ����������
	

	//	����������� �� ���������, ������ ������ ������ �� ������� �� ������ ����
	Voxel();
	//	���������� �����������, ����� ��� 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	���� ���� �������� ������
	 void Tick(DWORD dt);
	//	����������
	~ Voxel();
};