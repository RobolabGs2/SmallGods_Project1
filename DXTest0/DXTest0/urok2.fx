



float4 VS(float4 Pos : POSITION) : SV_POSITION
{
	// ��������� ���������� ����� ��� ���������
	return Pos;
}

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{   
	
	// ���������� ������ ����, ������������ (����� == 1, �����-����� �� �������).
	return float4(sin(Pos.x), cos(Pos.y), 0.0f, 1.0f);
}