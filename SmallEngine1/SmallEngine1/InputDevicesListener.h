#pragma once
#include "InputDevices.h"
#include "eKeyAction.h"
#include "IListenerWindow.h"

class InputDevicesListener :
	public InputDevices<eKeyAction>, public IListenerWindow
{
public:
	InputDevicesListener(HWND);
	virtual ~InputDevicesListener();
	void process_message(MSG msg) override;
};

