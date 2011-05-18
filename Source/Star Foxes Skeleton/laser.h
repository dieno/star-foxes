#ifndef LASER_H
#define LASER_H

#include "directXHeader.h"
#include "gamecore.h"

class Laser {
public:
	Laser(int damage, D3DXVECTOR3 direction, D3DXVECTOR3 startPosition)
		:damage(damage), speed(1), direction(direction), position(startPosition){}
	int damage;
	float speed;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 position;
};


#endif