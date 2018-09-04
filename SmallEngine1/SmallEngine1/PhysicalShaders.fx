
cbuffer CB : register(b0)
{
	unsigned int test;
};

struct BufType
{
	uint num;
};

RWStructuredBuffer<unsigned int> Buffer0 : register(u0);

[numthreads(3, 2, 1)]
void CS_test(uint3 DTid : SV_DispatchThreadID, uint GroupIndex : SV_GroupIndex)
{
	Buffer0[GroupIndex] = GroupIndex;
}
