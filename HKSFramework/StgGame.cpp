#include "StgGame.h"

#include "Graphic.h"
#include "Texture.h"
#include "Input.h"
#include "Sound.h"
#include "FPS.h"
#include "Random.h"
#include "Util.h"
#include "Actors.h"
#include "Font.h"
#include "Title.h"

StgGame::StgGame(HWND hWnd, HINSTANCE hInstance) :
GameApp(hWnd, hInstance) 
{
	m_textureManager->load("dat/car000.png", "car");
	m_textureManager->load("dat/gameover.png", "gameover");
	m_soundManager->load("dat/startup.wav", "start");
	m_soundManager->load("dat/burn.wav", "burn");
	m_scene = std::make_shared<Title>(m_inputManager, m_graphicDevice->getDevice());
}

void StgGame::update() {
	m_scene->update();
	if (m_scene->nextScene() != SceneType::None)
		changeScene(m_scene->nextScene());
}

void StgGame::draw() {
	m_scene->draw();
}

void StgGame::changeScene(SceneType type) {
	switch (type)
	{
	case SceneType::Title:
		m_scene = std::make_shared<Title>(m_inputManager, m_graphicDevice->getDevice());
		break;
	case SceneType::Play:
		m_scene = std::make_shared<Play>(m_inputManager, m_soundManager, m_graphicDevice->getDevice());
		break;
	}
}
