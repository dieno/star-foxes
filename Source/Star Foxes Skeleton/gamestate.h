#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "directXHeader.h"
#include "mainPlayerClass.h"

//Holds on gamestate information relevant to AI - player locations, player health, special events (bombs), etc
class GameState 
{
private:
	MainPlayerClass *players[8];
	HWND hWnd_;

public:

	//Take in relevant game variables and update them within this object.
	void updateGameState(HWND hWnd, MainPlayerClass *enemy[]);

	//Returns player at specified index.
	MainPlayerClass getPlayer(int idx);

	//Returns hWnd
	HWND getHWND();
};

#endif