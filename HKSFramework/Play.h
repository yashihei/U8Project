#pragma once
#include <d3d9.h>
#include <memory>
#include "Scene.h"
#include "Actor.h"

class InputManager;
class SoundManager;
class Random;
class Player;
class Shot;
class Enemy;

class Play : public Scene {
public:
	Play(std::shared_ptr<InputManager> inputManager, std::shared_ptr<SoundManager> soundManager, LPDIRECT3DDEVICE9 d3dDevice);
	void update() override;
	void draw() override;

	std::shared_ptr<Player> getPlayer() { return m_player; }
	std::shared_ptr<ActorManager<Shot>> getShots() { return m_shots; }
private:
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<SoundManager> m_soundManager;
	LPDIRECT3DDEVICE9 m_d3dDevice;
	std::shared_ptr<Random> m_random;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<ActorManager<Shot>> m_shots;
	std::shared_ptr<ActorManager<Enemy>> m_enemies;
};
