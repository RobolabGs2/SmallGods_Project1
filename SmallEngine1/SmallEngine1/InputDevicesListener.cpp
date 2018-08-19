#include "stdafx.h"
#include "InputDevicesListener.h"


InputDevicesListener::InputDevicesListener(HWND hwnd): InputDevices<eKeyAction>(hwnd)
{

}


InputDevicesListener::~InputDevicesListener()
{
}

void InputDevicesListener::process_message(MSG msg)
{
	ReadMessage(msg);
}
