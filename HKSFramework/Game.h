#pragma once
#include <Windows.h>
#include <memory>

class Direct3D;

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
};
