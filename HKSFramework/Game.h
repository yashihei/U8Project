#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

class Direct3D;
class ImageManager;
class AnimationImage;
class Keyboard;
class Audio;

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
	std::shared_ptr<AnimationImage> m_anmImage;
	std::shared_ptr<Keyboard> m_keyboard;
	std::shared_ptr<Audio> m_audio;
	int m_frameCount;
};
