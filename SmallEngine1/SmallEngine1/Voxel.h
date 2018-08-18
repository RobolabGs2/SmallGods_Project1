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
	XMVECTOR				location;		//	���������� ������������ �������
	float					volume;			//	����� ����
	XMVECTOR				speed;			//	��������
	XMVECTOR				acceleration;	//	�������� �������
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
		std::vector<XMVECTOR> vertices, std::vector<WORD> indices, XMVECTOR location);
	//	��������� ������� ����� ������. ���������� ������ �� ����
	Voxel* AddPrev(Voxel* pVoxel);
	//	���� ���� �������� ������
	virtual  void Tick(DWORD dt);
	//	���������� �������, ������ �������� � �������� ���������
	void Move(DWORD dt);
	//	������������� ������� ��� �����������
	void RecalculateImage();
	//	���������� ���������� ���������� �����������
	void Mound(int depth, float factor);
	//	��������� ���������� ��������� (����� � ����� ����)
	void RecalculatePhisicalParams();
	//	���� ������ ������������ � ������, ���������� true. 
	//	� pPoint ������� ���������� ����� ���-�� �� ������� ����������� 
	bool GetCollisionPoint(Voxel* pTarget, XMVECTOR* pPoint);
	//	����������
	virtual ~ Voxel();
	// ���������� ����������
	XMMATRIX GetMatrixWorld();
	//	���������� ������� ��������
	XMMATRIX GetRotation();
	//	���������� ���������� 
	XMVECTOR GetLocation();
	XMVECTOR GetVectorCourse();
	void EditLocation(XMVECTOR addVector);
	void EditLocationXZ(XMVECTOR addVector);
	void EditLocationY(XMVECTOR addVector);
	void EditRotation(XMMATRIX addRotation);
	//	������))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};