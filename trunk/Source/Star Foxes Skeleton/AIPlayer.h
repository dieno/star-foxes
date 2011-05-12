//#pragma once
//#include "directXHeader.h"
#include "MainPlayerClass.h"
#include <time.h>
#include <math.h>

enum EDir
{
   UP = 0,
   DWN = 1,
   LFT = 2,
   RGHT = 3,
   DIR_NONE
};

class AIPlayer: public MainPlayerClass
{
private:

public:
   void Evaluate();
   void Wander(HWND hWnd);
   void Flee(HWND hWnd, D3DXVECTOR3 pos);
   void Update(HWND hWnd, D3DXVECTOR3 pos);
   EDir Move(HWND hWnd, int dir, bool* outbound);
  
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

