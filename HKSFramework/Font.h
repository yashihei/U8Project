#pragma once
#include <d3dx9.h>
#include <string>
#include "Point.h"

class Font {
public:
	Font(int size, std::string fontName, bool bold, LPDIRECT3DDEVICE9 device) {
		HRESULT hr = D3DXCreateFont(device,
			size, 0, bold ? FW_NORMAL : FW_BOLD, 0, FALSE,
			SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
			fontName.c_str(),
			&m_font);
		if (FAILED(hr))
			throw std::runtime_error("Error createfont");
	}
	~Font() {
		if (m_font)
			m_font->Release();
	}
	void drawStr(Point pos, std::string str, D3DCOLOR color) {
		RECT rect = { pos.x, pos.y, 0, 0 };
		m_font->DrawText(NULL, str.c_str(), -1, &rect, DT_NOCLIP, color);
	}
private:
	ID3DXFont* m_font;
};
