#ifndef HUMANPLAYERCLASS_H
#define HUMANPLAYERCLASS_H
#include <string>
#include "mainPlayerClass.h"
#include "directXHeader.h"

class HumanPlayerClass : public MainPlayerClass {
public:
	HumanPlayerClass(
		LPD3DXMESH mesh, 
		D3DMATERIAL9* meshMat, 
		LPDIRECT3DTEXTURE9* meshTex, 
		DWORD meshNumMat, 
		LPDIRECT3DDEVICE9 newg_pDevice, 
		std::string playerName, 
		int teamNum, 
		int lives)
	: MainPlayerClass(playerName, teamNum, lives, MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice)) {}

};

#endif