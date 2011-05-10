#ifndef MAINPLAYERCLASS_H
#define MAINPLAYERCLASS_H

#include "directXHeader.h"
#include "gamecore.h"

#include "mainShipClass.h"
#include <string>

class MainPlayerClass {
public:
	MainPlayerClass (
		std::string playerName, 
		int teamNum, 
		int lives,
		MainShipClass playerShip):playerName_(playerName), teamNum_(teamNum), lives_(lives), playerShip_(playerShip),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,0.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10){}
	MainPlayerClass():playerName_(),teamNum_(),lives_(),playerShip_(),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,0.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10){}

	std::string getPlayerName() {
		return playerName_;
	}

	int getTeamNum() {
		return teamNum_;
	}

	int getLives() {
		return lives_;
	}

	virtual void drawSelf() {
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
		currentSpeed = playerShip_.getAfterburnerSpeed(); //place holder
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

	//ship takes a hit, damage taken is the damage caused by hit
	//returns bool true if still alive, false if dead
	bool takeHit(int damageTaken);

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
	bool afterboosterActive_;
	bool isAlive;
	int boosterTimer;
	float currentSpeed;
};


#endif