#pragma once
#include <Windows.h>
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>

// MEMO
// DirectInputによるマウスとキーボードの入力は非推奨とされているので、メッセージループを使う予定
// コントローラはとりあえず、XInputのみで

class Keyboard {
public:
	Keyboard(HWND hWnd, HINSTANCE hInstance);
	~Keyboard();
	void update();
	bool isClicked(BYTE code);
	bool isPressed(BYTE code);
	bool isReleased(BYTE code);
private:
	enum State {
		Prev,
		Press,
		Click,
		Release,
		StateNum
	};
	static const int KeyNum = 256;
	LPDIRECTINPUT8 m_DirectInput;
	LPDIRECTINPUTDEVICE8 m_DirectInputDevice;
	std::array<std::bitset<StateNum>, KeyNum> m_state;
};

class Mouse {
public:
	Mouse(HWND hWnd);
	~Mouse();
private:
	LPDIRECTINPUT8 m_DirectInput;
	LPDIRECTINPUTDEVICE8 m_DirectInputDevice;
};

class Input {
public:
	Input() = default;
private:
	std::shared_ptr<Keyboard> m_keyboard;
	//マウスとコントローラーもここに
};
