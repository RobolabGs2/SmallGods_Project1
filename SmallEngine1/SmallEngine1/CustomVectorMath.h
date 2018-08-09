#pragma once

#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;


//	Вычисляет площадь треугольника, образованного векторами
float CVMArea(XMVECTOR a, XMVECTOR b);
//	Возвращает косинус угла между векторами
float CVMCosAngle(XMVECTOR a, XMVECTOR b);
//	Возвращает синус угла между векторами
float CVMSinAngle(XMVECTOR a, XMVECTOR b);
//	Возвращает угол в радианах между векторами
float CVMAngle(XMVECTOR a, XMVECTOR b);
//	Возвращает длину вектора
float CVMAbs(XMVECTOR a);