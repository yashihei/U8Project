#pragma once
#include <string>
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>

class Texture {
public:
	Texture(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice);
	~Texture();
	LPDIRECT3DTEXTURE9 getTexture() const { return m_d3dTex; }
	D3DXVECTOR2 getSize() const { return m_size; }
private:
	LPDIRECT3DTEXTURE9 m_d3dTex;
	D3DXVECTOR2 m_size;
};

class Image {
public:
	Image(std::string fileName, LPDIRECT3DDEVICE9 d3dDevice);
	Image(std::shared_ptr<Texture> texure, LPDIRECT3DDEVICE9 d3dDevice);
	void draw(D3DXVECTOR2 pos, double rad, double scale = 1.0, bool isFlip = false);
private:
	struct ImageVertex {
		D3DXVECTOR3 p;
		float rhw;
		D3DXVECTOR2 t;
	};
	std::shared_ptr<Texture> m_texture;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};
