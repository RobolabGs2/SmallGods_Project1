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
	Camera()
	{
		this->Owner = Owner;
		OriginalRotation = XMVectorSet(1, 0, 0, 0);
		/*
		Eye = XMVectorSet(0.0f, 4.0f, -11.0f, 0.0f);  // Откуда смотрим
		At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);    // Куда смотрим
		Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);    // Направление верха
		*/
	}

	//	Устанавливает владельца
	void SetOwner(Voxel* Owner)
	{
		this->Owner = Owner;
	}

	XMMATRIX GetView()
	{
		XMFLOAT4 location = Owner->GetLocation();
		XMVECTOR Eye = XMLoadFloat4(&location);// Откуда смотрим
		XMVECTOR At = XMVectorAdd(XMVector3Transform(OriginalRotation, Owner->GetRotation),
			XMLoadFloat4(&location));// Куда смотрим
		XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// Направление верха

		return XMMatrixLookAtLH(Eye, At, Up);
	}

};
