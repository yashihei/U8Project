#pragma once
#include <d3dx9.h>
#include "Actor.h"
#include "Input.h"
#include "Shape.h"
#include "StgGame.h"

class Player {
public:
	Player(StgGame* game, std::shared_ptr<InputManager> inputManager) :
	m_pos(320, 240), m_vec(0.0, 0.0), m_frameCount(0), m_game(game), m_inputManager(inputManager)
	{}
	void update() {
		m_frameCount++;

		auto pad = m_inputManager->getXInput();
		m_vec = pad->getLeftThumb() * 5;
		m_pos += m_vec;

		if (pad->getRightThumb() != D3DXVECTOR2(0.0f, 0.0f)) {
			if (m_frameCount % 5 == 0) {
				m_game->addShot(m_pos, pad->getRightThumb() * 10);
			}
		}
	}
	void draw() {
		float rad = std::atan2(m_vec.y, m_vec.x);
		Shape::drawNgon(m_pos, 3, 20, rad, D3DCOLOR_ARGB(122, 150, 150, 255));
	}
	D3DXVECTOR2 getPos() { return m_pos; }
private:
	D3DXVECTOR2 m_pos, m_vec;
	int m_frameCount;
	StgGame* m_game;
	std::shared_ptr<InputManager> m_inputManager;
};

class Shot : public Actor {
public:
	Shot(D3DXVECTOR2 pos, D3DXVECTOR2 vec) : m_pos(pos), m_vec(vec) {}
	void update() override {
		m_pos += m_vec;
	}
	void draw() override {
		Shape::drawCircle(m_pos, 4, D3DCOLOR_ARGB(200, 255, 165, 30));
	}
	D3DXVECTOR2 getPos() { return m_pos; }
private:
	D3DXVECTOR2 m_pos, m_vec;
};

class Enemy : public Actor {
public:
	Enemy(D3DXVECTOR2 pos, StgGame* game) : m_pos(pos), m_rad(0), m_game(game) {}
	void update() override {
		auto dis = m_game->getPlayerPos() - m_pos;
		float rad = std::atan2(dis.y, dis.x);
		m_pos += D3DXVECTOR2(std::cos(rad), std::sin(rad)) * 2;
		m_rad += 0.1f;
	}
	void draw() override {
		Shape::drawNgon(m_pos, 6, 20, m_rad, D3DCOLOR_ARGB(122, 255, 100, 100));
	}
	D3DXVECTOR2 getPos() { return m_pos; }
private:
	D3DXVECTOR2 m_pos;
	float m_rad;
	StgGame* m_game;
};
