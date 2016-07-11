#include "Image.h"

#include <vector>

Texture::Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dTex(NULL), m_size(0, 0)
{
	HRESULT hr = D3DXCreateTextureFromFile(d3dDevice, filePath.c_str(), &m_d3dTex);
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

void Image::draw(D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	draw({ 0.0f, 0.0f, 1.0f, 1.0f }, pos, rad, scale, color, mirror);
}

//TODO:SetStreamで実装
void Image::draw(RectF uvRect, D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	std::vector<D3DXVECTOR2> uv = {
		{ uvRect.x, uvRect.y }, { uvRect.x + uvRect.w, uvRect.y }, { uvRect.x, uvRect.y + uvRect.h }, { uvRect.x + uvRect.w, uvRect.y + uvRect.h }
	};
	if (mirror) {
		std::swap(uv[0], uv[1]);
		std::swap(uv[2], uv[3]);
	}

	auto texSize = m_texture->getSize();
	texSize.x *= uvRect.w, texSize.y *= uvRect.h;
	std::vector<ImageVertex> vx {
		{ { -texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uv[0].x, uv[0].y } },
		{ { texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uv[1].x, uv[1].y } },
		{ { -texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uv[2].x, uv[2].y } },
		{ { texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uv[3].x, uv[3].y } },
	};

	//アフィン変換
	//回転→拡大→移動
	for (int i = 0; i < 4; i++) {
		auto tPos = vx[i].p;
		vx[i].p.x = pos.x + scale * (tPos.x * std::cos(rad) - tPos.y * std::sin(rad));
		vx[i].p.y = pos.y + scale * (tPos.x * std::sin(rad) + tPos.y * std::cos(rad));
	}

	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_d3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	auto d3dTex = m_texture->getTexture();
	m_d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	m_d3dDevice->SetTexture(0, d3dTex);
	m_d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vx.data(), sizeof(ImageVertex));
}

ImageManager::ImageManager(LPDIRECT3DDEVICE9 d3dDevice) :
m_d3dDevice(d3dDevice)
{}

void ImageManager::load(std::string filePath, std::string alias) {
	m_images[alias] = std::make_shared<Image>(filePath, m_d3dDevice);
}

AnimationImage::AnimationImage(std::shared_ptr<Image> image, int col, int row) :
m_image(image),
m_col(col), m_row(row), m_currentFrame(0),
m_uvRect(0.0f, 0.0f, 1.0f / m_col, 1.0f / m_row)
{}

void AnimationImage::addvanceFrame() {
	m_currentFrame++;
	setRect();
}

void AnimationImage::changeFrame(int value) {
	m_currentFrame = value;
	setRect();
}

void AnimationImage::addPattern(std::string alias, int startFrame, int endFrame) {
	std::vector<int> patternList;
	for (int i = startFrame; i < endFrame; i++)
		patternList.push_back(i);
	m_patterns[alias] = patternList;
}

void AnimationImage::setRect() {
	int nowFrame = 0;
	//指定するパターンが無い場合
	if (m_currentPattern.empty()) {
		nowFrame = m_currentFrame % (m_col * m_row);
	} else {
		int patternLen = m_patterns[m_currentPattern].size();
		nowFrame = m_patterns[m_currentPattern][m_currentFrame % patternLen];
	}

	int offsetU = nowFrame % m_col;
	int offsetV = nowFrame / m_col;

	m_uvRect.x = offsetU * m_uvRect.w;
	m_uvRect.y = offsetV * m_uvRect.h;
}

void AnimationImage::draw(D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	m_image->draw(m_uvRect, pos, rad, scale, color, mirror);
}
