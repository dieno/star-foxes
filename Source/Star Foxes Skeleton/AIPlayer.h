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

enum EBehaviour
{
   FLEE,
   WAND,
   ATCK,
   SEEK
};

typedef struct Movement
{
   int count;
   int dir;
   float top; // top bound
   float bottom; // bottom bound
   float left; // left bound
   float right; // right bound
}*PMovement;

class AIPlayer: public MainPlayerClass
{
private:
   EBehaviour _behave;
   PMovement _mv;
public:
   void Evaluate();
   void Wander(HWND hWnd);
   void Flee(HWND hWnd, D3DXVECTOR3 pos);
   void Update(HWND hWnd, D3DXVECTOR3 pos);
   EDir Move(HWND hWnd, int dir, bool* outbound);
   void SetBehaviour(EBehaviour beh);
   void SetBounds(D3DXVECTOR3 pos);
  
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
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)) {
   _mv = new Movement();
   }
	
	AIPlayer(MainShipClass shiptype, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, shiptype) {
   _mv = new Movement();
   }

	   AIPlayer():MainPlayerClass(){
      _mv = new Movement();
   }   

   ~AIPlayer(void);
};
