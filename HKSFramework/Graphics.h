#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <vector>

struct TextureVertex;

class Graphics {
public:
	explicit Graphics(HWND hWnd);
	~Graphics();

	HRESULT beginScene();
	HRESULT endScene();
	HRESULT present();
	LPDIRECT3DDEVICE9 getDevice() { return m_d3dDevice; }

	LPDIRECT3DTEXTURE9 loadTexture(std::string filePath);
	void drawTexture(LPDIRECT3DTEXTURE9 texure, const std::vector<TextureVertex>& vtx);
	void drawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, float width = 1, D3DXCOLOR color = 0xFFFFFFFF);
	void drawCircle(D3DXVECTOR2 pos, int radius, D3DXCOLOR color = 0xFFFFFFFF);
	void drawRectangle(D3DXVECTOR2 pos, float width, float height, float rad, D3DXCOLOR color = 0xFFFFFFFF);
	void drawNgon(D3DXVECTOR2 pos, int num, int radius, float radian, D3DXCOLOR color = 0xFFFFFFFF);
private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};
