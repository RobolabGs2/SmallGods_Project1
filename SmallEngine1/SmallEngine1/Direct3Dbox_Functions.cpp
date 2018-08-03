
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
	XMMATRIX World = XMMatrixTranslation(pVoxel->location->x, pVoxel->location->y, pVoxel->location->z);
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(World);
	cb.mView = XMMatrixTranspose(View);
	cb.mProjection = XMMatrixTranspose(Projection);
	cb.vOutputColor = {1.0f, 1.0f, 1.0f, 1.0f };

	pImmediateContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);


	D3D11_BUFFER_DESC bd;						// Структура, описывающая создаваемый буфер
	ZeroMemory(&bd, sizeof(bd));				// очищаем ее
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * 24;			// размер буфера = размер одной вершины * кол-во вершин
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// тип буфера - буфер вершин
	bd.CPUAccessFlags = 0;


	D3D11_SUBRESOURCE_DATA InitData;			// Структура, содержащая данные буфера
	ZeroMemory(&InitData, sizeof(InitData));	// очищаем ее
	InitData.pSysMem = pVoxel->vertices;		// указатель на наши 3 вершины

	// Вызов метода pd3dDevice создаст объект буфера вершин ID3D11Buffer
	ID3D11Buffer* pVertexBuffer = NULL;
	pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
   

	bd.Usage = D3D11_USAGE_DEFAULT;				// Структура, описывающая создаваемый буфер
	bd.ByteWidth = sizeof(WORD) * 36;			// 6 граней = 12 треугольников = 36 вершин
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		// тип - буфер индексов
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = pVoxel->indices;			// указатель на наш массив индексов

	// Вызов метода pd3dDevice создаст объект буфера индексов
	ID3D11Buffer* pIndexBuffer = NULL;
	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer);

	// Установка буфера вершин:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	// Установка буфера индексов
	pImmediateContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	//	Устанавливаем шейдеры и константные буферы
	pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->PSSetShader(pPixelShader[0 /* Тут по идеи должен меняться номер*/], NULL, 0);
	pImmediateContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

	pImmediateContext->DrawIndexed(36, 0, 0);
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
		MessageBox(NULL, szFileName, L"Ошибка. Не найден файл.", MB_OK);
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
