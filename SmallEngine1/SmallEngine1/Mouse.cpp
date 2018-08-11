#include "stdafx.h"
#include "Mouse.h"


void Mouse::refresh_delta()
{
	// Находим центр окна
	RECT r_window;
	GetWindowRect(window, &r_window);
	int c_x = r_window.left + (r_window.right - r_window.left) / 2;
	int c_y = r_window.top + (r_window.bottom - r_window.top) / 2;

	POINT pM;
	GetCursorPos(&pM);
	dx += pM.x - c_x;
	dy += pM.y - c_y;

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
void Mouse::Tick(DWORD)
{
	if (GetFocus())
		refresh_delta();
}
bool Mouse::GetStatusKey(eMouseKey key)
{
	refresh_delta();
	bool flag = false;
	switch (key)
	{
	case LEFT: 
		if (dx < -sensitivity) {
			flag = true;
			dx = 0;
		}
		break;
	case RIGHT: 
		if (dx > sensitivity) {
			flag = true;
			dx = 0;
		}
		break;
	case UP: 
		if (dy < -sensitivity) {
			flag = true;
			dy = 0;
		}
		break;
	case DOWN: 
		if (dy > sensitivity) {
			flag = true;
			dy = 0;
		}
		break;
	//default:;
	}
	return flag;
}
