#ifndef LIGHTSHIPCLASS_H
#define LIGHTSHIPCLASS_H

#include "mainShipClass.h"

class LightShipClass : public MainShipClass {
public:
	LightShipClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice){
			setDamagePerShot(1);
			setMaxHealth(80);
			setCurrentHealth(80);
	}

	LightShipClass(PMESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(meshStruct, newg_pDevice){
			setDamagePerShot(1);
			setMaxHealth(80);
			setCurrentHealth(80);
	}

	static float getAfterburnerSpeed() {
		return afterburnerSpeed_;
	}
	
private:
	static float afterburnerSpeed_;
};
#endif