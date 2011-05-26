#ifndef MAINSHIPCLASS_H
#define MAINSHIPCLASS_H

#include "directXHeader.h"
#include "gamecore.h"
#include "projectile.h"
#include "boundingBox.h"

#define Y_AXIS_INVERTED false
#define SHOOT_SPEED 0.1f

class MainShipClass {
public:
	MainShipClass(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, LPDIRECT3DDEVICE9 newg_pDevice):
		currentHealth(100),
		maxHealth(100),
		g_pMesh(mesh),
		g_pMeshMaterials(meshMat),
		g_pMeshTextures(meshTex),
		g_dwNumMaterials(meshNumMat),
		rotationAboutYMesh1(0),
		rotationAboutXMesh1(0),
		rotationAboutZMesh1(0),
		translateYMesh1(0),
		translateXMesh1(0),
		translateZMesh1(0),
		g_pDevice(newg_pDevice),
		vPosition_(0.0f,0.0f,0.0f),
		vScale_(1.0f,1.0f,1.0f),
		vRotation_(0.0f,0.0f,0.0f),
		vVelocity_(0.0f,0.0f,0.0f),
		mWorld_(),
		RotationRate_(1.5f),
		Mass_(1.0f),
		MaxForce_(24000.0f),
		Drag_(0.97f),
		thrustAmount_(0.0f),
		projectileList(),
		blinkStartTime(0),
		isBlinkedOut(false),
		blinkState(0){
         IniVectors();
		 shipBound = boundingBox(this->g_pMesh, g_pDevice);
      }

	MainShipClass(PMESHSTRUCT meshStruct, LPDIRECT3DDEVICE9 newg_pDevice):
		currentHealth(100),
		maxHealth(100),
		g_pMesh(meshStruct->g_pMesh),
		g_pMeshMaterials(meshStruct->g_pMeshMaterials),
		g_pMeshTextures(meshStruct->g_pMeshTextures),
		g_dwNumMaterials(meshStruct->g_dwNumMaterials),
		rotationAboutYMesh1(0),
		rotationAboutXMesh1(0),
		rotationAboutZMesh1(0),
		translateYMesh1(0),
		translateXMesh1(0),
		translateZMesh1(0),
		g_pDevice(newg_pDevice),
		vPosition_(0.0f,0.0f,0.0f),
		vScale_(1.0f,1.0f,1.0f),
		vRotation_(0.0f,0.0f,0.0f),
		vVelocity_(0.0f,0.0f,0.0f),
		mWorld_(),
		RotationRate_(1.5f),
		Mass_(1.0f),
		MaxForce_(24000.0f),
		Drag_(0.97f),
		thrustAmount_(0.0f),
		projectileList(),
		blinkStartTime(0),
		isBlinkedOut(false),
		blinkState(0){
         IniVectors();
		 shipBound = boundingBox(this->g_pMesh, g_pDevice);
      }

	MainShipClass():
		currentHealth(100),
		maxHealth(100),
		g_pMesh(NULL),
		g_pMeshMaterials(NULL),
		g_pMeshTextures(NULL),
		g_dwNumMaterials(0L),
		rotationAboutYMesh1(0),
		rotationAboutXMesh1(0),
		rotationAboutZMesh1(0),
		translateYMesh1(0),
		translateXMesh1(0),
		translateZMesh1(0),
		g_pDevice(NULL),
		vPosition_(0.0f,0.0f,0.0f),
		vScale_(1.0f,1.0f,1.0f),
		vRotation_(0.0f,0.0f,0.0f),
		vVelocity_(0.0f,0.0f,0.0f),
		mWorld_(),
		RotationRate_(1.5f),
		Mass_(1.0f),
		MaxForce_(24000.0f),
		Drag_(0.97f),
		thrustAmount_(0.0f),
		projectileList(),
		blinkStartTime(0),
		isBlinkedOut(false){
		 damagePerShot = 1;
         IniVectors();
      }

