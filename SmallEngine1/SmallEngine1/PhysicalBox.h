#pragma once

class PhysicalBox;

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>
#include <vector>
#include "Direct3Dbox.h"
#include "DirectDevicesBox.h"
#include "Voxel.h"


class PhysicalBox
{
private:
	DirectDevicesBox *		pDevicesBox;			//	��������� � ���������

	ID3D11ComputeShader*	pPhysicalParamsShader;	//	������, ��������� ���������� ��������� �������

	Direct3Dbox*			pDXbox;					//	������ �� ������ DirectX
	Voxel*					pMainVox;				//	������ �� ������-������
public:
	//	�����������
	PhysicalBox(Direct3Dbox* pDXbox, WCHAR* ShadersFileName);
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


