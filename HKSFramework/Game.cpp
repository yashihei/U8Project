#include "Game.h"
#include "Direct3D.h"

Game::Game() {}

Game::~Game() {}

void Game::init(HWND hWnd) {
	m_direct3d = std::make_shared<Direct3D>();
	m_direct3d->init(hWnd);
}

void Game::update() {
}

void Game::draw() {
}
