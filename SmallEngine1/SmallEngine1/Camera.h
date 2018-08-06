#pragma once

class Camera;

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

public:
	Camera()
	{
		Eye = XMVectorSet(0.0f, 4.0f, -11.0f, 0.0f);  // ������ �������
		At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);    // ���� �������
		Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);    // ����������� �����
	}
	XMMATRIX GetView()
	{
		return XMMatrixLookAtLH(Eye, At, Up);
	}

	void SetAt(XMVECTOR v)
	{
		At = v;
	}
	void SetEye(XMVECTOR v)
	{
		Eye = v;
	}
};