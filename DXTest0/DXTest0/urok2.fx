// ���������� ����������� �������
cbuffer ConstantBuffer : register( b0 ) // b0 - ������ ������
{
    matrix World;
    matrix View;
    matrix Projection;
	float4 vLightDir[2];              // ����������� ��������� �����
	float4 vLightColor[2];            // ���� ��������� �����
	float4 vOutputColor;              // �������� ����
}

//--------------------------------------------------------------------------------------

struct VS_INPUT // �������� ������ ���������� �������
{
	float4 Pos	: POSITION;        // ������� �� X, Y, Z
	float3 Norm : NORMAL;         // ������� �� X, Y, Z
};

struct PS_INPUT // �������� ������ ����������� �������
{
	float4 Pos	: SV_POSITION;     // ������� ������� � �������� (��������)
	float3 Norm	: TEXCOORD0;      // ������������� ������� ������� �� tu, tv
	float4 fPos	: POSITION;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(input.Norm, World);
	output.fPos = input.Pos;

	return output;
}

float Random(float a)
{
	return	(sin(sin(sin(sin(a) * 10) * 3) * 4) + 1) / 2;
}

float GetRotationMatr(float a, float b, float c)
{
	float snA = sin(a);
	float cnA = cos(a);

	float3x3  rotA =
	{
		cnA,	-snA,	0.0f,
		snA,	 cnA,	0.0f,
		0.0f,	0.0f,	1.0f
	};

	float snB = sin(b);
	float cnB = cos(b);

	float3x3  rotB =
	{
		cnB,	0.0f,	snB,
		0.0f,	1.0f,	0.0f,
		-snB,	0.0f,	cnB
	};

	float snC = sin(c);
	float cnC = cos(c);

	float3x3  rotC =
	{
		1.0f,	0.0f,	0.0f,
		0.0f,	cnC,	-snC,
		0.0f,	snC,	cnC
	};

	return mul(mul(rotA, rotB), rotC);
}

//��� �������� ����
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = {0, 0, 0, 0};

	float3x3 rot = GetRotationMatr(
		Random(input.fPos.x * 10) / 2, 
		Random(input.fPos.y * 10) / 2,
		Random(input.fPos.z * 10) / 2 );
	

	for (int i = 0; i < 2; i++)
	{
		finalColor += saturate(dot((float3)vLightDir[i], mul(input.Norm, rot)) * vLightColor[i]);
	}
	finalColor.a = 1;
	return finalColor;
}

//��� ���������� �����
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}