#pragma once
enum class eKeyAction;
class Player;

#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <DirectXMath.h>
#include <vector>
#include "Keyboard.h"
#include "Voxel.h"

using namespace DirectX;

class Player :
	public Voxel
{
private:
	float speed = 0.01f;
	float speed_rotation = 0.0009f;
public:
	Player(Voxel* pNext, Voxel* pPrev, Direct3Dbox* pDXbox, PhysicalBox* pPhBox, Keyboard<eKeyAction>* keyboard);
	~Player();
	void Tick(DWORD);
};

enum class eKeyAction
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_FORWARD,
	MOVE_BACKWARD,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_DOWN,
	TURN_UP
};
