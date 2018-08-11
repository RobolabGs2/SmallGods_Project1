#include "stdafx.h"
#include "Player.h"
#include "Keyboard.h"

Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, Keyboard* keyboard):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	location = XMFLOAT3(0, 5, -11);
	Rotation = XMMatrixRotationY(-3.14159265358979/2 );
	Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationX(0.2));

	keyboard->AddAction(eKeyAction::TURN_RIGHT, [&](DWORD dt) {
			Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / 1000.0f));
	});
	keyboard->AddAction(eKeyAction::TURN_LEFT, [&](DWORD dt) {
		Rotation = XMMatrixMultiply(Rotation, XMMatrixRotationY(dt / -1000.0f));
	});
	keyboard->AddAction(eKeyAction::MOVE_FORWARD, [&](DWORD dt) {
		location.z += dt * 0.01f;
	});
	keyboard->AddAction(eKeyAction::MOVE_BACKWARD, [&](DWORD dt) {
		location.z += dt * -0.01f;
	});
	keyboard->AddAction(eKeyAction::MOVE_RIGHT, [&](DWORD dt) {
		location.x += dt * 0.01f;
	});
	keyboard->AddAction(eKeyAction::MOVE_LEFT, [&](DWORD dt) {
		location.x += dt * -0.01f;
	});
	keyboard->AddAction(eKeyAction::TURN_UP, [&](DWORD dt) {
		location.y += dt * 0.01f;
	});
	keyboard->AddAction(eKeyAction::TURN_DOWN, [&](DWORD dt) {
		location.y += dt * -0.01f;
	});
};



Player::~Player()
{
}

void Player::Tick(DWORD dt)
{
}
/*
void Player::Tick(DWORD dt)
{
	
	//
	location.x += dt * 0.001f;
	location.z += dt * 0.0005f;
	camera->SetEye(XMVectorSet(location.x, sin(location.y)+5, cos(location.z)+-11, 0));
	
}*/
