#pragma once
#include <Windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>
#include <XInput.h>
#include "Point.h"

//TODO:–¼‘O‹óŠÔ‚ÅˆÍ‚Þ
enum State {
	Prev,
	Press,
	Click,
	Release,
	StateNum
};

class Keyboard {
public:
	Keyboard(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance);
	~Keyboard();
	void updateState();
	bool isClicked(BYTE code) { return m_state[code][Press]; }
	bool isPressed(BYTE code) { return m_state[code][Click]; }
	bool isReleased(BYTE code) { return m_state[code][Release]; }
private:
	static const int KeyNum = 256;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	std::array<std::bitset<StateNum>, KeyNum> m_state;
};

class Mouse {
public:
	enum Button {
		Left,
		Right,
		Center,
	};
	Mouse(LPDIRECTINPUT8 directInput, HWND hWnd, HINSTANCE hInstance);
	~Mouse();
	void updateState();
	Point getCursorPos() { return m_cursorPos; }
private:
	static const int buttonNum = 3;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
	HWND m_hWnd;
	Point m_cursorPos;
	std::array<std::bitset<StateNum>, buttonNum> m_state;
};

class XInput {
public:
	enum Button {
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
	void updateState();
	bool isClicked(Button button) { return m_state[button][Press]; }
	bool isPressed(Button button) { return m_state[button][Press]; }
	bool isReleased(Button button) { return m_state[button][Press]; }
	//Vec2 getLeftThumbDir();
	//Vec2 getRightThumbDir();
private:
	static const int buttonNum = 14;
	int m_index;
	std::array<std::bitset<StateNum>, buttonNum> m_state;
};

class InputManager {
public:
	InputManager(HWND hWnd, HINSTANCE hInstance);
	~InputManager();
	void update();

	bool isPressedButton1() const { return m_keyboard->isPressed(DIK_Z) | m_xInput->isPressed(XInput::A); }
	bool isPressedButton2() const { return m_keyboard->isPressed(DIK_X) | m_xInput->isPressed(XInput::B); }
	bool isPressedButton3() const { return m_keyboard->isPressed(DIK_C) | m_xInput->isPressed(XInput::X); }
	bool isPressedButton4() const { return m_keyboard->isPressed(DIK_V) | m_xInput->isPressed(XInput::Y); }
	bool isPressedUp()      const { return m_keyboard->isPressed(DIK_UP) | m_xInput->isPressed(XInput::Up); }
	bool isPressedDown()    const { return m_keyboard->isPressed(DIK_DOWN) | m_xInput->isPressed(XInput::Down); }
	bool isPressedLeft()    const { return m_keyboard->isPressed(DIK_LEFT) | m_xInput->isPressed(XInput::Left); }
	bool isPressedRight()   const { return m_keyboard->isPressed(DIK_RIGHT) | m_xInput->isPressed(XInput::Right); }

	bool isClickedButton1() const { return m_keyboard->isClicked(DIK_Z) | m_xInput->isClicked(XInput::A); };
	bool isClickedButton2() const { return m_keyboard->isClicked(DIK_X) | m_xInput->isClicked(XInput::B); };
	bool isClickedButton3() const { return m_keyboard->isClicked(DIK_C) | m_xInput->isClicked(XInput::X); };
	bool isClickedButton4() const { return m_keyboard->isClicked(DIK_V) | m_xInput->isClicked(XInput::Y); };
	bool isClickedUp()      const { return m_keyboard->isClicked(DIK_UP) | m_xInput->isClicked(XInput::Up); }
	bool isClickedDown()    const { return m_keyboard->isClicked(DIK_DOWN) | m_xInput->isClicked(XInput::Down); }
	bool isClickedLeft()    const { return m_keyboard->isClicked(DIK_LEFT) | m_xInput->isClicked(XInput::Left); }
	bool isClickedRIght()   const { return m_keyboard->isClicked(DIK_RIGHT) | m_xInput->isClicked(XInput::Right); }
private:
	LPDIRECTINPUT8 m_directInput;
	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Mouse> m_mouse;
	std::shared_ptr<XInput> m_xInput;
};
