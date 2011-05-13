#ifndef STANDARDSHIPCLASS_H
#define STANDARDSHIPCLASS_H

#include "mainShipClass.h"

class MedicShipClass : public MainShipClass {
public:
	MedicShipClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(mesh, meshMat, meshTex, meshNumMat, newg_pDevice){
			setMaxHealth(100);
			setCurrentHealth(100);
	}

	MedicShipClass(PMESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 newg_pDevice)
		:MainShipClass(meshStruct, newg_pDevice){
			setMaxHealth(100);
			setCurrentHealth(100);
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