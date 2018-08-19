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
	float					volume;			//	����� ����

	XMVECTOR				forceMomentum;	//	������ ����
	XMVECTOR				angularMomentum;//	������ ��������
	XMMATRIX				Rotation;		//	������� ��������

	XMVECTOR				force;			//	�������� ����
	XMVECTOR				lineMomentum;	//	�������
	XMVECTOR				location;		//	���������� ������������

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
	virtual void Tick(DWORD dt);
	//	���������� �������, ������ �������� � �������� ���������
	virtual void Move(DWORD dt);
	//	������������� ���� � ������� � ������ �����
	void AppForce(XMVECTOR point, XMVECTOR force);
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
	//	���������� ��������� ������, ����������� �������� ����������� �������
	XMVECTOR GetVectorCourse();
	//	������� ����� �������
	float GetMass();
	//	�������� location �� addVector
	void EditLocation(XMVECTOR addVector);
	//	�������� Rotation �� addRotation
	void EditRotation(XMMATRIX addRotation);
	void EditLocationXZ(XMVECTOR addVector);
	void EditLocationY(XMVECTOR addVector);
	//	������))
	friend class Direct3Dbox;
	friend class PhysicalBox;
};