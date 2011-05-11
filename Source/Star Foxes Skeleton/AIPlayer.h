//#pragma once
//#include "directXHeader.h"
#include "MainPlayerClass.h"
#include <time.h>

class AIPlayer: public MainPlayerClass
{
private:

public:
   void Evaluate();
   void Wander(HWND hWnd);
   void Update(HWND hWnd);
   //AI(void);  

	AIPlayer(
		LPD3DXMESH mesh, 
		D3DMATERIAL9* meshMat, 
		LPDIRECT3DTEXTURE9* meshTex, 
		DWORD meshNumMat, 
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)) {}
	
	AIPlayer(MainShipClass shiptype, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, shiptype) {}

	AIPlayer():MainPlayerClass(){}

   ~AIPlayer(void);
};

