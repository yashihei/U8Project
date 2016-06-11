#pragma once
#include <Windows.h>
#include <memory>
#include <vector>

class Direct3D;
class Image;
class AnimationImage;

class Game {
public:
	explicit Game(HWND hWnd);
	~Game();

	void run() {
		update();
		draw();
	}
private:
	void update();
	void draw();

	std::shared_ptr<Direct3D> m_direct3d;
	std::shared_ptr<Image> m_image;
	std::shared_ptr<AnimationImage> m_anmImage;
};
