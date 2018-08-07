#include "stdafx.h"
#include "Player.h"


Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	location = XMFLOAT4(0, 5, -11, 0);
	Rotation = XMMatrixRotationX(0);
};



Player::~Player()
{
}


/*
void Player::Tick(DWORD dt)
{
	
	//Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	location.x += dt * 0.001f;
	location.z += dt * 0.0005f;
	camera->SetEye(XMVectorSet(location.x, sin(location.y)+5, cos(location.z)+-11, 0));
	
}*/
