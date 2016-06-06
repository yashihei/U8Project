#include "Game.h"
#include "Direct3D.h"

Game::Game(HWND hWnd) {
	m_direct3d = std::make_shared<Direct3D>(hWnd);
}

Game::~Game() {}

void Game::update() {
}

void Game::draw() {
	if (SUCCEEDED(m_direct3d->beginScene())) {
		m_direct3d->endScene();
	}
	m_direct3d->present();
}
