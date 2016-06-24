#include "Direct3D.h"

#include <stdexcept>

Direct3D::Direct3D(HWND hWnd) : m_d3d(NULL), m_d3dDevice(NULL), m_d3dpp{} {
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		throw std::runtime_error("Error initialize Direct3D");
	}

	//windowの場合、width, heightは自動で設定される
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//垂直同期無視
	//m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_d3dDevice)))
	{
		throw std::runtime_error("Error creating Direct3D device");
	}

	//zバッファ有効
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//αブレンドに関する設定
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//ライト無効
	m_d3dDevice->LightEnable(0, FALSE);
}

Direct3D::~Direct3D() {
	if (m_d3dDevice)
		m_d3dDevice->Release();
	if (m_d3d)
		m_d3d->Release();
}

HRESULT Direct3D::beginScene() {
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10, 20, 30), 1.0f, 0);
	return m_d3dDevice->BeginScene();
}

HRESULT Direct3D::endScene() {
	return m_d3dDevice->EndScene();
}

HRESULT Direct3D::present() {
	return m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}
