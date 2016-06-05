#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Direct3D {
public:
	Direct3D();
	~Direct3D();
	void init(HWND hWnd);

	LPDIRECT3DDEVICE9 getDevice() { return m_d3dDevice; }
private:
	LPDIRECT3D9 m_d3d;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	D3DPRESENT_PARAMETERS m_d3dpp;
};
