//#pragma once
#include "directXClass.h"
#include <math.h>
//#include "FSM.h"

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
   DIR_NONE
};

enum EState
{
   WAND = 0,
   FLEE,
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
   float front; // z positive bound
   float back; // z negative bound

   float straightUpLow; // These do nothing by now
   float straightUpHi;
   float straightDownLow;
   float straightDownHi;
}*PMovement;


/*
	FSM Class
		- The FSM class controls a set of states
		- All state transitions depend on how the states are configured
		- Configure a set of states and then upload them to the FSM
*/
class FSM
{
	private:
		EState currentState;
		GameState* gamestate;
		int masterIdx; //Index of controlling computer.

	public:
		FSM(GameState* _gamestate, int _masterIdx):gamestate(_gamestate), masterIdx(_masterIdx){
			currentState = WAND;
		};

		void setCurrentState(EState _state);

		int getMasterIdx();
		std::string getCurrentStateName();
		EState getCurrentState();
		GameState* getGameState() {
			return gamestate;	
		};

		float evalDistToTarget(D3DXVECTOR3 target, D3DXVECTOR3 origin);
		D3DXVECTOR3 eval();
		D3DXVECTOR3 evalFlee();
		D3DXVECTOR3 evalWander();
		D3DXVECTOR3 evalAttack();
		D3DXVECTOR3 evalSeek();
};

class AIPlayer: public MainPlayerClass
{
private:
   FSM _fsm;
   bool KeepInBounds(HWND hWnd);
   D3DXVECTOR3 _shootArea;
public:
   PMovement _mv;   
   bool StraightenUp();
   bool StraightenDown();
   //void Shoot();
   void Shoot(D3DXVECTOR3 target, float timeDelta);
   void GetMeToPosVector(D3DXVECTOR3* loc, D3DXVECTOR3* res, float* dist);
   void Rotate2DvectorYZ(D3DXVECTOR3* pV2, float angle);
   void Rotate2DvectorXZ(D3DXVECTOR3* pV2, float angle);
   void Evaluate();
   void Seek(D3DXVECTOR3 enemyPos);
   void Attack(D3DXVECTOR3 target, float timeDelta);
   void Wander(HWND hWnd);
   void Flee(HWND hWnd, D3DXVECTOR3 pos);
   virtual void Update(float timeDelta);
   EDir Move(HWND hWnd, int dir, bool* outbound);
   void SetBehaviour(EState beh);
   virtual void SetBounds(D3DXVECTOR3* pos);
   void IniAI();

	AIPlayer(
		LPD3DXMESH mesh, 
		D3DMATERIAL9* meshMat, 
		LPDIRECT3DTEXTURE9* meshTex, 
		DWORD meshNumMat, 
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::wstring playerName, 
		int teamNum, 
		int lives,
		GameState* initgamestate,
		int playerIdx)
	: MainPlayerClass(playerName, playerIdx, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)), _fsm(initgamestate, playerIdx) {	
		IniAI();
   }

	AIPlayer(
		PMESHSTRUCT meshStruct,
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::wstring playerName, 
		int teamNum, 
		int lives,
		GameState* initgamestate,
		int playerIdx)
	: MainPlayerClass(playerName, playerIdx, teamNum, lives, MainShipClass(meshStruct, newg_pDevice)), _fsm(initgamestate, playerIdx) {
    IniAI();
   }
	
	AIPlayer(MainShipClass shiptype,
		std::wstring playerName, 
		int teamNum, 
		int lives,
		GameState* initgamestate,
		int playerIdx)
	: MainPlayerClass(playerName, playerIdx, teamNum, lives, shiptype), _fsm(initgamestate, playerIdx) {
      IniAI();
   }

   ~AIPlayer(void);
};

#endif
