#include "Image.h"
#include <vector>

Texture::Texture(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dTex(NULL), m_size(0, 0)
{
	auto hr = D3DXCreateTextureFromFile(d3dDevice, fileName.c_str(), &m_d3dTex);
	if (FAILED(hr)) {
		throw std::runtime_error("Error load " + fileName);
	}
	
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(fileName.c_str(), &info);
	m_size.x = static_cast<float>(info.Width);
	m_size.y = static_cast<float>(info.Height);
}

Texture::~Texture() {
	if (m_d3dTex) m_d3dTex->Release();
}

Image::Image(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice) :
m_texture(NULL), m_d3dDevice(d3dDevice)
{
	m_texture = std::make_shared<Texture>(fileName, d3dDevice);
}

Image::Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice) :
m_texture(texure), m_d3dDevice(d3dDevice)
{}

void Image::draw(D3DXVECTOR2 pos, double rad, double scale, bool isFlip) {
	auto d3dTex = m_texture->getTexture();
	auto texSize = m_texture->getSize();
	//ÉçÅ[ÉJÉãç¿ïW
	std::vector<ImageVertex> vx {
		{ { -texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, { 0.0f, 0.0f } },
		{ { texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, { 1.0f, 0.0f } },
		{ { -texSize.x/2, texSize.y/2, 0.0f }, 1.0f, { 0.0f, 1.0f } },
		{ { texSize.x/2, texSize.y/2, 0.0f }, 1.0f, { 1.0f, 1.0f } },
	};
	//âÒì]Å®ägëÂÅ®à⁄ìÆ
	for (int i = 0; i < 4; i++) {
		auto tPos = vx[i].p;
		vx[i].p.x = pos.x + scale * (tPos.x * std::cos(rad) - tPos.y * std::sin(rad));
		vx[i].p.y = pos.y + scale * (tPos.x * std::sin(rad) + tPos.y * std::cos(rad));
	}
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_d3dDevice->SetTexture(0, d3dTex);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vx.data(), sizeof(ImageVertex));
}
