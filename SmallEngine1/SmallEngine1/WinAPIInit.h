#pragma once

#include <windows.h>
#include "IListenerWindow.h"
#include "resource.h"
#include <set>

//Класс для работы с WinAPI
class WinAPIInit
{
private:
	HWND hWnd;
	HINSTANCE hInstance;
	MSG msg;
	std::set<IListenerWindow*> listeners_;
public:
	//Конструктор
	WinAPIInit(HINSTANCE hInstance, int nCmdShow);
	//Возвращает hWnd
	HWND GetHWnd();
	//Возвращает hInstance
	HINSTANCE GetHInstance();
	//Такт обработкаи окна, возвращает false при закрытии окна
	bool Tick();
	//Добавляет слушателя сообщений
	void AddListener(IListenerWindow*);
	//Деструктор вроде не нужен, но пусть будет
	~WinAPIInit();
};