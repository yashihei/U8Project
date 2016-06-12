#include "Input.h"

#include <stdexcept>

Keyboard::Keyboard(HWND hWnd, HINSTANCE hInstance) :
m_DirectInput(NULL), m_DirectInputDevice(NULL)
{
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	hr = m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_DirectInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	//データ形式の設定
	m_DirectInputDevice->SetDataFormat(&c_dfDIKeyboard);
	//強調レベルの設定
	m_DirectInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	//キーボード入力を有効化
	m_DirectInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (m_DirectInput)
		m_DirectInput->Release();
	if (m_DirectInputDevice)
		m_DirectInputDevice->Release();
}

void Keyboard::update() {
	std::array<BYTE, KeyNum> buf;
	m_DirectInputDevice->GetDeviceState(buf.size(), buf.data());

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
