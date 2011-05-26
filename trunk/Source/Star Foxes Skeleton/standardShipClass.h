#ifndef STANDARDSHIPCLASS_H
#define STANDARDSHIPCLASS_H

#include "mainShipClass.h"

class StandardShipClass : public MainShipClass {
public:
	StandardShipClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice){
			setDamagePerShot(1);
			setMaxHealth(120);
			setCurrentHealth(120);
	}

	StandardShipClass(PMESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(meshStruct, newg_pDevice){
			setDamagePerShot(1);
			setMaxHealth(120);
			setCurrentHealth(120);
	}

	static float getAfterburnerSpeed() {
		return afterburnerSpeed_;
	}
	
private:
	static float afterburnerSpeed_;
};
#endif