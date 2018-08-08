#include "stdafx.h"
#include "Player.h"


Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	location = XMFLOAT3(0, 5, -11);
	Rotation = XMMatrixRotationY(-3.14159265358979/2 );
	Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationX(0.2));
};



Player::~Player()
{
}


void Player::Tick(DWORD dt)
{
	//Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
}
/*
void Player::Tick(DWORD dt)
{
	
	//
	location.x += dt * 0.001f;
	location.z += dt * 0.0005f;
	camera->SetEye(XMVectorSet(location.x, sin(location.y)+5, cos(location.z)+-11, 0));
	
}*/
