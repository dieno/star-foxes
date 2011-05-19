#ifndef LASER_H
#define LASER_H

#include "directXHeader.h"
#include "gamecore.h"

class Laser {
public:
	Laser(int damage, D3DXVECTOR3 direction, D3DXVECTOR3 startPosition, MESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 g_pDevice)
		:damage(damage), speed(1), direction(direction), position(startPosition), meshStruct(meshStruct), g_pDevice(g_pDevice){}
	int damage;
	float speed;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 position;
	MESHSTRUCT meshStruct;
	LPDIRECT3DDEVICE9 g_pDevice;
	void drawSelf();
	void setupWorld();
};

#endif