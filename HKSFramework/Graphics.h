#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Graphics {
public:
	explicit Graphics(HWND hWnd);
	~Graphics();

	HRESULT beginScene();
	HRESULT endScene();
	HRESULT present();
	LPDIRECT3DDEVICE9 getDevice() { return m_d3dDevice; }

	void drawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, float width = 1, D3DXCOLOR color = 0xFFFFFFFF);
	void drawCircle(D3DXVECTOR2 pos, int radius, D3DXCOLOR color = 0xFFFFFFFF);
	void drawRectangle(D3DXVECTOR2 pos, float width, float height, float rad, D3DXCOLOR color = 0xFFFFFFFF);
	void drawNgon(D3DXVECTOR2 pos, int num, int radius, float radian, D3DXCOLOR color = 0xFFFFFFFF);
private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;
};
