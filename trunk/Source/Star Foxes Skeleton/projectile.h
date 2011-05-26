#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "directXHeader.h"

class Projectile
{
public: 
	Projectile(LPDIRECT3DDEVICE9 newg_pDevice, LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat, 
					D3DXVECTOR3 vDirection, D3DXMATRIX mWorld):
		pDevice(newg_pDevice),
		pMesh(mesh),
		pMeshMaterials(meshMat),
		pMeshTextures(meshTex),
		dwNumMaterials(meshNumMat),
		vDirection_(vDirection),
		mWorld_(mWorld),
		timeToLive_(10.0f),
		vPosition_(0.0f,0.0f,0.0f),
		vScale_(1.0f,1.0f,1.0f),
		vVelocity_(0.0f,0.0f,0.0f),
		vOffset_(0.0f,0.5f,1.0f),
		Mass(1.0f),
		MaxForce(24000.0f),
		Drag(0.97f),
		ThrustAmount(0.01f) {}

	Projectile(){}
	~Projectile(){}

	void Update(float timeDelta);

	void Draw(); 

	void setupWorldMatrix(D3DXMATRIX mWorld);

	float getTimeToLive() { return timeToLive_; }

	D3DXVECTOR3 getPosition() { return vPosition_; }

private:
	LPDIRECT3DDEVICE9 pDevice;

	LPD3DXMESH              pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*           pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     pMeshTextures; // Textures for our mesh
	DWORD                   dwNumMaterials;   // Number of mesh materials

	float timeToLive_;

	D3DXVECTOR3 vDirection_;

	D3DXVECTOR3 vPosition_;
	D3DXVECTOR3 vScale_;
	D3DXVECTOR3 vVelocity_;
	D3DXVECTOR3 vOffset_;

	D3DXMATRIX mWorld_;

	float Mass;
	float MaxForce;
	float Drag;
	float ThrustAmount;

};

#endif