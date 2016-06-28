#include "Game.h"

#include "Direct3D.h"
#include "Image.h"
#include "Input.h"
#include "Audio.h"
#include "FPS.h"
#include "Random.h"

#include "Actor.h"

Game::Game(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0), m_missNum(0), m_isGameOver(false)
{
	m_direct3d = std::make_shared<Direct3D>(hWnd);

	m_imageManager = std::make_shared<ImageManager>(m_direct3d->getDevice());
	m_imageManager->preLoad("dat/background.jpg", "background");
	m_imageManager->preLoad("dat/tewi_material01.png", "tewi01");
	m_imageManager->preLoad("dat/error.png", "error");
	m_imageManager->preLoad("dat/BSOD.png", "BSOD");

	m_audioManager = std::make_shared<AudioManager>();
	m_audioManager->loadWave("dat/startup.wav", "start");
	m_audioManager->loadWave("dat/critical.wav", "critical");
	m_audioManager->play("start");

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
	if (m_random->next(100) == 0) {
		auto item = std::make_shared<Item>(D3DXVECTOR2(m_random->next(0.0, 640.0), 0), m_imageManager);
		m_items.push_back(item);
	}
	for (auto& item : m_items) {
		item->update();

		if (std::pow(item->getPos().x - m_tewi->getPos().x, 2.0) + std::pow(item->getPos().y - m_tewi->getPos().y, 2.0) <= std::pow(50.0, 2.0)) {
			item->kill();
		}
		if (item->getPos().y > 480) {
			item->kill();
			m_missNum++;
		}
	}
	Util::Erase_if(m_items, [](std::shared_ptr<Item> item) {return !item->isEnabled();});
	if (m_missNum > 15) {
		m_audioManager->play("critical");
		m_isGameOver = true;
	}
	if (m_isGameOver && m_inputManager->isClickedButton1()) {
		m_isGameOver = false;
		m_missNum = 0;
	}
}

void Game::draw() {
	m_imageManager->getImage("background")->draw({ 320, 240 });
	m_tewi->draw();
	for (auto& item : m_items)
		item->draw();
	if (m_isGameOver)
		m_imageManager->getImage("BSOD")->draw({ 320, 240 });
}
