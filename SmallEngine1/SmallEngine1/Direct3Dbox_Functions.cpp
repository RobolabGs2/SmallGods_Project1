
#include "stdafx.h"
#include "Direct3Dbox.h"

void Direct3Dbox::Show()
{
	HRESULT hr = pDevicesBox->GetSwapChain()->Present(0, 0);

	float ClearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	pDevicesBox->GetDeviceContext()->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pDevicesBox->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3Dbox::Draw(Voxel* pVoxel)
{
	HRESULT hr = S_OK;

	XMMATRIX World = pVoxel->GetMatrixWorld();

	cb.mView = XMMatrixTranspose(camera->GetView());
	cb.mWorld = XMMatrixTranspose(World);
	//cb.vOutputColor = { 0.02f, 0.22f, 0.44f, 1.0f };
	//cb.vOutputColor = { 0.2f, 0.5f, 0.0f, 1.0f };
	cb.vOutputColor = { 1, 1, 1, 1.0f };

	pDevicesBox->GetDeviceContext()->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);


	D3D11_BUFFER_DESC bd;										// Структура, описывающая создаваемый буфер
	ZeroMemory(&bd, sizeof(bd));								// очищаем ее
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) *  pVoxel->img_vertices.size();	// размер буфера = размер одной вершины * кол-во вершин
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// тип буфера - буфер вершин
	bd.CPUAccessFlags = 0;

	Vertex* vertices = &(pVoxel->img_vertices)[0];

	D3D11_SUBRESOURCE_DATA InitData;			// Структура, содержащая данные буфера
	ZeroMemory(&InitData, sizeof(InitData));	// очищаем ее
	InitData.pSysMem = vertices;		// указатель на вершины

	// Вызов метода pd3dDevice создаст объект буфера вершин ID3D11Buffer
	ID3D11Buffer* pVertexBuffer = NULL;
	hr = pDevicesBox->GetDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	CheckAndThrowIfFailed(hr);


	// Установка буфера вершин:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDevicesBox->GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	//	Устанавливаем шейдеры и константные буферы
	pDevicesBox->GetDeviceContext()->VSSetShader(pVertexShader, NULL, 0);
	pDevicesBox->GetDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDevicesBox->GetDeviceContext()->PSSetShader(pPixelShader[0 /* Тут по идеи должен меняться номер*/], NULL, 0);
	pDevicesBox->GetDeviceContext()->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	//pImmediateContext->DrawIndexed(pVoxel->img_indices.size(), 0, 0);
	pDevicesBox->GetDeviceContext()->Draw(pVoxel->img_vertices.size(), 0);

	pVertexBuffer->Release();

}


DirectDevicesBox* Direct3Dbox::GetDevicesBox()
{
	return pDevicesBox;
}


LPCSTR Direct3Dbox::ShaderTypeToLPCSTR(PixelShaderType stype)
{
	switch (stype)
	{
	case SHT_BASE:
		return "PS_BASE";
	case SHT_LIGHT:
		return "PS_LIGHT";
	default:
		CheckAndThrowIfFailed(E_INVALIDARG);
		return "";
	}
}
