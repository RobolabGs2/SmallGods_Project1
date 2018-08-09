#pragma once

#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;


//	��������� ������� ������������, ������������� ���������
float CVMArea(XMVECTOR a, XMVECTOR b);
//	���������� ������� ���� ����� ���������
float CVMCosAngle(XMVECTOR a, XMVECTOR b);
//	���������� ����� ���� ����� ���������
float CVMSinAngle(XMVECTOR a, XMVECTOR b);
//	���������� ���� � �������� ����� ���������
float CVMAngle(XMVECTOR a, XMVECTOR b);
//	���������� ����� �������
float CVMAbs(XMVECTOR a);