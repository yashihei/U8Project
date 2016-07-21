#include "Game.h"

#include "Direct3D.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"

GameApp::GameApp(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0)
{
	m_direct3d = std::make_shared<Direct3D>(hWnd);

	m_textureManager = std::make_shared<TextureManager>(m_direct3d->getDevice());
	m_textureManager->load("dat/car000.png", "car");
	m_textureManager->load("dat/gameover.png", "gameover");

	m_soundManager = std::make_shared<SoundManager>();
	m_soundManager->load("dat/startup.wav", "start");
	m_soundManager->load("dat/burn.wav", "burn");

	m_inputManager = std::make_shared<InputManager>(hWnd, hInstance);
	m_fpsControler = std::make_shared<FPSControler>(60);
	m_random = std::make_shared<Random>();
}

void GameApp::run() {
	m_frameCount++;
	m_inputManager->update();
	update();
	if (SUCCEEDED(m_direct3d->beginScene())) {
		draw();
		m_direct3d->endScene();
	}
	m_direct3d->present();
	m_fpsControler->wait();
}
