#ifndef COMPUTERPLAYERCLASS_H
#define COMPUTERPLAYERCLASS_H
#include "mainPlayerClass.h"

class ComputerPlayerClass : public MainPlayerClass {
public:
	ComputerPlayerClass(
		LPD3DXMESH mesh, 
		D3DMATERIAL9* meshMat, 
		LPDIRECT3DTEXTURE9* meshTex, 
		DWORD meshNumMat, 
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::wstring playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)) {}
	
	ComputerPlayerClass(MainShipClass shiptype, 
		std::wstring playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, shiptype) {}

	ComputerPlayerClass():MainPlayerClass(){}



};

#endif