#pragma once
#include <memory>
#include <deque>

#include "Image.h"
#include "Input.h"
#include "Util.h"

class Tewi {
public:
	Tewi(std::shared_ptr<ImageManager> imageManager, std::shared_ptr<InputManager> inputManager) :
		m_inputManager(inputManager),
		m_pos(320, 400), m_vec(0, 0), m_cnt(0), m_jump(false), m_dir(true)
	{
		m_tewiAnm = std::make_shared<AnimationImage>(imageManager->getImage("tewi01"), 10, 8);
		m_tewiAnm->addPattern("wait", 0, 11);
		m_tewiAnm->addPattern("jump", { 30, 31, 32, 33, 34, 35, 36, 40, 41, 42 });
		m_tewiAnm->addPattern("squat", 20, 24);
		m_tewiAnm->addPattern("walk", 50, 59);
		m_tewiAnm->changePattern("wait");
	}
	void update() {
		m_cnt++;
		if (m_cnt % 5 == 0)
			m_tewiAnm->addvanceFrame();

		if (!m_jump) {
			if (m_inputManager->isPressedLeft() || m_inputManager->isPressedRight()) {
				if (!m_tewiAnm->isPlaying("walk"))
					m_tewiAnm->changePattern("walk");
			}
			if (m_inputManager->isReleasedLeft() || m_inputManager->isReleasedRight()) {
				m_tewiAnm->changePattern("wait");
			}
		}
		//move
		if (m_inputManager->isPressedLeft()) {
			m_dir = false;
			m_pos += D3DXVECTOR2(-5.0f, 0.0f);
		} else if (m_inputManager->isPressedRight()) {
			m_dir = true;
			m_pos += D3DXVECTOR2(5.0f, 0.0f);
		}
		m_pos.x = Util::clamp(m_pos.x, 0.0f, 640.0f);

		//jump
		m_vec += D3DXVECTOR2(0.0f, 0.7f);
		if (m_pos.y > 350) {
			m_vec = D3DXVECTOR2(0.0, 0.0);
			m_pos.y = 350;
			if (m_jump) {
				m_tewiAnm->changePattern("wait");
				m_jump = false;
			}
		}
		if (m_inputManager->isClickedButton1() || m_inputManager->isClickedUp()) {
			m_vec += D3DXVECTOR2(0.0, -15.0);
			m_tewiAnm->changePattern("jump");
			m_jump = true;
		}
		m_pos += m_vec;

		if (m_cnt % 2 == 0 && m_jump) {
			m_trail.push_front(m_pos);
			if (m_trail.size() > 5) m_trail.pop_back();
		}
		if (!m_jump)
			m_trail.clear();
	}
	void draw() {
		for (int i = m_trail.size() - 1; i > 0; i--) {
			float alpha = 1.0f - 0.2f * i;
			m_tewiAnm->draw(m_trail[i], 0.0, 1.5, D3DXCOLOR(1.0, 1.0, 1.0, alpha), !m_dir);
		}
		m_tewiAnm->draw(m_pos, 0.0, 1.5, D3DXCOLOR(1.0, 1.0, 1.0, 1.0), !m_dir);
	}
	D3DXVECTOR2 getPos() { return m_pos; }
private:
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<AnimationImage> m_tewiAnm;

	D3DXVECTOR2 m_pos, m_vec;
	bool m_jump;
	bool m_dir;
	int m_cnt;
	std::deque<D3DXVECTOR2> m_trail;
};
