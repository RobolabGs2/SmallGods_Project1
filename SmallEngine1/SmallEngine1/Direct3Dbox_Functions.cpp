
#include "stdafx.h"
#include "Direct3Dbox.h"

void Direct3Dbox::Show()
{
	pSwapChain->Present(0, 0);

	float ClearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pImmediateContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3Dbox::Draw(Voxel* pVoxel)
{
	
}