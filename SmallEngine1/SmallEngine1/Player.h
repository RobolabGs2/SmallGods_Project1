#pragma once
#include "InputDevices.h"
#include "eKeyAction.h"
class Player;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <DirectXMath.h>
#include "Voxel.h"

using namespace DirectX;

class Player :
	public Voxel
{
private:
	float speed = 0.01f;
	float speed_rotation = 0.09f;
public:
	Player(Voxel* pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, InputDevices<eKeyAction>* input_devices);
	~Player();
	void Tick(DWORD) override;
};