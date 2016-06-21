#include "Input.h"

#include <stdexcept>

Keyboard::Keyboard(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance) :
m_directInputDevice(NULL)
{
	auto hr = directInput->CreateDevice(GUID_SysKeyboard, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	//データ形式の設定
	hr = m_directInputDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");
	//強調レベルの設定
	hr = m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");

	//キーボード入力を有効化
	m_directInputDevice->Acquire();
}

Keyboard::~Keyboard() {
	if (m_directInputDevice)
		m_directInputDevice->Release();
}

void Keyboard::updateState() {
	std::array<BYTE, KeyNum> buf;
	m_directInputDevice->GetDeviceState(buf.size(), buf.data());
	//TODO:繋がっていなかった場合の処理

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

Mouse::Mouse(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance) :
m_directInputDevice(NULL)
{
	auto hr = directInput->CreateDevice(GUID_SysMouse, &m_directInputDevice, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error create DirectInput Device");

	//mouse→ボタン4つ mouse2→最大ボタン8つ
	hr = m_directInputDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		throw std::runtime_error("Error setDataFormat");

	hr = m_directInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		throw std::runtime_error("Error setCooperativeLevel");

	m_directInputDevice->Acquire();
}

Mouse::~Mouse() {
	if (m_directInputDevice)
		m_directInputDevice->Release();
}

void Mouse::updateState() {
	DIMOUSESTATE mouseState = {};
	m_directInputDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
	//TODO:繋がっていなかった場合の処理
}

//MEMO:XInput、win7ではlibのバージョンで動かないことがあるらしいので注意
XInput::XInput(int index) :
m_index(index)
{
}

void XInput::updateState() {
	//TODO:繋がっていなかった場合の処理
	XINPUT_STATE xInputState = {};
	XInputGetState(m_index, &xInputState);

	int buttonIndex = 1;
	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (xInputState.Gamepad.wButtons & buttonIndex) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
		buttonIndex *= (buttonIndex != 0x0400) ? 2 : 8;
	}
}

bool XInput::isPressed(Button button) {
	return m_state[static_cast<int>(button)][Press];
}

bool XInput::isClicked(Button button) {
	return m_state[static_cast<int>(button)][Press];
}

bool XInput::isReleased(Button button) {
	return m_state[static_cast<int>(button)][Press];
}

InputManager::InputManager(HWND hWnd, HINSTANCE hInstance) {
	auto hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize DirectInput");

	m_keyboard = std::make_shared<Keyboard>(m_directInput, hWnd, hInstance);
	m_mouse = std::make_shared<Mouse>(m_directInput, hWnd, hInstance);
	m_xInput = std::make_shared<XInput>();
}

InputManager::~InputManager() {
	if (m_directInput)
		m_directInput->Release();
}
