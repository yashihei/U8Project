#pragma once
#include <Windows.h>
#include <memory>

class Direct3D;
class ImageManager;
class SoundManager;
class InputManager;
class FPSControler;
class Random;
class Tewi;

class Game {
public:
	Game(HWND hWnd, HINSTANCE hInstance);
	~Game();
	void run();
private:
	void update();
	void draw();

	std::shared_ptr<Direct3D> m_direct3d;
	std::shared_ptr<ImageManager> m_imageManager;
	std::shared_ptr<SoundManager> m_soundManager;
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<FPSControler> m_fpsControler;
	std::shared_ptr<Random> m_random;
	std::shared_ptr<Tewi> m_tewi;
	int m_frameCount;
};
