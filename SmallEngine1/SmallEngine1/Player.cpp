#include "stdafx.h"
#include "Player.h"
#include <map>
Player::Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, Keyboard<eKeyAction>* keyboard):Voxel(pNext, pPrev, pDXbox, pPhBox)
{

	EditLocation(XMVectorSet(0, 0, -11, 0));
	EditRotation(XMMatrixRotationY(-3.14159265358979/2));
	std::map<eKeyAction, eKeyCodes> binds= {
		{ eKeyAction::MOVE_LEFT, eKeyCodes::KEY_A },
	{ eKeyAction::MOVE_RIGHT, eKeyCodes::KEY_D },
	{ eKeyAction::MOVE_FORWARD, eKeyCodes::KEY_W },
	{ eKeyAction::MOVE_BACKWARD, eKeyCodes::KEY_S },
	{ eKeyAction::TURN_LEFT, eKeyCodes::KEY_MOUSE_LEFT },
	{ eKeyAction::TURN_RIGHT, eKeyCodes::KEY_MOUSE_RIGHT },
	{ eKeyAction::TURN_UP, eKeyCodes::KEY_MOUSE_FORWARD },
	{ eKeyAction::TURN_DOWN, eKeyCodes::KEY_MOUSE_BACKWARD }
	};
	keyboard->BindKey(binds);

	keyboard->BindAction(eKeyAction::TURN_RIGHT, [&](DWORD dt) {
		EditRotation(XMMatrixRotationY((dt *speed_rotation)));
	});
	keyboard->BindAction(eKeyAction::TURN_LEFT, [&](DWORD dt) {
		EditRotation(XMMatrixRotationY((dt *-speed_rotation)));
	});
	keyboard->BindAction(eKeyAction::MOVE_FORWARD, [&](DWORD dt) {
		EditLocation(dt*GetVectorCourse()*speed);
	});
	keyboard->BindAction(eKeyAction::MOVE_BACKWARD, [&](DWORD dt) {
		EditLocation(dt*GetVectorCourse()*(-speed));
	});
	keyboard->BindAction(eKeyAction::MOVE_RIGHT, [&](DWORD dt) {
		EditLocationXZ(dt * XMVector3Transform(GetVectorCourse(), XMMatrixRotationY(3.1415926 / 2))*speed);
	});
	keyboard->BindAction(eKeyAction::MOVE_LEFT, [&](DWORD dt) {
		EditLocationXZ(dt * XMVector3Transform(GetVectorCourse(), XMMatrixRotationY(3.1415926 / 2))*-speed);
	});
	keyboard->BindAction(eKeyAction::TURN_UP, [&](DWORD dt) {
		EditRotation(XMMatrixRotationAxis(
			XMVector3Cross(XMVectorSet(0, 1, 0, 0),
				GetVectorCourse()), dt *-speed_rotation));
	});
	keyboard->BindAction(eKeyAction::TURN_DOWN, [&](DWORD dt) {
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