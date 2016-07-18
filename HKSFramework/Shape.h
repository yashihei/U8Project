#pragma once
#include <d3dx9.h>
#include <vector>
#include <cmath>

namespace Shape {
	struct ShapeVertex {
		D3DXVECTOR3 p;
		float rhw;
		DWORD color;
	};
	LPDIRECT3DDEVICE9 d3dDevice;
	LPD3DXLINE line;
	void init(LPDIRECT3DDEVICE9 device) {
		d3dDevice = device;
		D3DXCreateLine(d3dDevice, &line);
	}
	void drawLine(D3DXVECTOR2 start, D3DXVECTOR2 end, float width = 1, D3DXCOLOR color = 0xFFFFFFFF) {
		D3DXVECTOR2 vec[] = { start, end };
		line->SetWidth(width);
		line->Begin();
		line->Draw(vec, 2, color);
		line->End();
	}
	void drawCircle(D3DXVECTOR2 pos, int radius, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx(64, { { 0, 0, 0 }, 1, color });
		for (int i = 0; i < 64; i++) {
			float radian = D3DX_PI * 2 * i / 64;
			vtx[i].p.x = pos.x + radius * std::cos(radian);
			vtx[i].p.y = pos.y + radius * std::sin(radian);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 62, vtx.data(), sizeof(ShapeVertex));
	}
	void drawRectangle(D3DXVECTOR2 pos, float width, float height, float rad, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx {
			{ { -width / 2, -height / 2, 0 }, 1, color },
			{ { width / 2, -height / 2, 0 }, 1, color },
			{ { -width / 2, height / 2, 0 }, 1, color },
			{ { width / 2, height / 2, 0 }, 1, color },
		};
		for (int i = 0; i < 4; i++) {
			auto tPos = vtx[i].p;
			vtx[i].p.x = pos.x + tPos.x * std::cos(rad) - tPos.y * std::sin(rad);
			vtx[i].p.y = pos.y + tPos.x * std::sin(rad) + tPos.y * std::cos(rad);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vtx.data(), sizeof(ShapeVertex));
	}
	void drawNgon(D3DXVECTOR2 pos, int num, int radius, float radian, D3DXCOLOR color = 0xFFFFFFFF) {
		std::vector<ShapeVertex> vtx(num, { { 0, 0, 0 }, 1, color });
		for (int i = 0; i < num; i++) {
			float tRadian = D3DX_PI * 2 * i / num;
			vtx[i].p.x = pos.x + radius * std::cos(tRadian + radian);
			vtx[i].p.y = pos.y + radius * std::sin(tRadian + radian);
		}
		d3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
		d3dDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, num - 2, vtx.data(), sizeof(ShapeVertex));
	}
	//void drawStar(D3DXVECTOR2 pos, int num, int size, float rad, D3DXCOLOR color = 0xFFFFFFFF) {
	//}
	//void drawPolygon(std::vector<D3DXVECTOR2> points, D3DXCOLOR color = 0xFFFFFFFF) {
	//}
}
