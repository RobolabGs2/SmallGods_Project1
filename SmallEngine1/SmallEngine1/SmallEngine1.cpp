// SmallEngine1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "WinAPIInit.h"
#include "Direct3Dbox.h"


int __stdcall WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WinAPIInit* pWinInit = new WinAPIInit(hInstance, nCmdShow);
	Direct3Dbox* pDXBox = new Direct3Dbox(pWinInit);


	while (pWinInit->Tick())
	{
		
	}

	return 0;
}
