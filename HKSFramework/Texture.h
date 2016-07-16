#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>

struct RectF {
	RectF(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	float x, y, w, h;
};

class Texture {
public:
	Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice);
	~Texture();

	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
	void draw(RectF uvRect, D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
private:
	struct TextureVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
		D3DXVECTOR2 t;
	};
	LPDIRECT3DTEXTURE9 m_d3dTex;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	D3DXVECTOR2 m_size;
};

class TextureManager {
public:
	TextureManager(LPDIRECT3DDEVICE9 d3dDevice);
	void load(std::string filePath, std::string alias);
	std::shared_ptr<Texture> getTexture(std::string alias) { return m_textures[alias]; }
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};

class Animation {
public:
	Animation(std::shared_ptr<Texture> texture, int col, int row);
	void addvanceFrame();
	void changeFrame(int value);
	void addPattern(std::string alias, int startFrame, int endFrame);
	void addPattern(std::string alias, std::vector<int> patternList) { m_patterns[alias] = patternList; }
	void changePattern(std::string alias) { m_currentPattern = alias; m_currentFrame = 0; }
	bool isPlaying(std::string alias) { return alias == m_currentPattern; }
	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, const D3DXCOLOR& color = 0xFFFFFFFF, bool mirror = false);
private:
	void setRect();
	std::shared_ptr<Texture> m_texture;
	int m_col, m_row, m_currentFrame;
	RectF m_uvRect;
	std::string m_currentPattern;
	std::unordered_map<std::string, std::vector<int>> m_patterns;
};
