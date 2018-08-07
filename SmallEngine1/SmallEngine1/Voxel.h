#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class  Voxel
{
protected:
	Direct3Dbox*		pDXbox;		//	�������� Direct3Dbox
	PhysicalBox*		pPhBox;		//	������������ PhysicalBox
	Voxel*				pNext;		//	������ �� ���������
	Voxel*				pPrev;		//	������ �� ����������
	XMMATRIX			Rotation;	//	������� ��������
	XMFLOAT4			location;	//	���������� ������������ �������
	std::vector<Vertex>	vertices;	//	������ ������
	std::vector<WORD>	indices;	//	������ ��������
public:	

	//	����������� �� ���������, ������ ������ ������ �� ������� �� ������ ����
	Voxel();
	//	���������� �����������, ����� ��� 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	��������� ������� ����� ������. ���������� ������ �� ����
	Voxel* AddPrev(Voxel* pVoxel);
	//	���� ���� �������� ������
	virtual  void Tick(DWORD dt);
	//	����������
	~ Voxel();
	// ���������� ����������
	XMMATRIX GetMatrixWorld();
	//	���������� ������� ��������
	XMMATRIX GetRotation();
	//	���������� ���������� 
	XMFLOAT4 GetLocation();
	//	������))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};