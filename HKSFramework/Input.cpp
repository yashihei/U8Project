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

Mouse::Mouse(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance) :
m_directInputDevice(NULL), m_hWnd(hWnd), m_cursorPos(0, 0)
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

	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (mouseState.rgbButtons[i] & 0x80) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
	}

	//DirectInputでは相対値の座標しかとれないので、win32apiを使う
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hWnd, &cursorPos);
	m_cursorPos.x = cursorPos.x;
	m_cursorPos.y = cursorPos.y;
}

//MEMO:XInput、win7ではlibのバージョンで動かないことがあるらしいので注意
XInput::XInput(int index) :
m_index(index)
{
	m_state.fill(std::bitset<4>());
}

void XInput::updateState() {
	XINPUT_STATE xInputState = {};
	auto res = XInputGetState(m_index, &xInputState);
	if (res == ERROR_DEVICE_NOT_CONNECTED)
		return;
	//TODO:繋がっていなかった場合の処理

	int buttonIndex = 1;
	for (int i = 0; i < buttonNum; i++) {
		m_state[i][Press] = (xInputState.Gamepad.wButtons & buttonIndex) != 0;
		m_state[i][Click] = (m_state[i][Press] ^ m_state[i][Prev]) & m_state[i][Press];
		m_state[i][Release] = (m_state[i][Press] ^ m_state[i][Prev]) & !m_state[i][Press];
		m_state[i][Prev] = m_state[i][Press];
		buttonIndex *= (buttonIndex != 0x0400) ? 2 : 8;
	}
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

void InputManager::update() {
	m_keyboard->updateState();
	m_mouse->updateState();
	m_xInput->updateState();
}
