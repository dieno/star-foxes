#ifndef HUMANPLAYERCLASS_H
#define HUMANPLAYERCLASS_H
#include <string>
//#include "directXHeader.h"

class HumanPlayerClass : public MainPlayerClass {
public:
	HumanPlayerClass(
		LPD3DXMESH mesh, 
		D3DMATERIAL9* meshMat, 
		LPDIRECT3DTEXTURE9* meshTex, 
		DWORD meshNumMat, 
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::wstring playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)) {}

	
	HumanPlayerClass(
		PMESHSTRUCT meshStruct,
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::wstring playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(meshStruct, newg_pDevice)) {}
	
	HumanPlayerClass(MainShipClass shiptype, 
		std::wstring playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, shiptype) {}

	HumanPlayerClass():MainPlayerClass(){}

};

#endif