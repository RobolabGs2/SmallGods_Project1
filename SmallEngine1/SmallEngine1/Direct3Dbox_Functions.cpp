
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


	D3D11_BUFFER_DESC bd;										// ���������, ����������� ����������� �����
	ZeroMemory(&bd, sizeof(bd));								// ������� ��
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) *  pVoxel->img_vertices.size();	// ������ ������ = ������ ����� ������� * ���-�� ������
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;					// ��� ������ - ����� ������
	bd.CPUAccessFlags = 0;

	Vertex* vertices = &(pVoxel->img_vertices)[0];

	D3D11_SUBRESOURCE_DATA InitData;			// ���������, ���������� ������ ������
	ZeroMemory(&InitData, sizeof(InitData));	// ������� ��
	InitData.pSysMem = vertices;		// ��������� �� �������

	// ����� ������ pd3dDevice ������� ������ ������ ������ ID3D11Buffer
	ID3D11Buffer* pVertexBuffer = NULL;
	hr = pDevicesBox->GetDevice()->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	CheckAndThrowIfFailed(hr);


	// ��������� ������ ������:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDevicesBox->GetDeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	//	������������� ������� � ����������� ������
	pDevicesBox->GetDeviceContext()->VSSetShader(pVertexShader, NULL, 0);
	pDevicesBox->GetDeviceContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pDevicesBox->GetDeviceContext()->PSSetShader(pPixelShader[0 /* ��� �� ���� ������ �������� �����*/], NULL, 0);
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
