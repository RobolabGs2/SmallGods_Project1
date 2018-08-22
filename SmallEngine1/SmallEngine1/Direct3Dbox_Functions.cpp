
#include "stdafx.h"
#include "Direct3Dbox.h"

void Direct3Dbox::Show()
{
	HRESULT hr =  pSwapChain->Present(0, 0);

	float ClearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3Dbox::Draw(Voxel* pVoxel)
{
	HRESULT hr = S_OK;

	XMMATRIX World = pVoxel->GetMatrixWorld();

	cb.mView = XMMatrixTranspose(camera->GetView());
	cb.mWorld = XMMatrixTranspose(World);
	//cb.vOutputColor = { 0.02f, 0.22f, 0.44f, 1.0f };
	cb.vOutputColor = { 0.2f, 0.5f, 0.0f, 1.0f };

	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);


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
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	Exp(hr);


	// ��������� ������ ������:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);


	//	������������� ������� � ����������� ������
	pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->PSSetShader(pPixelShader[0 /* ��� �� ���� ������ �������� �����*/], NULL, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	//pImmediateContext->DrawIndexed(pVoxel->img_indices.size(), 0, 0);
	pImmediateContext->Draw(pVoxel->img_vertices.size(), 0);

	pVertexBuffer->Release();

}


void Direct3Dbox::Exp(HRESULT hr)
{
	if (FAILED(hr))
		throw new _com_error(hr);
}


HRESULT Direct3Dbox::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	ID3DBlob* pErrorBlob = NULL;
	hr = D3DCompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
		0, 0, ppBlobOut, NULL);


	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		if (pErrorBlob) pErrorBlob->Release();
		MessageBox(NULL, szFileName, L"������. �� ������ ����.", MB_OK);
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
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
		Exp(E_INVALIDARG);
		return "";
	}
}
