#include "Input.h"

#include <stdexcept>

Keyboard::Keyboard(HWND hWnd, HINSTANCE hInstance) :
m_directInput(NULL), m_directInputDevice(NULL)
{
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	auto hr = m_directInput->CreateDevice(GUID_SysKeyboard, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	//データ形式の設定
	m_directInputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");
	//強調レベルの設定
	m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");

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

Mouse::Mouse(HWND hWnd, HINSTANCE hInstance)
{
	//FIXME
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	auto hr = m_directInput->CreateDevice(GUID_SysMouse, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	hr = m_directInputDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");

	hr = m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");
}

Mouse::~Mouse() {
	if (m_directInputDevice)
		m_directInputDevice->Release();
	if (m_directInput)
		m_directInput->Release();
}

XInput::XInput(int index) :
m_index(index), m_xInputState{}
{
}

void XInput::update() {
	XInputGetState(m_index, &m_xInputState);

	int buttonIndex = 1;
	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (m_xInputState.Gamepad.wButtons & buttonIndex) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
		buttonIndex *= (buttonIndex != 0x0400) ? 2 : 8;
	}
}
