#pragma once
#include <memory>
#include <d3dx9.h>
#include "Actor.h"
#include "GameApp.h"

enum class SceneType;
class Scene;

class StgGame : public GameApp {
public:
	StgGame(HWND hWnd, HINSTANCE hInstance);
private:
	void update() override;
	void draw() override;
	void changeScene(SceneType type);
	std::shared_ptr<Scene> m_scene;
};
