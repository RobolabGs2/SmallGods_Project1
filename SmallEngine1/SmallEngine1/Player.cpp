#include "stdafx.h"
#include "Player.h"
#include "Keyboard.h"

Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, Keyboard* keyboard):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	EditLocation(XMVectorSet(0, 0, -11, 0));
	EditRotation(XMMatrixRotationY(-3.14159265358979/2));

	keyboard->AddAction(eKeyAction::TURN_RIGHT, [&](DWORD dt) {
		EditRotation(XMMatrixRotationY((dt *speed_rotation)));
	});
	keyboard->AddAction(eKeyAction::TURN_LEFT, [&](DWORD dt) {
		EditRotation(XMMatrixRotationY((dt *-speed_rotation)));
	});
	keyboard->AddAction(eKeyAction::MOVE_FORWARD, [&](DWORD dt) {
		EditLocation(dt*GetVectorCourse()*speed);
	});
	keyboard->AddAction(eKeyAction::MOVE_BACKWARD, [&](DWORD dt) {
		EditLocation(dt*GetVectorCourse()*(-speed));
	});
	keyboard->AddAction(eKeyAction::MOVE_RIGHT, [&](DWORD dt) {
		EditLocationXZ(dt * XMVector3Transform(GetVectorCourse(), XMMatrixRotationY(3.1415926 / 2))*speed);
	});
	keyboard->AddAction(eKeyAction::MOVE_LEFT, [&](DWORD dt) {
		EditLocationXZ(dt * XMVector3Transform(GetVectorCourse(), XMMatrixRotationY(3.1415926 / 2))*-speed);
	});
	keyboard->AddAction(eKeyAction::TURN_UP, [&](DWORD dt) {
		EditRotation(XMMatrixRotationAxis(
			XMVector3Cross(XMVectorSet(0, 1, 0, 0),
				GetVectorCourse()), dt *-speed_rotation));
	});
	keyboard->AddAction(eKeyAction::TURN_DOWN, [&](DWORD dt) {
		EditRotation(XMMatrixRotationAxis(
			XMVector3Cross(XMVectorSet(0, 1, 0, 0),
				GetVectorCourse()), dt *speed_rotation));
	});
};



Player::~Player()
{
}

void Player::Tick(DWORD dt)
{
}