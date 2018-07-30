// Переменные константных буферов
cbuffer ConstantBuffer : register( b0 ) // b0 - индекс буфера
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_OUTPUT    // формат выходных данных вершинного шейдера

{

    float4 Pos : SV_POSITION;

    float4 Color : COLOR0;

};



VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	// Трансформация позиции вершины при помощи умножения на матрицу
	output.Pos = mul(Pos, World); // сначала в пространство мира
	output.Pos = mul(output.Pos, View); // затем в пространство вида
	output.Pos = mul(output.Pos, Projection); // в проекционное пространство
	output.Color = Color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{   
	
	// Возвращаем желтый цвет, непрозрачный (альфа == 1, альфа-канал не включен).
	//return float4(sin(input.Pos.x), cos(input.Pos.y), cos(input.Pos.y), 1.0f);
	return input.Color;
}