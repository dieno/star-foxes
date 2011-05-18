#include "gamestate.h"

void GameState::updateGameState(HWND hWnd, MainPlayerClass* enemy) {
	hWnd_ = hWnd;
	players = enemy;
}

MainPlayerClass GameState::getPlayer(int idx) {
	return players[idx];
}

HWND GameState::getHWND() {
	return hWnd_;
}

