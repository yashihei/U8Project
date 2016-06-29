#include "Game.h"

#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "Audio.h"
#include "FPS.h"
#include "Random.h"

#include "Actor.h"

Game::Game(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0)
{
	m_direct3d = std::make_shared<Direct3D>(hWnd);

	m_imageManager = std::make_shared<ImageManager>(m_direct3d->getDevice());
	m_imageManager->load("dat/background.jpg", "background");
	m_imageManager->load("dat/tewi_material01.png", "tewi01");

	m_audioManager = std::make_shared<AudioManager>();
	m_audioManager->loadWave("dat/startup.wav", "start");

	m_inputManager = std::make_shared<InputManager>(hWnd, hInstance);
	m_fpsControler = std::make_shared<FPSControler>(60);
	m_random = std::make_shared<Random>();
	
	m_tewi = std::make_shared<Tewi>(m_imageManager, m_inputManager);
}

Game::~Game() {}

void Game::run() {
	update();
	if (SUCCEEDED(m_direct3d->beginScene())) {
		draw();
		m_direct3d->endScene();
	}
	m_direct3d->present();
	m_fpsControler->wait();
}

void Game::update() {
	m_frameCount++;
	m_inputManager->update();

	m_tewi->update();
}

void Game::draw() {
	m_imageManager->getImage("background")->draw({ 320, 240 });
	m_tewi->draw();
}
