#pragma once
#include <Windows.h>
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>
#include <XInput.h>

class Keyboard {
public:
	Keyboard(HWND hWnd, HINSTANCE hInstance);
	~Keyboard();
	void update();
	bool isClicked(BYTE code);
	bool isPressed(BYTE code);
	bool isReleased(BYTE code);
private:
	//bitset & enumでキーのフラグ管理
	enum State {
		Prev,
		Press,
		Click,
		Release,
		StateNum
	};
	static const int KeyNum = 256;
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	std::array<std::bitset<StateNum>, KeyNum> m_state;
};

class Mouse {
public:
	Mouse(HWND hWnd, HINSTANCE hInstance);
	~Mouse();
private:
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
};

enum class Button {
	Up,
	Down,
	Left,
	Right,
	Start,
	Back,
	LeftThumb,
	RightThumb,
	LeftShoulder,
	RightShoulder,
	A, B, X, Y
};

class XInput {
public:
	XInput(int index = 0);
	~XInput();
	void update();
	bool isClicked(Button button);
	bool isPressed(Button button);
	bool isReleased(Button button);
	//Vec2 getLeftThumbDir();
	//Vec2 getRightThumbDir();
private:
	int m_index;
	XINPUT_STATE m_xInputState;

	enum State {
		Prev,
		Press,
		Click,
		Release,
		StateNum
	};
	static const int buttonNum = 14;
	std::array<std::bitset<StateNum>, buttonNum> m_state;
};
