#include "stdafx.h"
#include "Keyboard.h"



/*template<class TKeyAction> 
Keyboard<TKeyAction>::Keyboard(Mouse* mouse)
{
	this->mouse = mouse;
	/*Buttons = {
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


template<class TKeyAction>
Keyboard<TKeyAction>::~Keyboard()
{
}

template<class TKeyAction>
void Keyboard<TKeyAction>::Tick(DWORD dt)
{
	mouse->Tick(dt);
	for(auto p = Actions.begin(); p!=Actions.end(); ++p)
	{
		if(GetFocus()&&dt)
		{
			bool will_do = false;
			if (*Buttons[p->first] > eKeyCodes::KEY_MAX)
				will_do = mouse->GetStatusKey(ConvertKeyCode(*Buttons[p->first]));
			else
				will_do = GetKeyState(*Buttons[p->first]) & 256;
			if (will_do)
				(Actions[p->first])(dt);
		}
		
	}

}

template <class TKeyAction>
void Keyboard<TKeyAction>::BindKey(TKeyAction key, eKeyCodes code)
{
	*Buttons[key] = code;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::BindKey(std::map<TKeyAction, eKeyCodes>* buttons)
{
	Buttons = buttons;
}

template<class TKeyAction>
void Keyboard<TKeyAction>::BindAction(TKeyAction key, KeyboardAction action)
{
	Actions[key] = action;
}*/
