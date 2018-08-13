#pragma once

class Keyboard;
class Player;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <DirectXMath.h>
#include <vector>
#include "Voxel.h"

using namespace DirectX;

class Player :
	public Voxel
{
public:
	Player(Voxel * pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, Keyboard* keyboard);
	~Player();
	void Tick(DWORD);
};

