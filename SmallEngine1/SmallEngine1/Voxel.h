#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"


class  Voxel
{
private:
	Direct3Dbox*	pDXbox;		//	�������� Direct3Dbox
	PhysicalBox*	pPhBox;		//	������������ PhysicalBox
	Voxel *			pNext;		//	������ �� ���������
	Voxel*			pPrev;		//	������ �� ����������
	Float3*			location;	//	���������� ������������ �������
	Vertex*			vertices;
	WORD*			indices;
public:	

	//	����������� �� ���������, ������ ������ ������ �� ������� �� ������ ����
	Voxel();
	//	���������� �����������, ����� ��� 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	��������� ������� ����� ������. ���������� ������ �� ����
	Voxel* AddPrev(Voxel* pVoxel);
	//	���� ���� �������� ������
	 void Tick(DWORD dt);
	//	����������
	~ Voxel();

	//	������))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};