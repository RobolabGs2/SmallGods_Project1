#pragma once
#include <Windows.h>
class IListenerWindow
{
public:
	virtual ~IListenerWindow() = default;
	virtual void process_message(MSG msg) = 0;
};