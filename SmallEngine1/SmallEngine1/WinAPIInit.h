#pragma once

#include <windows.h>
#include "resource.h"

//Класс для работы с WinAPI
class WinAPIInit
{
private:
	HWND hWnd;
	HINSTANCE hInstance;
	MSG msg;
public:
	//Конструктор
	WinAPIInit(HINSTANCE hInstance, int nCmdShow);
	//Возвращает hWnd
	HWND GetHWnd();
	//Возвращает hInstance
	HINSTANCE GetHInstance();
	//Такт обработкаи окна, возвращает false при закрытии окна
	bool Tick();
	//Деструктор вроде не нужен, но пусть будет
	~WinAPIInit();
};