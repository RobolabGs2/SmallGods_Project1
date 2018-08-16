// SmallEngine1.cpp: определяет точку входа для консольного приложения.
//


#include "stdafx.h"
#include "WinAPIInit.h"
#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <DirectXMath.h>
#include <vector>
#include <comdef.h>
#include "Player.h"
#include "Keyboard.h"
#include "Mouse.h"

using namespace DirectX;

int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WinAPIInit* pWinInit = new WinAPIInit(hInstance, nCmdShow);

	try
	{
		WCHAR fileName[] = L"Shaders.fx";

		Camera* camera = new Camera();
		Mouse* mouse = new Mouse(pWinInit->GetHWnd());
		Keyboard<eKeyAction>* keyboard = new Keyboard<eKeyAction>(mouse);
		Direct3Dbox* pDXBox = new Direct3Dbox(pWinInit, fileName, camera);
		PhysicalBox* pPhBox = new PhysicalBox(pDXBox);
		Player* pPlayer = new Player(NULL, NULL, pDXBox, pPhBox, keyboard);
		camera->SetOwner(pPlayer);

		DWORD dwLastTime = GetTickCount();
		DWORD dwCurrentTime = dwLastTime;
		
		pPhBox->AddObject(pPlayer);
		pPhBox->GenerateVoxels(pDXBox);

		while (pWinInit->Tick())
		{
			auto dt = dwCurrentTime - dwLastTime;
			keyboard->Tick(dt);
			//mouse->Tick(dt);
			pPhBox->Tick(dt);
			dwLastTime = dwCurrentTime;
			dwCurrentTime = GetTickCount();
		}
		
	}
	catch (const _com_error* e)
	{
		MessageBoxW(pWinInit->GetHWnd(), e->ErrorMessage(), L"Ошибка", 16);
	}

	return 0;
}
