#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H
#include "mainShipClass.h"
#include "directXHeader.h"

class PlayerClass {
public:
	PlayerClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice, int shipClass)
		:shiptype(mesh, meshMat, meshTex, meshNumMat, newg_pDevice), shipClass(shipClass){}
	PlayerClass(){}
	void drawSelf() {
		shiptype.renderSelf();
	}
private:
	MainShipClass shiptype;
	int shipClass;
};

#endif
