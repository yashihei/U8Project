#include "Game.h"

#include "Direct3D.h"
#include "Image.h"
#include "Input.h"

Game::Game(HWND hWnd, HINSTANCE hInstance) {
	m_direct3d = std::make_shared<Direct3D>(hWnd);

	m_imageManager = std::make_shared<ImageManager>(m_direct3d->getDevice());
	m_imageManager->preLoad("dora01.png", "dragon");
	m_imageManager->preLoad("car000.png", "car");
	m_imageManager->preLoad("sysfont.tga", "font");
	m_anmImage = std::make_shared<AnimationImage>(m_imageManager->getImage("dragon"), 3, 4, 0, 20);

	m_keyboard = std::make_shared<Keyboard>(hWnd, hInstance);
}

Game::~Game() {}

void Game::update() {
	m_keyboard->update();
	m_anmImage->update();
}

void Game::draw() {
	if (SUCCEEDED(m_direct3d->beginScene())) {
		m_anmImage->draw({ 320, 240 });
		m_direct3d->endScene();
	}
	m_direct3d->present();
}
