#include "StgGame.h"

#include "Graphics.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"
#include "Util.h"
#include "Actors.h"

StgGame::StgGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance)
{
	m_player = std::make_shared<Player>(this, m_inputManager, m_graphics);
	m_shots = std::make_shared<ActorManager<Shot>>();
	m_enemies = std::make_shared<ActorManager<Enemy>>();
}

bool isHit(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void StgGame::update() {
	if (m_gameover) {
		if (m_inputManager->isClickedButton3()) {
			m_gameover = false;
			m_enemies->clear();
			m_shots->clear();
		}
		return;
	}
	if (m_random->next(0, 60) == 0) {
		auto enemy = std::make_shared<Enemy>(D3DXVECTOR2(m_random->next(0.0f, 640.0f), m_random->next(0.0f, 480.0f)), this, m_graphics);
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

void StgGame::draw() {
	for (int i = 0; i < 16; i++)
		m_graphics->drawLine({ i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	for (int i = 0; i < 12; i++)
		m_graphics->drawLine({ 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	m_player->draw();
	m_shots->draw();
	m_enemies->draw();
	if (m_gameover)
		m_textureManager->getTexture("gameover")->draw({ 320, 240 });
}

void StgGame::addShot(D3DXVECTOR2 pos, D3DXVECTOR2 vec) {
	auto shot = std::make_shared<Shot>(pos, vec, m_graphics);
	m_shots->add(shot);
}

D3DXVECTOR2 StgGame::getPlayerPos() {
	return m_player->getPos();
}
