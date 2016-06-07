#include "Game.h"
#include "Direct3D.h"
#include "Image.h"

Game::Game(HWND hWnd) {
	m_direct3d = std::make_shared<Direct3D>(hWnd);
	m_image = std::make_shared<Image>("car000.png", m_direct3d->getDevice());
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
