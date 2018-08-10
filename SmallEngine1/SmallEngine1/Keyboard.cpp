#include "stdafx.h"
#include "Keyboard.h"


Keyboard::Keyboard()
{
	Buttons = {
		{ eKeyAction::MOVE_LEFT, eKeyCodes::KEY_A },
		{ eKeyAction::MOVE_RIGHT, eKeyCodes::KEY_D },
		{ eKeyAction::MOVE_FORWARD, eKeyCodes::KEY_W },
		{ eKeyAction::MOVE_BACKWARD, eKeyCodes::KEY_S },
		{ eKeyAction::TURN_LEFT, eKeyCodes::KEY_LEFT},
		{ eKeyAction::TURN_RIGHT, eKeyCodes::KEY_RIGHT},
		{ eKeyAction::TURN_UP, eKeyCodes::KEY_UP },
		{ eKeyAction::TURN_DOWN, eKeyCodes::KEY_DOWN }
	};
}


Keyboard::~Keyboard()
{
}

void Keyboard::Tick(DWORD dt)
{
	for(auto p = Actions.begin(); p!=Actions.end(); ++p)
	{
		if (GetKeyState(Buttons[p->first]) & 256)
			(Actions[p->first])(dt);
	}

}
void Keyboard::AddAction(eKeyAction key, Action action)
{
	Actions[key] = action;

}
