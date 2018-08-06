// Переменные константных буферов
cbuffer ConstantBuffer : register( b0 ) // b0 - индекс буфера
{
    matrix World;
    matrix View;
	matrix Projection;
	float4 vOutputColor;              // Активный цвет
	float3 SunPosition;
}

//--------------------------------------------------------------------------------------

struct VS_INPUT // Входящие данные вершинного шейдера
{
	float4 Pos : POSITION;        // Позиция по X, Y, Z
	float3 Norm : NORMAL;         // Нормаль по X, Y, Z
};

struct PS_INPUT // Входящие данные пиксельного шейдера
{
	float4 Pos : SV_POSITION;     // Позиция пикселя в проекции (экранная)
	float3 Norm : TEXCOORD0;      // Относительная нормаль пикселя по tu, tv
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


float4 PS_BASE(PS_INPUT input) : SV_Target
{
	float4 Illumination = saturate(dot(SunPosition, input.Norm));

	return vOutputColor * Illumination;
}

float4 PS_LIGHT(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}