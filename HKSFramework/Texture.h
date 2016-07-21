#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>
#include "Graphics.h"

struct RectF {
	RectF(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	float x, y, w, h;
};

struct TextureVertex {
	D3DXVECTOR3 p;
	float rhw;
	DWORD color;
	D3DXVECTOR2 t;
};

class Texture {
public:
	Texture(std::string filePath, std::shared_ptr<Graphics> graphics);
	~Texture();

	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
	void draw(RectF uvRect, D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
	void drawFrame(int col, int row, int index, D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
private:
	std::shared_ptr<Graphics> m_graphics;
	LPDIRECT3DTEXTURE9 m_d3dTex;
	D3DXVECTOR2 m_size;
};

class TextureManager {
public:
	TextureManager(std::shared_ptr<Graphics> graphics);
	void load(std::string filePath, std::string alias);
	std::shared_ptr<Texture> getTexture(std::string alias) { return m_textures[alias]; }
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	std::shared_ptr<Graphics> m_graphics;
};
