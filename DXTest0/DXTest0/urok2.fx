



float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	// Оставляем координаты точки без изменений
	return Pos;
}

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
	// Возвращаем желтый цвет, непрозрачный (альфа == 1, альфа-канал не включен).
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}