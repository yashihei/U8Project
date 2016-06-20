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

	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f);
	void draw(RectF uvRect, D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f);
private:
	struct ImageVertex {
		D3DXVECTOR3 p;
		float rhw;
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
	/// <param name="image">Imageへのスマートポインタ</param>
	/// <param name="col">列の分割数</param>
	/// <param name="row">行の分割数</param>
	/// <param name="currentRow">何行目を再生するか</param>
	/// <param name="interval">アニメーションの間隔</param>
	/// <param name="autoLineBreak">行を自動で送る</param>
	AnimationImage(std::shared_ptr<Image> image, int col, int row, int currentRow, int interval, bool autoLineBreak = false);

	void update();
	void switchRow(int currentRow) { m_currentRow = currentRow; }
	void draw(D3DXVECTOR2 pos, float rad = 0.0f, float scale = 1.0f);
private:
	std::shared_ptr<Image> m_image;
	int m_col, m_row, m_currentRow, m_interval;
	int m_cnt;
	bool m_autoLineBreak;
	RectF m_uvRect;
};
