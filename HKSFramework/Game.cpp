#include "Game.h"

#include "Direct3D.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"
#include "Shape.h"
#include "Util.h"

#include "Actors.h"

GameApp::GameApp(HWND hWnd, HINSTANCE hInstance) :
m_frameCount(0), m_gameover(false)
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

	Shape::init(m_direct3d->getDevice());
	m_player = std::make_shared<Player>(this, m_inputManager);
	m_shots = std::make_shared<ActorManager<Shot>>();
	m_enemies = std::make_shared<ActorManager<Enemy>>();
}

GameApp::~GameApp() {}

void GameApp::run() {
	update();
	if (SUCCEEDED(m_direct3d->beginScene())) {
		draw();
		m_direct3d->endScene();
	}
	m_direct3d->present();
	m_fpsControler->wait();
}

bool isHit(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void GameApp::update() {
	m_frameCount++;
	m_inputManager->update();
	OutputDebugValue(m_fpsControler->getFps());

	if (m_gameover) {
		if (m_inputManager->isClickedButton3()) {
			m_gameover = false;
			m_enemies->clear();
			m_shots->clear();
		}
		return;
	}
	if (m_random->next(0, 60) == 0) {
		auto enemy = std::make_shared<Enemy>(D3DXVECTOR2(m_random->next(0.0f, 640.0f), m_random->next(0.0f, 480.0f)), this);
		m_enemies->add(enemy);
	}
	m_player->update();
	m_shots->update();
	m_enemies->update();
	for (auto& shot : *m_shots) {
		for (auto& enemy : *m_enemies) {
			if (isHit(enemy->getPos(), shot->getPos(), 10, 3)) {
				shot->kill();
				enemy->kill();
				m_soundManager->play("burn");
			}
		}
	}
	for (auto& enemy : *m_enemies) {
		if (isHit(enemy->getPos(), m_player->getPos(), 10, 3))
			m_gameover = true;
	}
}

void GameApp::draw() {
	for (int i = 0; i < 16; i++)
		Shape::drawLine({ i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	for (int i = 0; i < 12; i++)
		Shape::drawLine({ 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	m_player->draw();
	m_shots->draw();
	m_enemies->draw();
	if (m_gameover)
		m_textureManager->getTexture("gameover")->draw({ 320, 240 });
}

void GameApp::addShot(D3DXVECTOR2 pos, D3DXVECTOR2 vec) {
	auto shot = std::make_shared<Shot>(pos, vec);
	m_shots->add(shot);
}

D3DXVECTOR2 GameApp::getPlayerPos() {
	return m_player->getPos();
}
