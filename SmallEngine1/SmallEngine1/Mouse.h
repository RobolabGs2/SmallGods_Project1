#pragma once
#include <windows.h>
#include <functional>
// Функция от времени, координаты по x, y
typedef std::function<void(DWORD, float, float)> MouseAction;

enum eMouseKey
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
	void refresh_delta();
	MouseAction bind_action = nullptr;
	HWND window;
public:
	Mouse(HWND);
	~Mouse();
	bool GetStatusKey(eMouseKey key);
	void Tick(DWORD);
	void SetBindAction(MouseAction);
};
