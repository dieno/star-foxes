#ifndef MAINSHIPCLASS_H
#define MAINSHIPCLASS_H

#include "directXHeader.h"
#include "gamecore.h"

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
		g_pDevice(newg_pDevice){}

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
		g_pDevice(NULL){}

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

   void setRotation(float x, float y, float z) {
      rotationAboutYMesh1 = y;
      rotationAboutXMesh1 = x;
      rotationAboutZMesh1 = z;

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

	float getRotationAboutY() {
		return rotationAboutYMesh1;
	}

	float getRotationAboutX() {
		return rotationAboutXMesh1;
	}

	float getRotationAboutZ() {
		return rotationAboutZMesh1;
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

	static float getDamagePerShot() {
		return damagePerShot;
	}

private:
	void setupWorld();
	static float afterburnerSpeed_;
	int currentHealth;
	int maxHealth;
	LPD3DXMESH              g_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*           g_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     g_pMeshTextures; // Textures for our mesh
	DWORD                   g_dwNumMaterials;   // Number of mesh materials
	float rotationAboutYMesh1;
	float rotationAboutXMesh1;
	float rotationAboutZMesh1;
	float translateYMesh1;
	float translateXMesh1;
	float translateZMesh1;
	LPDIRECT3DDEVICE9 g_pDevice;
	static float damagePerShot;
};

#endif