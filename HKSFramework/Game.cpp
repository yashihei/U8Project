#include "Game.h"

#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "Audio.h"
#include "FPS.h"

Game::Game(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0)
{
	m_direct3d = std::make_shared<Direct3D>(hWnd);

	m_imageManager = std::make_shared<ImageManager>(m_direct3d->getDevice());
	m_imageManager->preLoad("dora01.png", "dragon");
	m_imageManager->preLoad("car000.png", "car");
	m_anmImage = std::make_shared<AnimationImage>(m_imageManager->getImage("dragon"), 3, 4, 20);

	m_audioManager = std::make_shared<AudioManager>();
	m_audioManager->loadWave("bgm.wav", "bgm");
	//m_audioManager->play("bgm");

	m_inputManager = std::make_shared<InputManager>(hWnd, hInstance);
	m_fpsControler = std::make_shared<FPSControler>(60);
}

Game::~Game() {}

void Game::run() {
	update();
	draw();
	m_fpsControler->wait();
}

void Game::update() {
	m_frameCount++;
	m_anmImage->update();
	m_inputManager->update();
}

void Game::draw() {
	if (SUCCEEDED(m_direct3d->beginScene())) {
		m_anmImage->draw({ 320, 240 });
		m_direct3d->endScene();
	}
	m_direct3d->present();
}
