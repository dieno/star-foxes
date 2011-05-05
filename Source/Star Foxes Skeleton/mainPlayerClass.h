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
		MainShipClass playerShip):playerName_(playerName), teamNum_(teamNum), lives_(lives), playerShip_(playerShip),isAlive(true){}
	MainPlayerClass():playerName_(),teamNum_(),lives_(),playerShip_(),isAlive(true){}

	std::string getPlayerName() {
		return playerName_;
	}

	int getTeamNum() {
		return teamNum_;
	}

	int getLives() {
		return lives_;
	}

	void drawSelf() {
		playerShip_.renderSelf();
	}

	void bankLeft(float value);
	void bankRight(float value);

	void bankUp(float value)
	{
		vRotation.x += value;
		playerShip_.setRotationAboutX(vRotation.x);
	}

	void bankDown(float value)
	{
		vRotation.x -= value;
		playerShip_.setRotationAboutX(vRotation.x);
	}

	void useAfterBooster() {
		afterboosterActive_ = true;
		currentSpeed = 1000.0f; //place holder
	}

	void deactivateAfterBooster() {
		afterboosterActive_ = false;
		currentSpeed = 1.0f; //place holder
	}

	void updatePosition(); //put movement in here!

	void updateRotation();

	int getBoosterTimer() {
		return boosterTimer;
	}

	int getShipCurrentHealth() {
		return playerShip_.getCurrentHealth();
	}

	int getShipMaxHealth() {
		return playerShip_.getMaxHealth();
	}

private:
	//Player's  name (Bob, Bill, Bojangles, etc)
	std::string playerName_;

	//Player's team (1-4)
	int teamNum_;

	//How many lifes remain (decrements on death)
	int lives_;

	//The ship this player controls
	MainShipClass playerShip_;

	D3DXVECTOR3 vRotation;
	D3DXVECTOR3 vPosition;
	float rotationZ;
	float rotationX;
	float positionX;

	int boosterTimer;
	bool afterboosterActive_;
	int currentSpeed;
	bool isAlive;
};


#endif