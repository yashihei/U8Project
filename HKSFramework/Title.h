#pragma once
#include <d3dx9.h>
#include <memory>
#include "Scene.h"
#include "Play.h"
#include "Font.h"
#include "Input.h"

class Title : public Scene {
public:
	Title(std::shared_ptr<InputManager> input, LPDIRECT3DDEVICE9 d3dDevice) :
		m_inputManager(input)
	{
		m_font = std::make_shared<Font>(50, "メイリオ", true, d3dDevice);
	}
	void update() override {
		if (m_inputManager->isClickedButton1())
			moveTo(SceneType::Play);
	}
	void draw() override {
		m_font->drawStr("タイトルだよっ☆", { 0, 0 });
	}
private:
	std::shared_ptr<Font> m_font;
	std::shared_ptr<InputManager> m_inputManager;
};
