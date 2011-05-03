#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H
#include "mainShipClass.h"
#include "directXHeader.h"

class PlayerClass {
public:
	PlayerClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice, int shipClass)
		:shiptype(mesh, meshMat, meshTex, meshNumMat, newg_pDevice), shipClass(shipClass),
		rotationZ(0), rotationX(0), currentSpeed(0), afterboosterActive(false), boosterTimer(10){}
	PlayerClass(){}
	void drawSelf() {
		shiptype.renderSelf();
	}

	void bankLeft(float value)
	{
		rotationZ -= value;
		shiptype.setRotationAboutZ(rotationZ);
	}

	void bankRight(float value)
	{
		rotationZ += value;
		shiptype.setRotationAboutZ(rotationZ);
	}

	void bankUp(float value)
	{
		rotationX += value;
		shiptype.setRotationAboutX(rotationX);
	}

	void bankDown(float value)
	{
		rotationX -= value;
		shiptype.setRotationAboutX(rotationX);
	}

	void useAfterBooster() {
		afterboosterActive = true;
		currentSpeed = 1000; //place holder
	}

	void deactivateAfterBooster() {
		afterboosterActive = false;
		currentSpeed = 0; //place holder
	}

	void updatePosition(); //put movement in here!

	int getBoosterTimer() {
		return boosterTimer;
	}

private:
	MainShipClass shiptype;
	int shipClass;
	float rotationZ;
	float rotationX;
	float currentSpeed;
	bool afterboosterActive;
	int boosterTimer;
};

#endif
