#pragma once
#include <Windows.h>
#include <memory>
#include <vector>
#include <list>

class Direct3D;
class ImageManager;
class AnimationImage;
class AudioManager;
class InputManager;
class FPSControler;
class Random;
class Tewi;
class Item;

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
	std::shared_ptr<AudioManager> m_audioManager;
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<FPSControler> m_fpsControler;
	std::shared_ptr<Random> m_random;
	std::shared_ptr<Tewi> m_tewi;
	std::list<std::shared_ptr<Item>> m_items;
	int m_frameCount, m_missNum;
	bool m_isGameOver;
};
