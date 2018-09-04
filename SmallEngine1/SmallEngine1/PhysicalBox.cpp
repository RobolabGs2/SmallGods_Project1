#include "stdafx.h"
#include "PhysicalBox.h"

PhysicalBox::PhysicalBox(Direct3Dbox* pDXbox, WCHAR* ShadersFileName)
{
	this->pDXbox = pDXbox;
	this->pDevicesBox = pDXbox->GetDevicesBox();
	pMainVox = new Voxel();

	HRESULT hr = S_OK;


	// Компиляция шейдера из файла
	ID3DBlob* pVSBlob = NULL; // Вспомогательный объект - просто место в оперативной памяти
	hr = pDevicesBox->CompileShaderFromFile(ShadersFileName, "CS_test", "cs_5_0", &pVSBlob);
	CheckAndThrowIfFailed(hr);

	// Создание шейдера
	hr = pDevicesBox->GetDevice()->CreateComputeShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pPhysicalParamsShader);
	CheckAndThrowIfFailed(hr);
}


void PhysicalBox::Tick(DWORD dt)
{
	
	Voxel* pointer = pMainVox->pNext;
	while (pointer != pMainVox)
	{
		pointer->Tick(dt);
		pointer = pointer->pNext;
	}

	pointer = pointer->pNext;

	while (pointer != pMainVox)
	{
		pointer->Move(dt);
		pointer = pointer->pNext;
	}

	pDXbox->Show();
}


void PhysicalBox::RunShader()
{

	HRESULT hr = S_OK;

	//	тестовый массивчик в качестве входных данных
	UINT myData[] = { 1, 0, 0, 0, 0, 0 };

	//	дескриптор UnorderedAccessView
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = ARRAYSIZE(myData);

	//	дескриптор буфера, который будет хранить тот массивчик
	D3D11_BUFFER_DESC descB;
	ZeroMemory(&descB, sizeof(descB));
	descB.Usage = D3D11_USAGE_DEFAULT;
	descB.ByteWidth = ARRAYSIZE(myData) * sizeof(UINT);
	descB.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descB.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descB.StructureByteStride = sizeof(UINT);
	descB.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	// Структура, содержащая данные буфера
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = myData;

	//	Собственно буфер
	ID3D11Buffer*  pBuffer;
	pDevicesBox->GetDevice()->CreateBuffer(&descB, &InitData, &pBuffer);

	//	UnorderedAccessView
	ID3D11UnorderedAccessView* pUAView;
	hr = pDevicesBox->GetDevice()->CreateUnorderedAccessView(pBuffer, &descUAV, &pUAView);
	CheckAndThrowIfFailed(hr);


	pDevicesBox->GetDeviceContext()->CSSetShader(pPhysicalParamsShader, NULL, 0);

	pDevicesBox->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &pUAView, NULL);

	pDevicesBox->GetDeviceContext()->Dispatch(1, 1, 1);

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	pDevicesBox->GetDeviceContext()->Map(pBuffer, 0, D3D11_MAP_READ, 0, &MappedResource);

	UINT* res = (UINT*)MappedResource.pData;
}

void PhysicalBox::AddObject(Voxel* voxel)
{
	pMainVox->AddPrev(voxel);

}

void PhysicalBox::GenerateVoxels(Direct3Dbox* pDXBox)
{
	float len = 1;

	std::vector<XMVECTOR> vertices =
	{
		XMVectorSet(0.0f,  -len,  len, 0.0f),
		XMVectorSet(0.0f,  0.0f, -len, 0.0f),
		XMVectorSet(-len ,  0.0f,  len, 0.0f),
		XMVectorSet(len ,  0.0f,  len, 0.0f),
	};

	std::vector<WORD> indices =
	{
		1,2,3,
		0,2,1,
		0,3,2,
		0,1,3,
	};

	std::vector<XMVECTOR> verticesTARDIS =
	{
		XMVectorSet(-1.0f, -2.0f, -1.0f, 0.0f),
		XMVectorSet(1.0f, -2.0f, -1.0f, 0.0f),
		XMVectorSet(1.0f, 2.0f, -1.0f, 0.0f),
		XMVectorSet(-1.0f, 2.0f, -1.0f, 0.0f),

		XMVectorSet(-1.0f, -2.0f, 1.0f, 0.0f),
		XMVectorSet(1.0f, -2.0f, 1.0f, 0.0f),
		XMVectorSet(1.0f, 2.0f, 1.0f, 0.0f),
		XMVectorSet(-1.0f, 2.0f, 1.0f, 0.0f),
	};

	std::vector<WORD> indicesTARDIS =
	{
		0,3,2,
		0,2,1,
		1,2,6,
		1,6,5,
		5,6,7,
		5,7,4,
		4,7,3,
		4,3,0,
		3,7,6,
		3,6,2,
		4,0,1,
		4,1,5,
	};

	Voxel* pyramid = new Voxel(NULL, NULL, pDXBox, this, vertices, indices, XMVectorSet(3, 0, 0, 0));
	Voxel* TARDIS = (new Voxel(NULL, NULL, pDXBox, this, verticesTARDIS, indicesTARDIS, XMVectorSet(-5, 0, 0, 0)))->SetMovable(false);
	
	TARDIS->Mound(2, 0.1);
	TARDIS->RecalculateImage();
	TARDIS->RecalculatePhisicalParams();
	AddObject(pyramid);
	AddObject(TARDIS);
}

Voxel* PhysicalBox::GetVoxelsQueueEnd()
{
	return pMainVox;
}

void PhysicalBox::RemoveVoxel(Voxel* pVoxel)
{
	pVoxel->pPrev->pNext = pVoxel->pNext;
	pVoxel->pNext->pPrev = pVoxel->pPrev;
	pVoxel->~Voxel();
}

PhysicalBox::~PhysicalBox()
{
} 