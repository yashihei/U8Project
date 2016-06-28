#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>

class Texture {
public:
	Texture(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice);
	~Texture();
	LPDIRECT3DTEXTURE9 getTexture() const { return m_d3dTex; }
	D3DXVECTOR2 getSize() const { return m_size; }
private:
	LPDIRECT3DTEXTURE9 m_d3dTex;
	D3DXVECTOR2 m_size;
};

struct RectF {
	RectF(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	float x, y, w, h;
};

class Image {
public:
	Image(std::string filePath, LPDIRECT3DDEVICE9 d3dDevice);
	Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice);

	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, bool isFlip = false);
	void draw(RectF uvRect, D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, float alpha = 1.0f, bool isFlip = false);
private:
	struct ImageVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
		D3DXVECTOR2 t;
	};
	std::shared_ptr<Texture> m_texture;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};

class ImageManager {
public:
	ImageManager(LPDIRECT3DDEVICE9 d3dDevice);
	void preLoad(std::string filePath, std::string alias);
	std::shared_ptr<Image> getImage(std::string alias) { return m_images[alias]; }
private:
	std::unordered_map<std::string, std::shared_ptr<Image>> m_images;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};

class AnimationImage {
public:
	/// <param name="image">Image�ւ̃X�}�[�g�|�C���^</param>
	/// <param name="col">��̕�����</param>
	/// <param name="row">�s�̕�����</param>
	/// <param name="interval">�A�j���[�V�����̊Ԋu</param>
	/// <param name="startRow">���s�ڂ���Đ����邩</param>
	AnimationImage(std::shared_ptr<Image> image, int col, int row, int interval);
	void update();
	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f, float alpha = 1.0f, bool isFlip = false);
	void addPattern(std::string alias, std::vector<int> patternList) { m_patterns[alias] = patternList; }
	void changePattern(std::string alias) { m_currentPattern = alias; m_cnt = 0; }
	bool isPlaying(std::string alias) { return alias == m_currentPattern; }
private:
	std::shared_ptr<Image> m_image;
	int m_col, m_row, m_interval, m_cnt;
	RectF m_uvRect;
	std::string m_currentPattern;
	std::unordered_map<std::string, std::vector<int>> m_patterns;
};
