#pragma once
#include <windows.h>
#include <functional>
#include "Cursor.h"
// Функция от времени, координаты по x, y
typedef std::function<void(DWORD, float, float)> MouseActionMove;
typedef std::function<void(DWORD, int)> MouseActionScroll;

enum class eMouseKey
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Mouse
{
private:
	float dx = 0, dy = 0, sensitivity = 0.001f;
	int scroll_ = 0;
	void refresh_delta();
	Cursor cursor_;
	MouseActionMove bind_action_move = nullptr;
	MouseActionScroll bind_action_scroll = nullptr;
	HWND window;
public:
	Mouse(HWND);
	~Mouse();
	bool GetStatusKey(eMouseKey key);
	void Tick(DWORD);
	void SetBindActionMove(MouseActionMove);
	void SetBindActionScroll(MouseActionScroll);
	void SetCursorState(CursorState);
	void ReadScrollMessage(MSG msg);
};