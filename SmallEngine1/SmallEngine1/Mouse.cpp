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
	ShowCursor(FALSE);
}


Mouse::~Mouse()
{
}

void Mouse::Tick(DWORD dt)
{
	if (GetFocus())
	{
		if (dt) {
			refresh_delta();
			if (bind_action)
				bind_action(dt, dx, dy);
		}
		ShowCursor(FALSE);
	}
	else
		ShowCursor(TRUE);
}

void Mouse::SetBindAction(MouseAction action)
{
	bind_action = action;
}

bool Mouse::GetStatusKey(eMouseKey key)
{
	bool flag = false;
	switch (key)
	{
	case LEFT:
		if (dx < -sensitivity)
		{
			flag = true;
			//dx = 0;
		}
		break;
	case RIGHT:
		if (dx > sensitivity)
		{
			flag = true;
			//dx = 0;
		}
		break;
	case UP:
		if (dy < -sensitivity)
		{
			flag = true;
			//dy = 0;
		}
		break;
	case DOWN:
		if (dy > sensitivity)
		{
			flag = true;
			//dy = 0;
		}
		break;
	}
	return flag;
}
