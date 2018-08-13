#pragma once

#include <windows.h>
#include "resource.h"

//����� ��� ������ � WinAPI
class WinAPIInit
{
private:
	HWND hWnd;
	HINSTANCE hInstance;
	MSG msg;
public:
	//�����������
	WinAPIInit(HINSTANCE hInstance, int nCmdShow);
	//���������� hWnd
	HWND GetHWnd();
	//���������� hInstance
	HINSTANCE GetHInstance();
	//���� ���������� ����, ���������� false ��� �������� ����
	bool Tick();
	//���������� ����� �� �����, �� ����� �����
	~WinAPIInit();
};