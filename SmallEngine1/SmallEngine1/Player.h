#pragma once
#include "Voxel.h"
#include "Camera.h"
class Player :
	public Voxel
{
public:
	Player(Camera* camera);
	~Player();
	void Tick(DWORD);
private:
	Camera * camera;
};

