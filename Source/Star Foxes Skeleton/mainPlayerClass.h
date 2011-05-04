#ifndef MAINPLAYERCLASS_H
#define MAINPLAYERCLASS_H
#include <string>
#include "mainShipClass.h"
#include "directXHeader.h"

class MainPlayerClass {
public:
	MainPlayerClass (
		std::string playerName, 
		int teamNum, 
		int lives,
		MainShipClass playerShip):playerName_(playerName), teamNum_(teamNum), lives_(lives), playerShip_(playerShip){}

	//Player's  name (Bob, Bill, Bojangles, etc)
	std::string playerName_;

	//Player's team (1-4)
	int teamNum_;

	//How many lifes remain (decrements on death)
	int lives_;

	//The ship this player controls
	MainShipClass playerShip_;
};


#endif