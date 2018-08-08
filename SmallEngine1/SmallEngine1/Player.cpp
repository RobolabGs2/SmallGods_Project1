#include "stdafx.h"
#include "Player.h"


Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	location = XMFLOAT4(0, 0, -11, 0);
	Rotation = XMMatrixRotationX(0);
};



Player::~Player()
{
}


void Player::Tick(DWORD dt)
{
	if(GetKeyState(0x44)&256)
		Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	if (GetKeyState(0x41) & 256)
		Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(-(dt / 1000.0f)));
}
/*
void Player::Tick(DWORD dt)
{
	
	//
	location.x += dt * 0.001f;
	location.z += dt * 0.0005f;
	camera->SetEye(XMVectorSet(location.x, sin(location.y)+5, cos(location.z)+-11, 0));
	
}*/
