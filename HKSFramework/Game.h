#pragma once
#include <Windows.h>
#include <memory>

class Direct3D;
class TextureManager;
class SoundManager;
class InputManager;
class FPSControler;
class Random;

#include <d3dx9.h>
#include "Actor.h"
class Player;
class Shot;
class Enemy;

class GameApp {
public:
	GameApp(HWND hWnd, HINSTANCE hInstance);
	~GameApp();
	void run();

	void addShot(D3DXVECTOR2 pos, D3DXVECTOR2 vec);
	D3DXVECTOR2 getPlayerPos();
private:
	void update();
	void draw();

	std::shared_ptr<Direct3D> m_direct3d;
	std::shared_ptr<TextureManager> m_textureManager;
	std::shared_ptr<SoundManager> m_soundManager;
	std::shared_ptr<InputManager> m_inputManager;
	std::shared_ptr<FPSControler> m_fpsControler;
	std::shared_ptr<Random> m_random;
	int m_frameCount;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<ActorManager<Shot>> m_shots;
	std::shared_ptr<ActorManager<Enemy>> m_enemies;
	bool m_gameover;
};
