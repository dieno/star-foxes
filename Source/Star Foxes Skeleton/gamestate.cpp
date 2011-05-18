#include "gamestate.h"

void GameState::updateGameState(HWND hWnd, MainPlayerClass* enemy[], int numberOfPlayers) {
	hWnd_ = hWnd;
	numberOfPlayers = numberOfPlayers;
	for (int index = 0; index < numberOfPlayers; index++) {
		players[index] = enemy[index];
	}
}

MainPlayerClass GameState::getPlayer(int idx) {
	return *players[idx];
}

HWND GameState::getHWND() {
	return hWnd_;
}

