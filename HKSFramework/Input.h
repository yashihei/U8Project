#pragma once
#include <Windows.h>
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>
#include <XInput.h>

class Keyboard {
public:
	Keyboard(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance);
	~Keyboard();
	void updateState();
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
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	std::array<std::bitset<StateNum>, KeyNum> m_state;
};

class Mouse {
public:
	enum class Button {
		Left,
		Right,
		Center,
	};
	Mouse(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance);
	~Mouse();
	void updateState();
private:
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
};

class XInput {
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
		A, B, X, Y,
	};

	XInput(int index = 0);
	~XInput();
	void updateState();
	bool isClicked(Button button);
	bool isPressed(Button button);
	bool isReleased(Button button);
	//Vec2 getLeftThumbDir();
	//Vec2 getRightThumbDir();
private:
	int m_index;

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

class InputManager {
public:
	InputManager(HWND hWnd, HINSTANCE hInstance);
	~InputManager();
	void update();
private:
	LPDIRECTINPUT8 m_directInput;
	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Mouse> m_mouse;
	std::shared_ptr<XInput> m_xInput;
};
