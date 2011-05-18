#ifndef HEAVYSHIPCLASS_H
#define HEAVYSHIPCLASS_H

#include "mainShipClass.h"

class HeavyShipClass : public MainShipClass {
public:
	HeavyShipClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice){
			setMaxHealth(160);
			setCurrentHealth(160);
	}

	HeavyShipClass(PMESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(meshStruct, newg_pDevice){
			setMaxHealth(160);
			setCurrentHealth(160);
	}

	static float getAfterburnerSpeed() {
		return afterburnerSpeed_;
	}

	static float getDamagePerShot() {
		return damagePerShot;
	}
	
private:
	static float afterburnerSpeed_;
	static float damagePerShot;
};
#endif