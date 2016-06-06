#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Direct3D {
public:
	explicit Direct3D(HWND hWnd);
	~Direct3D();

	HRESULT beginScene();
	HRESULT endScene();
	HRESULT present();
	LPDIRECT3DDEVICE9 getDevice() { return m_d3dDevice; }
private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
};
