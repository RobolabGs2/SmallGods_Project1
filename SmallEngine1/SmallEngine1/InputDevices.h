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
	CursorState cursor_state_ = CursorState::Hide;
public:
	InputDevices(HWND);
	~InputDevices();
	void Tick(DWORD);
	Keyboard<TKeyAction>* GetKeyboard();
	void SetMouseAction(MouseActionMove);
	void SetMouseAction(MouseActionScroll);
	void SetCursorState(CursorState);
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
	if (GetFocus()) {
		mouse_.Tick(dt);
		keyboard_.Tick(dt);
		mouse_.SetCursorState(cursor_state_);
	} else
		mouse_.SetCursorState(CursorState::Show);
}

template <class TKeyAction>
Keyboard<TKeyAction>* InputDevices<TKeyAction>::GetKeyboard()
{
	return &keyboard_;
}

template <class TKeyAction>
void InputDevices<TKeyAction>::SetMouseAction(MouseActionMove action)
{
	mouse_.SetBindActionMove(action);
}

template <class TKeyAction>
void InputDevices<TKeyAction>::SetMouseAction(MouseActionScroll action)
{
	mouse_.SetBindActionScroll(action);
}

template <class TKeyAction>
void InputDevices<TKeyAction>::SetCursorState(CursorState cursor_state)
{
	cursor_state_ = cursor_state;
}
