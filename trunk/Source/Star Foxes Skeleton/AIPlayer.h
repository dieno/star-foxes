//#pragma once
//#include "directXHeader.h"
#include "directXClass.h"
//#include "MainPlayerClass.h"
#include <math.h>

#ifndef AIPLAYER
#define AIPLAYER

enum EDir
{
   UP = 0,
   DWN = 1,
   LFT = 2,
   RGHT = 3,
   UPRGHT = 4,
   UPLEFT = 5,
   DWNRGHT = 6,
   DWNLFT = 7,
   FWRD = 8,
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
   int count; // count of movement
   int dir; // direction of movement
   float top; // top bound
   float bottom; // bottom bound
   float left; // left bound
   float right; // right bound
}*PMovement;

class AIPlayer: public MainPlayerClass
{
private:
   EBehaviour _behave;
   
public:
   PMovement _mv;
   void Rotate2DvectorYZ(D3DXVECTOR3* pV2, float angle);
   void Rotate2DvectorXZ(D3DXVECTOR3* pV2, float angle);
   void Evaluate();
   void Seek(D3DXVECTOR3 enemyPos);
   void SeekIra(D3DXVECTOR3 enemyPos);
   void Wander(HWND hWnd);
   void Flee(HWND hWnd, D3DXVECTOR3 pos);
   void Update(HWND hWnd, D3DXVECTOR3 pos);
   EDir Move(HWND hWnd, int dir, bool* outbound);
   void SetBehaviour(EBehaviour beh);
   void SetBounds(D3DXVECTOR3 pos);
   void IniAI();
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
    IniAI();
   }

	AIPlayer(
		PMESHSTRUCT meshStruct,
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(meshStruct, newg_pDevice)) {
    IniAI();
   }
	
	AIPlayer(MainShipClass shiptype, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, shiptype) {
      IniAI();
   }

	AIPlayer():MainPlayerClass(){
      IniAI();
   }

   ~AIPlayer(void);
};

#endif
