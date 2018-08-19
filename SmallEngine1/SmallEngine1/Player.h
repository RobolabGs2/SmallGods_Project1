#pragma once
#include "InputDevices.h"
enum class eKeyAction;
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
	void Tick(DWORD);
	void Move(DWORD dt);
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
