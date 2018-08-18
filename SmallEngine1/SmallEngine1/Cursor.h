#pragma once
#include <Windows.h>

enum class CursorState
{
	Show,
	Hide
};

class Cursor
{
private:
	CursorState state_ = CursorState::Hide;
public:
	Cursor()
	{
		on();
	}

	~Cursor()
	= default;

	void on()
	{
		if (state_ == CursorState::Hide)
			while (ShowCursor(TRUE) < 0);
		state_ = CursorState::Show;
	}

	void off()
	{
		if (state_ == CursorState::Show)
			while (ShowCursor(FALSE) >= 0);
		state_ = CursorState::Hide;
	}

	void SetState(CursorState state)
	{
		switch (state)
		{
		case CursorState::Show:
			on();
			break;
		case CursorState::Hide: 
			off();
			break;
		default: ;//TODO исключение
		}
	}

	bool IsOn()
	{
		return state_ == CursorState::Show;
	};
};
