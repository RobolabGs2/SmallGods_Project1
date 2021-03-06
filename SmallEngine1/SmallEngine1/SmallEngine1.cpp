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
#include "InputDevicesListener.h"

using namespace DirectX;

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine, int nCmdShow)
{
	WinAPIInit* pWinInit = new WinAPIInit(hInstance, nCmdShow);

	try
	{
		WCHAR graphicShadersFileName[] = L"GraphicShaders.fx";
		WCHAR physicalShadersFileName[] = L"PhysicalShaders.fx";

		Camera* camera = new Camera();
		InputDevicesListener* input_devices = new InputDevicesListener(pWinInit->GetHWnd());
		pWinInit->AddListener(input_devices);
		Direct3Dbox* pDXBox = new Direct3Dbox(pWinInit, graphicShadersFileName, camera);
		PhysicalBox* pPhBox = new PhysicalBox(pDXBox, physicalShadersFileName);
		Player* pPlayer = new Player(NULL, NULL, pDXBox, pPhBox, input_devices);
		camera->SetOwner(pPlayer);

		DWORD dwLastTime = GetTickCount();
		DWORD dwCurrentTime = dwLastTime;

		pPhBox->AddObject(pPlayer);
		pPhBox->GenerateVoxels(pDXBox);
		while (pWinInit->Tick())
		{
			DWORD dt = dwCurrentTime - dwLastTime;
			input_devices->Tick(dt);
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
