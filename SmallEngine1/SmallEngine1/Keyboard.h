#pragma once
#include <windows.h>
#include <functional>
typedef std::function<void(DWORD)> KeyboardAction;
#include <map>
#include "eKeyCode.h"

template <class TKeyAction>
class Keyboard
{
protected:
	std::map<TKeyAction, eKeyCodes> Buttons;
	std::map<TKeyAction, KeyboardAction> Actions;
	Mouse* mouse;
	static eMouseKey ConvertKeyCode(eKeyCodes);
	const int button_pressed = 256;
public:
	Keyboard(Mouse*);
	~Keyboard();
	// Привязать физическую клавишу к виртуальной
	void AddBindKey(TKeyAction, eKeyCodes);
	void AddBindKey(std::map<TKeyAction, eKeyCodes>);
	void SetBindKey(std::map<TKeyAction, eKeyCodes>);
	// Привязать действие к виртуальной клавише
	void BindAction(TKeyAction, KeyboardAction);
	void Tick(DWORD);
};

template <class TKeyAction>
eMouseKey Keyboard<TKeyAction>::ConvertKeyCode(eKeyCodes key)
{
	switch (key)
	{
	case eKeyCodes::KEY_MOUSE_LEFT: return eMouseKey::LEFT;
	case eKeyCodes::KEY_MOUSE_RIGHT: return eMouseKey::RIGHT;
	case eKeyCodes::KEY_MOUSE_FORWARD: return eMouseKey::UP;
	case eKeyCodes::KEY_MOUSE_BACKWARD: return eMouseKey::DOWN;
	default: throw "Ошибка перевода из клавиатуры в мышь";
	}
}

template <class TKeyAction>
Keyboard<TKeyAction>::Keyboard(Mouse* mouse)
{
	this->mouse = mouse;
}

template <class TKeyAction>
Keyboard<TKeyAction>::~Keyboard()
{
}

template <class TKeyAction>
void Keyboard<TKeyAction>::AddBindKey(TKeyAction key, eKeyCodes code)
{
	Buttons[key] = code;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::AddBindKey(std::map<TKeyAction, eKeyCodes> binds)
{
	for (auto p = binds.begin(); p != binds.end(); ++p)
	{
		Buttons.insert(*p);
	}
}

template <class TKeyAction>
void Keyboard<TKeyAction>::SetBindKey(std::map<TKeyAction, eKeyCodes> buttons)
{
	Buttons = buttons;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::BindAction(TKeyAction key, KeyboardAction action)
{
	Actions[key] = action;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::Tick(DWORD dt)
{
	mouse->Tick(dt);
	for (auto p = Actions.begin(); p != Actions.end(); ++p)
	{
		if (GetFocus() && dt)
		{
			bool will_do = false;
			if (Buttons[p->first] > eKeyCodes::KEY_MAX)
				will_do = mouse->GetStatusKey(ConvertKeyCode(Buttons[p->first]));
			else
				will_do = GetKeyState(static_cast<int>(Buttons[p->first])) & button_pressed;
			if (will_do)
				(Actions[p->first])(dt);
		}
	}
}
