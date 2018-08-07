#include "stdafx.h"
#include "Player.h"


Player::Player(Camera* camera)
{
	this->camera = camera;
	location = XMFLOAT3(0, 5, -11);
	Rotation = XMMatrixRotationX(0);
};



Player::~Player()
{
}

void Player::Tick(DWORD dt)
{
	//Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	location.x += dt * 0.001f;
	location.z += dt * 0.0005f;
	camera->SetEye(XMVectorSet(location.x, sin(location.y)+5, cos(location.z)+-11, 0));
}
