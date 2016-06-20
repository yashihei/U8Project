#include "Image.h"

#include <vector>
#include "Util.h"

Texture::Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dTex(NULL), m_size(0, 0)
{
	auto hr = D3DXCreateTextureFromFile(d3dDevice, filePath.c_str(), &m_d3dTex);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed load " + filePath);
	}
	
	//テクスチャのサイズを取得
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(filePath.c_str(), &info);
	m_size.x = static_cast<float>(info.Width);
	m_size.y = static_cast<float>(info.Height);
}

Texture::~Texture() {
	if (m_d3dTex)
		m_d3dTex->Release();
}

Image::Image(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice) :
m_texture(NULL), m_d3dDevice(d3dDevice)
{
	m_texture = std::make_shared<Texture>(filePath, d3dDevice);
}

Image::Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice) :
m_texture(texure), m_d3dDevice(d3dDevice)
{}

void Image::draw(D3DXVECTOR2 pos, float rad, float scale) {
	draw({ 0.0f, 0.0f, 1.0f, 1.0f }, pos, rad, scale);
}

//TODO:反転(uv値逆転で実装)
//TODO:matrixで実装
//TODO:SetStreamで実装
void Image::draw(RectF uvRect, D3DXVECTOR2 pos, float rad, float scale) {
	auto d3dTex = m_texture->getTexture();
	auto texSize = m_texture->getSize();
	texSize.x *= uvRect.w, texSize.y *= uvRect.h;
	//ローカル座標
	std::vector<ImageVertex> vx {
		{ { -texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, { uvRect.x, uvRect.y } },
		{ { texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, { uvRect.x + uvRect.w, uvRect.y } },
		{ { -texSize.x/2, texSize.y/2, 0.0f }, 1.0f, { uvRect.x, uvRect.y + uvRect.h } },
		{ { texSize.x/2, texSize.y/2, 0.0f }, 1.0f, { uvRect.x + uvRect.w, uvRect.y + uvRect.h } },
	};
	//回転→拡大→移動
	for (int i = 0; i < 4; i++) {
		auto tPos = vx[i].p;
		vx[i].p.x = pos.x + scale * (tPos.x * std::cos(rad) - tPos.y * std::sin(rad));
		vx[i].p.y = pos.y + scale * (tPos.x * std::sin(rad) + tPos.y * std::cos(rad));
	}
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	m_d3dDevice->SetTexture(0, d3dTex);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vx.data(), sizeof(ImageVertex));
}

ImageManager::ImageManager(LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice)
{}

void ImageManager::preLoad(std::string filePath, std::string alias) {
	m_images[alias] = std::make_shared<Image>(filePath, m_d3dDevice);
}

AnimationImage::AnimationImage(std::shared_ptr<Image> image, int col, int row, int currentRow, int interval, bool autoLineBreak) :
m_image(image),
m_col(col), m_row(row), m_currentRow(currentRow), m_interval(interval),
m_cnt(0),
m_autoLineBreak(autoLineBreak),
m_uvRect(0.0f, 0.0f, 1.0f / m_col, 1.0f / m_row)
{}

void AnimationImage::update() {
	m_cnt++;

	const int startFrame = m_col * m_currentRow;
	const int nowFrame = Util::wrap(startFrame + m_cnt / m_interval, startFrame, startFrame + m_col);
	const int offsetU = nowFrame % m_col;
	const int offsetV = nowFrame / m_col;

	m_uvRect.x = offsetU * m_uvRect.w;
	m_uvRect.y = offsetV * m_uvRect.h;
}

void AnimationImage::draw(D3DXVECTOR2 pos, float rad, float scale) {
	m_image->draw(m_uvRect, pos, rad, scale);
}
