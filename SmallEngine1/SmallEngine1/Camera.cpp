#include "stdafx.h"

#include "Camera.h"

Camera::Camera()
{
	OriginalRotation = XMVectorSet(1, 0, 0, 0);
	/*
	Eye = XMVectorSet(0.0f, 4.0f, -11.0f, 0.0f);  // Откуда смотрим
	At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);    // Куда смотрим
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);    // Направление верха
	*/
}


XMMATRIX Camera::GetView()
{
	XMVECTOR location = Owner->GetLocation();
	XMVECTOR Eye = location;// Откуда смотрим
	XMVECTOR At = XMVector3Transform(OriginalRotation, Owner->GetRotation()) + location;// Куда смотрим
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// Направление верха
	
	return XMMatrixLookAtLH(Eye, At, Up);
}


void Camera::SetOwner(Voxel* Owner)
{
	this->Owner = Owner;
}