   void IniVectors()
   {
      vRight_	   = D3DXVECTOR3(1.0f,0.0f,0.0f);
      vUp_		   = D3DXVECTOR3(0.0f,1.0f,0.0f);
      vDirection_ = D3DXVECTOR3(0.0f,0.0f,1.0f);
   }
	// updates the ship's location + orientation
	void Update(float timeDelta);

	void bankRight(bool active);
	void bankLeft(bool active);
	void bankUp(bool active);
	void bankDown(bool active);
	void boost(bool active);
	void shoot(float timeDelta);

	// getters + setters:

	D3DXMATRIX getWorldMatrix() {return mWorld_;}

	void setWorldMatrix(D3DXMATRIX newWorld_)
	{
		mWorld_ = newWorld_;
	}

	D3DXVECTOR3 getPositionVector() {
		return vPosition_;
	}

	D3DXVECTOR3 getRotationVector() {return vRotation_;}

	D3DXVECTOR3 getDirectionVector() { return vDirection_; }

	D3DXVECTOR3 getUpVector() { return vUp_; }

	float getDmgPerShot() { return damagePerShot; }

	D3DXVECTOR3 getRightVector() 
	{
		return D3DXVECTOR3(vRight_.x, vRight_.y, vRight_.z);
		//return vUp_;
	}

	void setPositionVector(D3DXVECTOR3 vPos)
	{
		vPosition_ = vPos; 
	}

	void setRotationVector(D3DXVECTOR3 vRot)
	{
		vRotation_ = vRot; 
	}

	void setUpVector(D3DXVECTOR3 newUpVector) {
		vUp_ = newUpVector;
	}

	void setRightVector(D3DXVECTOR3 newRightVector) {
		vRight_ = newRightVector;
	}

	void setDirectionVector(D3DXVECTOR3 newDirectionVector) {
		vDirection_ = newDirectionVector;
	}

	// OLD STUFF

	int getCurrentHealth() {
		return currentHealth;
	}

   void SetMtrlColor(D3DXCOLOR ambient, D3DXCOLOR emissive, D3DXCOLOR difusse)
   {
      g_pMeshMaterials->Ambient = ambient;
      g_pMeshMaterials->Emissive = emissive;
      g_pMeshMaterials->Diffuse = difusse;
   }

	int getMaxHealth() {
		return maxHealth;
	}

	void renderSelf();

   void setTranslation(float x, float y, float z) {
      translateYMesh1 = y;
      translateZMesh1 = z;
      translateXMesh1 = x;
   }

	void setRotationAboutY(float rotationY) {
		rotationAboutYMesh1 = rotationY;
	}
	
	void setRotationAboutX(float rotationX) {
		rotationAboutXMesh1 = rotationX;
	}
	
	void setRotationAboutZ(float rotationZ) {
		rotationAboutZMesh1 = rotationZ;
	}

	void setTranslateX(float translateX) {
		translateXMesh1 = translateX;
	}

   void SetRotation(float x, float y, float z)
   {
      vRotation_.x = x;
      vRotation_.y = y;
      vRotation_.z = z;
   }

	void setTranslateY(float translateY) {
		translateYMesh1 = translateY;
	}

	void setTranslateZ(float translateZ) {
		translateZMesh1 = translateZ;
	}

	float getTranslateY() {
		return translateYMesh1;
	}

	float getTranslateX() {
		return translateYMesh1;
	}

	float getTranslateZ() {
		return translateYMesh1;
	}

	void setg_pDevice(LPDIRECT3DDEVICE9 newg_pDevice) {
		g_pDevice = newg_pDevice;
	}
	
	LPDIRECT3DDEVICE9 getg_pDevice() {
		return g_pDevice;
	}

	float getRotationAboutY() {
		return rotationAboutYMesh1;
	}
   
	float getRotationAboutX() {
		return rotationAboutXMesh1;
	}

	float getRotationAboutZ() {
		return rotationAboutZMesh1;
	}

	std::list<Projectile> getProjectiles()
	{
		return projectileList;
	}

