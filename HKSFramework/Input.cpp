#include "Input.h"

#include <stdexcept>

Keyboard::Keyboard(HWND hWnd, HINSTANCE hInstance) :
m_directInput(NULL), m_directInputDevice(NULL)
{
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	//データ形式の設定
	m_directInputDevice->SetDataFormat(&c_dfDIKeyboard);
	//強調レベルの設定
	m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//キーボード入力を有効化
	m_directInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (m_directInputDevice)
		m_directInputDevice->Release();
	if (m_directInput)
		m_directInput->Release();
}

void Keyboard::update() {
	std::array<BYTE, KeyNum> buf;
	m_directInputDevice->GetDeviceState(buf.size(), buf.data());

	for (int i = 0; i < KeyNum; i++) {
		m_state[i][Press] = (buf[i] & 0x80) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
	}
}

bool Keyboard::isPressed(BYTE code) {
	return m_state[code][Press];
}

bool Keyboard::isClicked(BYTE code) {
	return m_state[code][Click];
}

bool Keyboard::isReleased(BYTE code) {
	return m_state[code][Release];
}
