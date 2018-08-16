#pragma once

class  Voxel;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include "CustomVectorMath.h"
#include <DirectXMath.h>
#include <stdlib.h>
#include <vector>

using namespace DirectX;

class  Voxel
{
private:
	Direct3Dbox*			pDXbox;			//	�������� Direct3Dbox
	PhysicalBox*			pPhBox;			//	������������ PhysicalBox
	Voxel*					pNext;			//	������ �� ���������
	Voxel*					pPrev;			//	������ �� ����������
	XMMATRIX				Rotation;		//	������� ��������
	XMFLOAT3				location;		//	���������� ������������ �������
	float					volume;			//	����� ����
	std::vector<Vertex>		img_vertices;	//	������ ������
	std::vector<XMVECTOR>	vertices;		//	������ ������
	std::vector<WORD>		indices;		//	������ ��������
public:	

	//	����������� �� ���������, ������ ������ ������ �� ������� �� ������ ����
	Voxel();
	//	����������� � ��������� �����(������) 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox);
	//	����������� � ���������� �����	 
	Voxel(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox,
		std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMFLOAT3 location);
	//	��������� ������� ����� ������. ���������� ������ �� ����
	Voxel* AddPrev(Voxel* pVoxel);
	//	���� ���� �������� ������
	virtual  void Tick(DWORD dt);
	//	������������� ������� ��� �����������
	void RecalculateImage();
	//	���������� ���������� ���������� �����������
	void Mound(int depth, float factor);
	//	��������� ���������� ��������� (����� � ����� ����)
	void RecalculatePhisicalParams();
	//	����������
	virtual ~ Voxel();
	// ���������� ����������
	XMMATRIX GetMatrixWorld();
	//	���������� ������� ��������
	XMMATRIX GetRotation();
	//	���������� ���������� 
	XMFLOAT3 GetLocation();
	XMVECTOR GetVectorCourse();
	void EditLocation(XMVECTOR addVector);
	void EditLocationXZ(XMVECTOR addVector);
	void EditLocationY(XMVECTOR addVector);
	void EditRotation(XMMATRIX addRotation);
	//	������))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};