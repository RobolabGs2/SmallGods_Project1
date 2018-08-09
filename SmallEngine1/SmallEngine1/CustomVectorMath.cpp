#include "stdafx.h"

#include "CustomVectorMath.h"

float CVMArea(XMVECTOR a, XMVECTOR b)
{
	XMFLOAT3 mult;
	
	XMStoreFloat3(&mult, XMVectorMultiply(a, b));

	float absA = CVMAbs(a);
	float absB = CVMAbs(b);

	float cos = (mult.x + mult.y + mult.z) / (absA * absB);
	float area = absA * absB * sqrt(1 - cos * cos) / 2;

	return area;
}

float CVMCosAngle(XMVECTOR a, XMVECTOR b)
{
	XMFLOAT3 mult;
	XMStoreFloat3(&mult, XMVectorMultiply(a, b));

	float cos = (mult.x + mult.y + mult.z) / (CVMAbs(a) * CVMAbs(b));

	return cos;
}

float CVMSinAngle(XMVECTOR a, XMVECTOR b)
{
	float cos = CVMCosAngle(a, b);
	return sqrt(1 - cos * cos);
}

float CVMAngle(XMVECTOR a, XMVECTOR b)
{
	float cos = CVMCosAngle(a, b);
	return acos(cos);
}

float CVMAbs(XMVECTOR a)
{
	XMFLOAT3 sqrA;
	XMStoreFloat3(&sqrA, XMVectorMultiply(a, a));
	float absA = sqrt(sqrA.x + sqrA.y + sqrA.z);
	return absA;
}