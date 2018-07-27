#pragma once

#include <windows.h>
#include <d3d11.h>


// Инициализация устройств DirectX
HRESULT InitDevice(HWND);
// Удаление созданнных устройств DirectX
void CleanupDevice();
// Функция рисования
void Render();     
