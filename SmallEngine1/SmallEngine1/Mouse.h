#pragma once
#include <windows.h>
#include <functional>
typedef std::function<void(DWORD, int, int)> MouseAction;

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
	int dx = 0, dy = 0, sensitivity = 5;
	void refresh_delta();
	HWND window;
public:
	Mouse(HWND);
	~Mouse();
	bool GetStatusKey(eMouseKey key);
	void Tick(DWORD);
};
