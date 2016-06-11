#include "Game.h"

#include "Direct3D.h"
#include "Image.h"

Game::Game(HWND hWnd) {
	m_direct3d = std::make_shared<Direct3D>(hWnd);
	m_image = std::make_shared<Image>("dora01.png", m_direct3d->getDevice());
	m_anmImage = std::make_shared<AnimationImage>(m_image, 3, 4, 3, 20);
}

Game::~Game() {}

void Game::update() {
}

void Game::draw() {
	if (SUCCEEDED(m_direct3d->beginScene())) {
		m_anmImage->update();
		m_anmImage->draw({ 320, 240 });
		m_direct3d->endScene();
	}
	m_direct3d->present();
}
