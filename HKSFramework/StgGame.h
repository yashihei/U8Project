#pragma once
#include <memory>
#include <d3dx9.h>
#include "Actor.h"
#include "Game.h"

class Player;
class Shot;
class Enemy;

class StgGame : public GameApp {
public:
	StgGame(HWND hWnd, HINSTANCE hInstance);
	void addShot(D3DXVECTOR2 pos, D3DXVECTOR2 vec);
	D3DXVECTOR2 getPlayerPos();
private:
	void update() override;
	void draw() override;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<ActorManager<Shot>> m_shots;
	std::shared_ptr<ActorManager<Enemy>> m_enemies;
	bool m_gameover;
};
