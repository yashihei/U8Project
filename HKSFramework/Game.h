#pragma once
#include <Windows.h>
#include <memory>

class Direct3D;

class Game {
public:
	Game();
	~Game();

	void init(HWND hWnd);
	void run() {
		update();
		draw();
	}
private:
	void update();
	void draw();

	std::shared_ptr<Direct3D> m_direct3d;
};
