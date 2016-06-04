#include "Direct3D.h"

Direct3D::Direct3D() : m_d3d(NULL), m_d3dDevice(NULL), m_d3dpp{} {}

HRESULT Direct3D::init(HWND hWnd) {
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		return E_FAIL;
	}

	m_d3dpp.Windowed = TRUE;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_d3dDevice)))
	{
		return E_FAIL;
	}

	return S_OK;
}
