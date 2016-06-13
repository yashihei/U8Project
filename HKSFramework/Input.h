#pragma once
#include <Windows.h>
#include <dinput.h>
#include <bitset>
#include <array>
#include <memory>

// MEMO
// DirectInput�ɂ��}�E�X�ƃL�[�{�[�h�̓��͂�MS����񐄏��Ƃ���Ă���̂ŁA���b�Z�[�W���[�v���g�������H
// �R���g���[���͂Ƃ肠�����AXInput�݂̂�

class Keyboard {
public:
	Keyboard(HWND hWnd, HINSTANCE hInstance);
	~Keyboard();
	void update();
	bool isClicked(BYTE code);
	bool isPressed(BYTE code);
	bool isReleased(BYTE code);
private:
	//bitset & enum�ŃL�[�̃t���O�Ǘ�
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
	Mouse(HWND hWnd);
	~Mouse();
private:
	LPDIRECTINPUT8 m_directInput;
	LPDIRECTINPUTDEVICE8 m_directInputDevice;
};

class Input {
public:
	Input() = default;
private:
	std::shared_ptr<Keyboard> m_keyboard;
	//�}�E�X�ƃR���g���[���[��������
};
