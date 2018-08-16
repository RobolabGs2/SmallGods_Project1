#pragma once
#include "Windows.h"
#include "Keyboard.h"
#include "Mouse.h"
template <class TKeyAction>
class InputDevices
{
private:
	Mouse mouse_;
	Keyboard<TKeyAction> keyboard_;
public:
	InputDevices(HWND);
	~InputDevices();
	void Tick(DWORD);
	Keyboard<TKeyAction>* GetKeyboard();
	void SetMouseAction(MouseAction);
};

template <class TKeyAction>
InputDevices<TKeyAction>::InputDevices(HWND hwnd): mouse_(hwnd), keyboard_(&mouse_)
{
}

template <class TKeyAction>
InputDevices<TKeyAction>::~InputDevices()
{
}

template <class TKeyAction>
void InputDevices<TKeyAction>::Tick(DWORD dt)
{
	mouse_.Tick(dt);
	keyboard_.Tick(dt);
}

template <class TKeyAction>
Keyboard<TKeyAction>* InputDevices<TKeyAction>::GetKeyboard()
{
	return &keyboard_;
}

template <class TKeyAction>
void InputDevices<TKeyAction>::SetMouseAction(MouseAction action)
{
	mouse_.SetBindAction(action);
}

