#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

class Direct3D;
class ImageManager;
class AnimationImage;
class AudioManager;
class InputManager;

class Game {
public:
	Game(HWND hWnd, HINSTANCE hInstance);
	~Game();

	void run() {
		update();
		draw();
	}
private:
	void update();
	void draw();

	std::shared_ptr<Direct3D> m_direct3d;
	std::shared_ptr<ImageManager> m_imageManager;
	std::shared_ptr<AudioManager> m_audioManager;
	std::shared_ptr<InputManager> m_inputManager;

	std::shared_ptr<AnimationImage> m_anmImage;
	int m_frameCount;
};
