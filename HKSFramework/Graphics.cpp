#include "Graphics.h"

#include <vector>
#include <stdexcept>
#include "Texture.h"

Graphics::Graphics(HWND hWnd) : m_d3d(NULL), m_d3dDevice(NULL) {
	if ((m_d3d = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		throw std::runtime_error("Error initialize Direct3D");
	}

	D3DPRESENT_PARAMETERS d3dpp = {};
	//window�̏ꍇ�Awidth, height�͎����Őݒ肳���
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//������������
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(m_d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_d3dDevice)))
	{
		throw std::runtime_error("Error creating Direct3D device");
	}

	//z�o�b�t�@�L��
	m_d3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//���u�����h�Ɋւ���ݒ�
	m_d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//���C�g����
	m_d3dDevice->LightEnable(0, FALSE);
}

Graphics::~Graphics() {
	if (m_d3dDevice)
		m_d3dDevice->Release();
	if (m_d3d)
		m_d3d->Release();
}

HRESULT Graphics::beginScene() {
	m_d3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(10, 20, 30), 1.0f, 0);
	return m_d3dDevice->BeginScene();
}

HRESULT Graphics::endScene() {
	return m_d3dDevice->EndScene();
}

HRESULT Graphics::present() {
	return m_d3dDevice->Present(NULL, NULL, NULL, NULL);
}

LPDIRECT3DTEXTURE9 Graphics::loadTexture(std::string filePath) {
	LPDIRECT3DTEXTURE9 d3dTex;
	HRESULT hr = D3DXCreateTextureFromFile(m_d3dDevice, filePath.c_str(), &d3dTex);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed load " + filePath);
	}
	return d3dTex;
}

void Graphics::drawTexture(LPDIRECT3DTEXTURE9 texture, const std::vector<TextureVertex>& vtx) {
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_d3dDevice->SetTexture(0, texture);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(TextureVertex));
	m_d3dDevice->SetTexture(0, NULL);
}

//D3DXLINE�A�d���̂ł��܂�g��Ȃ�����
void Graphics::drawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, float width, D3DXCOLOR color) {
	static LPD3DXLINE line = nullptr;
	if (!line)
		D3DXCreateLine(m_d3dDevice, &line);
	D3DXVECTOR2 vec[] = { start, end };
	line->SetWidth(width);
	line->Begin();
	line->Draw(vec, 2, color);
	line->End();
}

namespace {
	struct ShapeVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
	};
}

void Graphics::drawCircle(D3DXVECTOR2 pos, int radius, D3DXCOLOR color) {
	static const int splitNum = 64;
	std::vector<ShapeVertex> vtx(splitNum, { { 0, 0, 0 }, 1, color });
	for (int i = 0; i < splitNum; i++) {
		float radian = D3DX_PI * 2 * i / splitNum;
		vtx[i].p.x = pos.x + radius * std::cos(radian);
		vtx[i].p.y = pos.y + radius * std::sin(radian);
	}
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, splitNum - 2, vtx.data(), sizeof(ShapeVertex));
}

void Graphics::drawRectangle(D3DXVECTOR2 pos, float width, float height, float rad, D3DXCOLOR color) {
	std::vector<ShapeVertex> vtx {
		{ { -width / 2, -height / 2, 0 }, 1, color },
		{ { width / 2, -height / 2, 0 }, 1, color },
		{ { -width / 2, height / 2, 0 }, 1, color },
		{ { width / 2, height / 2, 0 }, 1, color },
	};
	for (int i = 0; i < 4; i++) {
		auto tPos = vtx[i].p;
		vtx[i].p.x = pos.x + tPos.x * std::cos(rad) - tPos.y * std::sin(rad);
		vtx[i].p.y = pos.y + tPos.x * std::sin(rad) + tPos.y * std::cos(rad);
	}
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(ShapeVertex));
}

void Graphics::drawNgon(D3DXVECTOR2 pos, int num, int radius, float radian, D3DXCOLOR color) {
	std::vector<ShapeVertex> vtx(num, { { 0, 0, 0 }, 1, color });
	for (int i = 0; i < num; i++) {
		float tRadian = D3DX_PI * 2 * i / num;
		vtx[i].p.x = pos.x + radius * std::cos(tRadian + radian);
		vtx[i].p.y = pos.y + radius * std::sin(tRadian + radian);
	}
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num - 2, vtx.data(), sizeof(ShapeVertex));
}
