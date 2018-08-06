#include "stdafx.h"
#include "Player.h"


Player::Player(Camera* camera)
{
	this->camera = camera;
};



Player::~Player()
{
}

void Player::Tick(DWORD dt)
{
	camera->SetEye(XMVectorSet(0.0f, 4.0f, -11.0f-dt, 0.0f));
}
