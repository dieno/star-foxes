#ifndef MAINPLAYERCLASS_H
#define MAINPLAYERCLASS_H

#include "directXHeader.h"
#include "gamecore.h"

#include "mainShipClass.h"
#include <string>

class MainPlayerClass {
public:
   virtual void SetBounds(D3DXVECTOR3* pos){};

	MainPlayerClass (
		std::wstring playerName, 
		int playerIdx,
		int teamNum, 
		int lives,
		MainShipClass playerShip):playerName_(playerName), id_(playerIdx), teamNum_(teamNum), lives_(lives), playerShip_(playerShip),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,20.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10), hadCollision(false), hasShot(false), mat1(new D3DMATERIAL9){
		if (teamNum == 0) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0, 0.0f, 0, 1.0f),D3DXCOLOR(0, 0.0f, 0, 1.0f),D3DXCOLOR(0, 0.0f, 0, 1.0f));
		}
		if (teamNum == 1) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(1, 0.0f, 0, 1.0f),D3DXCOLOR(1, 0.0f, 0, 1.0f),D3DXCOLOR(1, 0.0f, 0, 1.0f));
		}
		if (teamNum == 2) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0, 1.0f, 0, 1.0f),D3DXCOLOR(0, 1.0f, 0, 1.0f),D3DXCOLOR(0, 1.0f, 0, 1.0f));
		}
		if (teamNum == 3) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0, 0.0f, 1, 1.0f),D3DXCOLOR(0, 0.0f, 1, 1.0f),D3DXCOLOR(0, 0.0f, 1, 1.0f));
		}
		if (teamNum == 4) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0, 1.0f, 1, 1.0f),D3DXCOLOR(0, 1.0f, 1, 1.0f),D3DXCOLOR(0, 1.0f, 1, 1.0f));
		}
		if (teamNum == 5) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(1, 0.0f, 1, 1.0f),D3DXCOLOR(1, 0.0f, 1, 1.0f),D3DXCOLOR(1, 0.0f, 1, 1.0f));
		}
		if (teamNum == 6) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(1, 1.0f, 0, 1.0f),D3DXCOLOR(1, 1.0f, 0, 1.0f),D3DXCOLOR(1, 1.0f, 0, 1.0f));
		}
		if (teamNum == 7) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(1, 1.0f, 1, 1.0f),D3DXCOLOR(1, 1.0f, 1, 1.0f),D3DXCOLOR(1, 1.0f, 1, 1.0f));
		}
		if (teamNum == 8) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f),D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
		}
		if (teamNum == 9) {
			playerShip_.setMat(mat1);
			playerShip_.SetMtrlColor(D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f),D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f),D3DXCOLOR(0.5f, 0.5f, 0.0f, 1.0f));
		}
	}

	MainPlayerClass():playerName_(),teamNum_(),lives_(),playerShip_(),
		rotationZ(0.0f), rotationX(0.0f), positionX(0.0f), currentSpeed(1.0f), vRotation(0.0f,0.0f,0.0f), vPosition(0.0f,0.0f,20.0f), afterboosterActive_(false),
		isAlive(true), boosterTimer(10), hadCollision(false), hasShot(false){}

	void left(bool active) {playerShip_.bankLeft(active);}
	void right(bool active) {playerShip_.bankRight(active);}
	void up(bool active) {playerShip_.bankUp(active);}
	void down(bool active) {playerShip_.bankDown(active);}
	void boost(bool active) {playerShip_.boost(active);}
	void shoot(float timeDelta) {playerShip_.shoot(timeDelta);}
   //virtual void Update(HWND hWnd, D3DXVECTOR3 pos, float timeDelta){};
	virtual void Update(float timeDelta);
   virtual void Start(){};

	void updatePosition(float timeDelta); //put movement in here!

	void updateRotation(D3DXVECTOR3* vRotation);

	D3DXVECTOR3 getPositionVector() {
		return playerShip_.getPositionVector();
	}
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

	virtual void drawSelf(D3DXMATRIX mView, bool displayHealth) {
		playerShip_.Draw(mView, displayHealth);
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
		return id_;
	}

	void startBlinking() {
		playerShip_.startBlinking();
	}
   
   //ship takes a hit, damage taken is the damage caused by hit
   //returns bool true if still alive, false if dead
   bool takeHit(int damageTaken);

	void initProjectiles(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat);

   void SetRotation(float x, float y, float z) ;

	void setShipSpawnLocationRotation(D3DXVECTOR3 spawnLoc, D3DXVECTOR3 spawnRot) {
		playerShip_.setSpawnLocationRotation(spawnLoc, spawnRot);
	}

	D3DXVECTOR3 getShipSpawnLocation() {
		return playerShip_.getSpawnLocation();
	}

	D3DXVECTOR3 getShipSpawnRotation() {
		return playerShip_.getSpawnRotation();
	}

	void respawn() {
		playerShip_.respawnShip();
	}

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
	int id_;

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
	D3DMATERIAL9 *mat1;
};


#endif