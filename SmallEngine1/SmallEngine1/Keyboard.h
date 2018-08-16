#pragma once
#include <windows.h>
#include <functional>
typedef std::function<void(DWORD)> KeyboardAction;
#include <map>
#include "eKeyCode.h"
enum class key_state {up = 0, down = 256};
template <class TKeyAction>
class Keyboard
{
protected:
	// Привязки виртуальных кнопок к физическим
	std::map<TKeyAction, eKeyCodes> bind_keys_;
	// Для зажатых кнопок
	std::map<TKeyAction, KeyboardAction> key_down_actions_;
	// Для отжатых кнопок
	std::map<TKeyAction, KeyboardAction> key_up_actions_;
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
	void BindAction(TKeyAction, KeyboardAction, key_state state = key_state::down);
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
	bind_keys_[key] = code;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::AddBindKey(std::map<TKeyAction, eKeyCodes> binds)
{
	for (auto p = binds.begin(); p != binds.end(); ++p)
	{
		bind_keys_.insert(*p);
	}
}

template <class TKeyAction>
void Keyboard<TKeyAction>::SetBindKey(std::map<TKeyAction, eKeyCodes> buttons)
{
	bind_keys_ = buttons;
}

template <class TKeyAction>
void Keyboard<TKeyAction>::BindAction(TKeyAction key, KeyboardAction action, key_state state)
{
	switch(state)
	{
	case key_state::up: 
		key_up_actions_[key] = action;
		break;
	case key_state::down: 
		key_down_actions_[key] = action;
		break;
	//default: 
		//todo бросить исключение;
	}
}

template <class TKeyAction>
void Keyboard<TKeyAction>::Tick(DWORD dt)
{
	for (auto p = key_down_actions_.begin(); p != key_down_actions_.end(); ++p)
	{
		if (GetFocus() && dt)
		{
			bool pressed = false;
			if (bind_keys_[p->first] > eKeyCodes::KEY_MAX)
				pressed = mouse->GetStatusKey(ConvertKeyCode(bind_keys_[p->first]));
			else
				pressed = GetKeyState(static_cast<int>(bind_keys_[p->first])) & button_pressed;
			if (pressed)
				(p->second)(dt);
		}
	}
	for (auto p = key_up_actions_.begin(); p != key_up_actions_.end(); ++p)
	{
		if (GetFocus() && dt)
		{
			bool pressed = false;
			if (bind_keys_[p->first] > eKeyCodes::KEY_MAX)
				pressed = mouse->GetStatusKey(ConvertKeyCode(bind_keys_[p->first]));
			else
				pressed = GetKeyState(static_cast<int>(bind_keys_[p->first])) & button_pressed;
			if (!pressed)
				(p->second)(dt);
		}
	}
}
