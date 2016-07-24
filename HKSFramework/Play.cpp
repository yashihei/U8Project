#include "Play.h"

#include "Actors.h"
#include "Random.h"
#include "Input.h"
#include "Sound.h"

Play::Play(std::shared_ptr<InputManager> inputManager, std::shared_ptr<SoundManager> soundManager, LPDIRECT3DDEVICE9 d3dDevice) :
	m_inputManager(inputManager), m_soundManager(soundManager), m_d3dDevice(d3dDevice)
{
	m_random = std::make_shared<Random>();
	m_player = std::make_shared<Player>(this, m_inputManager, m_d3dDevice);
	m_shots = std::make_shared<ActorManager<Shot>>();
	m_enemies = std::make_shared<ActorManager<Enemy>>();
}

bool isHit(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float radius1, float radius2) {
	if (std::pow(pos1.x - pos2.x, 2.0f) + std::pow(pos1.y - pos2.y, 2.0f) <= std::pow(radius1 + radius2, 2.0f))
		return true;
	return false;
}

void Play::update() {
	if (m_random->next(60) == 0) {
		auto enemy = std::make_shared<Enemy>(D3DXVECTOR2(m_random->next(640.0f), m_random->next(480.0f)), this, m_d3dDevice);
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
			moveTo(SceneType::Title);
	}
}

void Play::draw() {
	for (int i = 0; i < 16; i++)
		Shape::drawLine(m_d3dDevice, { i*40.0f, 0.0f }, { i*40.0f, 480.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	for (int i = 0; i < 12; i++)
		Shape::drawLine(m_d3dDevice, { 0.0f, i*40.0f }, { 640.f, i*40.0f }, 1.0f, D3DCOLOR_ARGB(32, 255, 255, 255));
	m_player->draw();
	m_shots->draw();
	m_enemies->draw();
}
