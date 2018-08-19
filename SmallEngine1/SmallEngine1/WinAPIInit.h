#pragma once

#include <windows.h>
#include "IListenerWindow.h"
#include "resource.h"
#include <set>

//����� ��� ������ � WinAPI
class WinAPIInit
{
private:
	HWND hWnd;
	HINSTANCE hInstance;
	MSG msg;
	std::set<IListenerWindow*> listeners_;
public:
	//�����������
	WinAPIInit(HINSTANCE hInstance, int nCmdShow);
	//���������� hWnd
	HWND GetHWnd();
	//���������� hInstance
	HINSTANCE GetHInstance();
	//���� ���������� ����, ���������� false ��� �������� ����
	bool Tick();
	//��������� ��������� ���������
	void AddListener(IListenerWindow*);
	//���������� ����� �� �����, �� ����� �����
	~WinAPIInit();
};