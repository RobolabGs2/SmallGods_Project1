// SmallEngine1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "WinAPIInit.h"
#include "Direct3Dbox.h"
#include "PhysicalBox.h"
#include <comdef.h>
#include "Player.h"

int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WinAPIInit* pWinInit = new WinAPIInit(hInstance, nCmdShow);

	try
	{
		WCHAR fileName[] = L"Shaders.fx";
		Camera* camera = new Camera();
		Player* player = new Player(camera);
		Direct3Dbox* pDXBox = new Direct3Dbox(pWinInit, fileName, camera);
		PhysicalBox* pPhBox = new PhysicalBox(pDXBox);

		DWORD dwLastTime = GetTickCount();
		DWORD dwCurrentTime = dwLastTime;

		//--Добавление объектов для демонстрации
		pPhBox->AddObject(new Voxel(NULL, NULL, pDXBox, pPhBox));
		pPhBox->AddObject(player);
		//--Добавление объектов для демонстрации

		while (pWinInit->Tick())
		{
			pPhBox->Tick(dwCurrentTime - dwLastTime);
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
