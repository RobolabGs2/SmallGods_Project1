#include "stdafx.h"

#include "Camera.h"

Camera::Camera()
{
	OriginalRotation = XMVectorSet(1, 0, 0, 0);
	/*
	Eye = XMVectorSet(0.0f, 4.0f, -11.0f, 0.0f);  // ������ �������
	At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);    // ���� �������
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);    // ����������� �����
	*/
}


XMMATRIX Camera::GetView()
{
	XMVECTOR location = Owner->GetLocation();
	XMVECTOR Eye = location;// ������ �������
	XMVECTOR At = XMVector3Transform(OriginalRotation, Owner->GetRotation()) + location;// ���� �������
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// ����������� �����
	
	return XMMatrixLookAtLH(Eye, At, Up);
}


void Camera::SetOwner(Voxel* Owner)
{
	this->Owner = Owner;
}