#include "gamestate.h"

void GameState::updateGameState(HWND hWnd, MainPlayerClass* enemy[]) {
	hWnd_ = hWnd;
	for (int index = 0; index < 8; index++) {
		players[index] = enemy[index];
	}
}

MainPlayerClass* GameState::getPlayer(int idx) {
	return players[idx];
}

HWND GameState::getHWND() {
	return hWnd_;
}

