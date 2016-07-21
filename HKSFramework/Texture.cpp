#include "Texture.h"

#include <vector>

Texture::Texture(std::string filePath, std::shared_ptr<Graphics> graphics) :
m_d3dTex(NULL), m_graphics(graphics)
{
	m_d3dTex = m_graphics->loadTexture(filePath);

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

void Texture::draw(D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	draw({ 0.0f, 0.0f, 1.0f, 1.0f }, pos, rad, scale, color, mirror);
}

void Texture::drawFrame(int col, int row, int index, D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	RectF uvRect(0.0f, 0.0f, 1.0f / col, 1.0f / row);
	index %= (col * row);
	uvRect.x = uvRect.w * (index % col);
	uvRect.y = uvRect.h * (index / col);
	draw(uvRect, pos, rad, scale, color, mirror);
}

void Texture::draw(RectF uvRect, D3DXVECTOR2 pos, float rad, float scale, const D3DXCOLOR& color, bool mirror) {
	auto texSize = D3DXVECTOR2(m_size.x * uvRect.w, m_size.y * uvRect.h);
	std::vector<TextureVertex> vtx {
		{ { -texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x, uvRect.y } },
		{ { texSize.x/2, -texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x + uvRect.w, uvRect.y } },
		{ { -texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x, uvRect.y + uvRect.h } },
		{ { texSize.x/2, texSize.y/2, 0.0f }, 1.0f, color, { uvRect.x + uvRect.w, uvRect.y + uvRect.h } },
	};
	if (mirror) {
		std::swap(vtx[0].t, vtx[1].t);
		std::swap(vtx[2].t, vtx[3].t);
	}

	//アフィン変換
	for (int i = 0; i < 4; i++) {
		auto tPos = vtx[i].p;
		vtx[i].p.x = pos.x + scale * (tPos.x * std::cos(rad) - tPos.y * std::sin(rad));
		vtx[i].p.y = pos.y + scale * (tPos.x * std::sin(rad) + tPos.y * std::cos(rad));
	}

	m_graphics->drawTexture(m_d3dTex, vtx);
}

TextureManager::TextureManager(std::shared_ptr<Graphics> graphics) :
m_graphics(graphics)
{}

void TextureManager::load(std::string filePath, std::string alias) {
	m_textures[alias] = std::make_shared<Texture>(filePath, m_graphics);
}
