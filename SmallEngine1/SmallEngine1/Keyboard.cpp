#include "stdafx.h"
#include "Keyboard.h"

eMouseKey ConvertKeyCode(eKeyCodes key) {
	switch (key)
	{
	case eKeyCodes::KEY_MOUSE_LEFT: return eMouseKey::LEFT;
	case eKeyCodes::KEY_MOUSE_RIGHT: return eMouseKey::RIGHT;
	case eKeyCodes::KEY_MOUSE_FORWARD: return eMouseKey::UP;
	case eKeyCodes::KEY_MOUSE_BACKWARD: return eMouseKey::DOWN;
	default: throw "Ошибка перевода из клавиатуры в мышь";
	}
}

Keyboard::Keyboard(Mouse* mouse)
{
	this->mouse = mouse;
	Buttons = {
		{ eKeyAction::MOVE_LEFT, eKeyCodes::KEY_A },
		{ eKeyAction::MOVE_RIGHT, eKeyCodes::KEY_D },
		{ eKeyAction::MOVE_FORWARD, eKeyCodes::KEY_W },
		{ eKeyAction::MOVE_BACKWARD, eKeyCodes::KEY_S },
		{ eKeyAction::TURN_LEFT, eKeyCodes::KEY_MOUSE_LEFT},
		{ eKeyAction::TURN_RIGHT, eKeyCodes::KEY_MOUSE_RIGHT },
		{ eKeyAction::TURN_UP, eKeyCodes::KEY_MOUSE_FORWARD },
		{ eKeyAction::TURN_DOWN, eKeyCodes::KEY_MOUSE_BACKWARD }
	};
}


Keyboard::~Keyboard()
{
}

void Keyboard::Tick(DWORD dt)
{
	mouse->Tick(dt);
	for(auto p = Actions.begin(); p!=Actions.end(); ++p)
	{
		if(GetFocus()&&dt)
		{
			bool will_do = false;
			if (Buttons[p->first] > eKeyCodes::KEY_MAX)
				will_do = mouse->GetStatusKey(ConvertKeyCode(Buttons[p->first]));
			else
				will_do = GetKeyState(Buttons[p->first]) & 256;
			if (will_do)
				(Actions[p->first])(dt);
		}
		
	}

}
void Keyboard::AddAction(eKeyAction key, KeyAction action)
{
	Actions[key] = action;

}
