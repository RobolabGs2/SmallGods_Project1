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
	XMFLOAT3 location = Owner->GetLocation();
	XMVECTOR Eye = XMLoadFloat3(&location);// ������ �������
	XMVECTOR At = XMVectorAdd(XMVector3Transform(OriginalRotation, Owner->GetRotation()),XMLoadFloat3(&location));// ���� �������
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// ����������� �����
	
	return XMMatrixLookAtLH(Eye, At, Up);
}


void Camera::SetOwner(Voxel* Owner)
{
	this->Owner = Owner;
}