	void setMaxHealth(int newMaxHealth) {
		maxHealth = newMaxHealth;
	}

	void setCurrentHealth(int newHealth) {
		currentHealth = newHealth;
	}

	static float getAfterburnerSpeed() {
		return afterburnerSpeed_;
	}

	float getDamagePerShot() {
		return damagePerShot;
	}

	void setDamagePerShot(float _damagePerShot) {
		damagePerShot = _damagePerShot;
	}

	void loadProjectile(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat);
	void Draw();
   void SetPosition(float x, float y, float z)
   {
      vPosition_.x = x;
      vPosition_.y = y;
      vPosition_.z = z;
   }

   MESHSTRUCT getMeshData()
   {
	   MESHSTRUCT meshData;
	   meshData.g_dwNumMaterials = g_dwNumMaterials;
	   meshData.g_pMesh = g_pMesh;
	   meshData.g_pMeshMaterials = g_pMeshMaterials;
	   meshData.g_pMeshTextures = g_pMeshTextures;
	   return meshData;
   }

   void startBlinking();

   void setMat(D3DMATERIAL9* newMat) {
		g_pMeshMaterials = newMat;
		g_pMeshMaterials2 = newMat;
   }

   boundingBox getBound()
   {
	   return shipBound;
   }

   std::list<Projectile> projectileList;

	void setSpawnLocationRotation(D3DXVECTOR3 spawnLoc, D3DXVECTOR3 spawnRot) {
		vRespawnPosition_ = spawnLoc;
		vRespawnRotation_ = spawnRot;
	}

	D3DXVECTOR3 getSpawnLocation() {
		return vRespawnPosition_;
	}

	D3DXVECTOR3 getSpawnRotation() {
		return vRespawnRotation_;
	}

	void respawnShip() {
		vPosition_ = vRespawnPosition_;
		vRotation_ = vRespawnRotation_;
	}

private:
	static float afterburnerSpeed_;
	int currentHealth;
	int maxHealth;
	LPD3DXMESH              g_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTextures; // Textures for our mesh
	DWORD                   g_dwNumMaterials;   // Number of mesh materials
	LPD3DXMESH              g_pMesh2; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterials2; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTextures2; // Textures for our mesh
	DWORD                   g_dwNumMaterials2;   // Number of mesh materials
	float rotationAboutYMesh1;
	float rotationAboutXMesh1;
	float rotationAboutZMesh1;
	float translateYMesh1;
	float translateXMesh1;
	float translateZMesh1;
	LPDIRECT3DDEVICE9 g_pDevice;
	float damagePerShot;
	
	float timeToShoot;

	// ship's location & orientation
	D3DXVECTOR3 vPosition_;
	D3DXVECTOR3 vScale_;
	D3DXVECTOR3 vRotation_;
	D3DXVECTOR3 vDirection_;
	D3DXVECTOR3 vUp_;
	D3DXVECTOR3 vRight_;
	D3DXVECTOR3 vVelocity_;

	D3DXMATRIX mWorld_; // world transformation matrix

	// physics variables

	// speed at which the ship rotates
	float RotationRate_;
	
	// mass of ship
	float Mass_;

	// maximum force that can be applied along the ships direction
	float MaxForce_;

	// scalar that simulates drag
	float Drag_;

	int currentHealth_;
	int maxHealth_;
	float thrustAmount_;

	// sets up the world transform
	void setupWorld();

	// resets the ship's location to the origin
	void reset();

	// updates the world transform matrix
	void updateWorldMatrix();

	void updateRotation(D3DXVECTOR3 *vRotation);

	void pitch(float angle);
	void yaw(float angle);
	void roll(float angle);

	void updateProjectiles(float timeDelta);
	void drawProjectiles();

	
	float blinkStartTime;
	bool isBlinkedOut;
	int blinkState;

	boundingBox shipBound;
	
	//respawn stuff
	D3DXVECTOR3 vRespawnPosition_;
	D3DXVECTOR3 vRespawnRotation_;
};

#endif