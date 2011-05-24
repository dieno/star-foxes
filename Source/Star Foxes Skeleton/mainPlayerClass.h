#ifndef MAINPLAYERCLASS_H
#define MAINPLAYERCLASS_H

#include "directXHeader.h"
#include "gamecore.h"

#include "mainShipClass.h"
#include <string>

class MainPlayerClass {
public:
	MainPlayerClass (
		std::wstring playerName, 
		int teamNum, 
		int lives,
		MainShipClass playerShip):playerName_(playerName), teamNum_(teamNum), lives_(lives), playerShip_(playerShip),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,20.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10), hadCollision(false), hasShot(false){}

	MainPlayerClass():playerName_(),teamNum_(),lives_(),playerShip_(),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,20.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10), hadCollision(false), hasShot(false){}

	void left(bool active) {playerShip_.bankLeft(active);}
	void right(bool active) {playerShip_.bankRight(active);}
	void up(bool active) {playerShip_.bankUp(active);}
	void down(bool active) {playerShip_.bankDown(active);}
	void boost(bool active) {playerShip_.boost(active);}
	void shoot(float timeDelta) {playerShip_.shoot(timeDelta);}

	void Update(float timeDelta);

	void updatePosition(float timeDelta); //put movement in here!

	void updateRotation(D3DXVECTOR3* vRotation);

	D3DXVECTOR3 getPositionVector() {return playerShip_.getPositionVector();}
	D3DXVECTOR3 getDirectionVector() {return playerShip_.getDirectionVector();}
	D3DXVECTOR3 getUpVector() {return playerShip_.getUpVector();}

	MainShipClass* GetShip();
	std::wstring getPlayerName() {
		return playerName_;
	}

	int getTeamNum() {
		return teamNum_;
	}

	int getLives() {
		return lives_;
	}

	virtual void drawSelf() {
		playerShip_.Draw();
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

	int getBoosterTimer() {
		return boosterTimer;
	}

	int getShipCurrentHealth() {
		return playerShip_.getCurrentHealth();
	}

	int getShipMaxHealth() {
		return playerShip_.getMaxHealth();
	}

   float getPositionX()
   {
	   return playerShip_.getPositionVector().x;
   }

   float getPositionY()
   {
      return playerShip_.getPositionVector().y;
   }

   float getPositionZ()
   {
      return playerShip_.getPositionVector().z;
   }

   bool gethadCollision()
   {
	   return hadCollision;
   }

   void sethadCollision(bool _hadCollision)
   {
	   hadCollision = _hadCollision;
   }

   bool gethasShot()
   {
	   return hasShot;
   }

   void sethasShot(bool _hasShot)
   {
	   hasShot = _hasShot;
   }

   D3DXVECTOR3 getPosition()
   {
	   return playerShip_.getPositionVector();
   }

	D3DXVECTOR3 getRotation()
	{
		return vRotation;
	}

	void reduceLives() {
		lives_--;
		isAlive = true;
		playerShip_.setCurrentHealth(playerShip_.getMaxHealth());
	}

	int getID() {
		return id;
	}

	void setID(int newID) {
		id = newID;
	}
   
   //ship takes a hit, damage taken is the damage caused by hit
   //returns bool true if still alive, false if dead
   bool takeHit(int damageTaken);

	void initProjectiles(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat);

   void SetRotation(float x, float y, float z) ;

private:
	//Player's  name (Bob, Bill, Bojangles, etc)
	std::wstring playerName_;

	//Player's team (1-4)
	int teamNum_;

	//How many lifes remain (decrements on death)
	int lives_;

	//The ship this player controls
	MainShipClass playerShip_;

	//player id 0-7;
	int id;

	D3DXVECTOR3 vRotation;
	D3DXVECTOR3 vPosition;
	float rotationZ;
	float rotationX;
	float positionX;
	bool afterboosterActive_;
	bool isAlive;
	int boosterTimer;
	float currentSpeed;
	bool hadCollision;
	bool hasShot;
};


#endif