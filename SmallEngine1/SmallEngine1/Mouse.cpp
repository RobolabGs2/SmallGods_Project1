#include "stdafx.h"
#include "Mouse.h"


void Mouse::refresh_delta()
{
	// Находим центр окна
	RECT r_window;
	GetWindowRect(window, &r_window);
	const auto width = r_window.right - r_window.left;
	const auto height = r_window.bottom - r_window.top;

	const auto c_x = r_window.left + width / 2;
	const auto c_y = r_window.top + height / 2;

	POINT pM;
	GetCursorPos(&pM);
	float x = static_cast<float>(pM.x - c_x)/width;
	float y = static_cast<float>(pM.y - c_y) / height;
	dx = x;
	dy = y;
	SetCursorPos(c_x, c_y);

	// Не даём выйти курсору за пределы окна
	ClipCursor(&r_window);
}

Mouse::Mouse(HWND window)
{
	this->window = window;
	cursor_.off();
}


Mouse::~Mouse()
{
}

void Mouse::Tick(DWORD dt)
{
	if (dt) {
		refresh_delta();
		if (bind_action_move)
			bind_action_move(dt, dx, dy);
		if (bind_action_scroll)
			bind_action_scroll(dt, scroll_);
		scroll_ = 0;
	}
}

void Mouse::SetBindActionMove(MouseActionMove action)
{
	bind_action_move = action;
}

void Mouse::SetBindActionScroll(MouseActionScroll action)
{
	bind_action_scroll = action;
}

void Mouse::SetCursorState(CursorState state)
{
	cursor_.SetState(state);
}

void Mouse::ReadScrollMessage(MSG msg)
{
	scroll_ += (static_cast<short>HIWORD(msg.wParam)) / WHEEL_DELTA;
}

bool Mouse::GetStatusKey(eMouseKey key)
{
	bool flag = false;
	switch (key)
	{
	case eMouseKey::LEFT:
		if (dx < -sensitivity)
		{
			flag = true;
			//dx = 0;
		}
		break;
	case eMouseKey::RIGHT:
		if (dx > sensitivity)
		{
			flag = true;
			//dx = 0;
		}
		break;
	case eMouseKey::UP:
		if (dy < -sensitivity)
		{
			flag = true;
			//dy = 0;
		}
		break;
	case eMouseKey::DOWN:
		if (dy > sensitivity)
		{
			flag = true;
			//dy = 0;
		}
		break;
	}
	return flag;
}
