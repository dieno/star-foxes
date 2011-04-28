#ifndef PLAYERCLASS_H
#define PLAYERCLASS_H
#include "mainShipClass.h"

class playerClass {
public:
	playerClass(bool playerOrAI, int shipClass):shiptype(),playerOrAI(playerOrAI),shipclass(shipclass){}
private:
	mainShipClass shiptype;
	bool playerOrAI;
	int shipClass;
}

#endif
