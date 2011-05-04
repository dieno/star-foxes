#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H
#include "mainShipClass.h"
#include "directXHeader.h"

class PlayerClass {
public:
	PlayerClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice, int shipClass)
		:shiptype(mesh, meshMat, meshTex, meshNumMat, newg_pDevice), shipClass(shipClass),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,0.0f), afterboosterActive(false),
		isAlive(true), boosterTimer(10){}
	PlayerClass(){}

	void drawSelf() {
		shiptype.renderSelf();
	}

	void bankLeft(float value);
	void bankRight(float value);

	void bankUp(float value)
	{
		vRotation.x += value;
		shiptype.setRotationAboutX(vRotation.x);
	}

	void bankDown(float value)
	{
		vRotation.x -= value;
		shiptype.setRotationAboutX(vRotation.x);
	}

	void useAfterBooster() {
		afterboosterActive = true;
		currentSpeed = 1000.0f; //place holder
	}

	void deactivateAfterBooster() {
		afterboosterActive = false;
		currentSpeed = 1.0f; //place holder
	}

	void updatePosition(); //put movement in here!

	void updateRotation();

	int getBoosterTimer() {
		return boosterTimer;
	}

private:
	MainShipClass shiptype;
	int shipClass;
	D3DXVECTOR3 vRotation;
	D3DXVECTOR3 vPosition;
	float rotationZ;
	float rotationX;
	float positionX;
	float currentSpeed;
	bool afterboosterActive;
	int boosterTimer;
	bool isAlive;
};

#endif
