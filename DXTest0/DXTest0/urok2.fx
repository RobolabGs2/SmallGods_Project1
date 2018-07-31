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
	float4 Pos : POSITION;        // ������� �� X, Y, Z
	float3 Norm : NORMAL;         // ������� �� X, Y, Z
};

struct PS_INPUT // �������� ������ ����������� �������
{
	float4 Pos : SV_POSITION;     // ������� ������� � �������� (��������)
	float3 Norm : TEXCOORD0;      // ������������� ������� ������� �� tu, tv
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(input.Norm, World);

	return output;
}


//��� �������� ����
float4 PS(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;

	float inp = input.Pos.x*10;

	float random = sin(sqrt(log10(inp) * inp) * inp);

	float a = (random + 1) / 100;
	
	float sn = sin(a);
	float cn = cos(a);
	
	float3x3  rot1 =
	{
	 cn,	-sn,	0.0f,
	 sn,	 cn,	0.0f,
	0.0f,	0.0f,	1.0f
	};


	 inp = input.Pos.y * 10;

	 random = sin(sqrt(log10(inp) * inp) * inp);

	 a = (random + 1) / 100;

	 sn = sin(a);
	 cn = cos(a);

	float3x3  rot2 =
	{
		1.0f,	0.0f,	0.0f,
		0.0f,	cn,	sn,
		0.0f,	-sn,	cn
	};


	for (int i = 0; i < 2; i++)
	{
		finalColor += saturate(dot((float3)vLightDir[i], mul(input.Norm, mul(rot1, rot2)) )* vLightColor[i]);
	}
	finalColor.a = 1;
	return finalColor;
}

//��� ���������� �����
float4 PSSolid(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}