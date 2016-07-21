#include "GameApp.h"

#include "Graphics.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"

GameApp::GameApp(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0)
{
	m_graphics = std::make_shared<Graphics>(hWnd);
	m_textureManager = std::make_shared<TextureManager>(m_graphics);
	m_soundManager = std::make_shared<SoundManager>();
	m_inputManager = std::make_shared<InputManager>(hWnd, hInstance);
	m_fpsControler = std::make_shared<FPSControler>(60);
	m_random = std::make_shared<Random>();
}

void GameApp::run() {
	m_frameCount++;
	m_inputManager->update();
	update();
	if (SUCCEEDED(m_graphics->beginScene())) {
		draw();
		m_graphics->endScene();
	}
	m_graphics->present();
	m_fpsControler->wait();
}
