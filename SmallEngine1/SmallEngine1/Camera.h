#pragma once


class Camera;

#include <DirectXMath.h>
#include "Voxel.h"

using namespace DirectX;

class Camera
{
private:

	XMVECTOR	OriginalRotation;
	Voxel*		Owner;


public:
	Camera();

	//	������������� ���������
	void SetOwner(Voxel* Owner);
	XMMATRIX GetView();

};
