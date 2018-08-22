#pragma once

class PhysicalBox;

#include "Direct3Dbox.h"
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
	//	������� �������. ������. ��� ����� ��� � �� ����.
	void RemoveVoxel(Voxel* pVoxel);
	//	���� ���� �������� ������
	void Tick(DWORD dt);
	//	�����, ������� ������ ��� ���������� GenerateWorld, �� ����� ������, ���
	//	����� � ��� ����� ������ World, ����� ��������� ��������, ��� ��� ���.
	void GenerateVoxels(Direct3Dbox* pDXBox);
	//	���������� ��������� �� ������� - ������
	Voxel* GetVoxelsQueueEnd();
	//  ��������� ������ �� �����
	void AddObject(Voxel*);
	//	����������
	~PhysicalBox();
};


