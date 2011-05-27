#include "mainPlayerClass.h"
#include "directXClass.h"


void MainPlayerClass::Update(float timeDelta)
{
	playerShip_.Update(timeDelta);
}

MainShipClass* MainPlayerClass::GetShip()
{
   return &playerShip_;
}

/*void MainPlayerClass::updatePosition(float timeDelta)
{
	playerShip_.Update(timeDelta);
	D3DXVECTOR3 vMovement(0.0f, 0.0f, 0.0f);

	if(isAlive)
	{
		if(vRotation.x != 0)
			vPosition.z += -(vRotation.x * currentSpeed) / 2.0f; 

		vMovement = D3DXVECTOR3(0.0f, 1.0f, 0.0f) - vPosition;

		//D3DXVec3Normalize(&vMovement, &vMovement);

		playerShip_.setTranslateY(vMovement.z);
		//shiptype.setTranslateY(vMovement.y);
		//shiptype.setTranslateZ(vPosition.z);
	}
}*/

void MainPlayerClass::updateRotation(D3DXVECTOR3* vRotation)
{
/*	if(vRotation.z != 0.0f)
	{
		if(vRotation.z < 0.0f)
		{
			vRotation.z += 0.02f;

			if(vRotation.z >= 0.01f)
				vRotation.z = 0.0f;
		}
		else
		{
			vRotation.z -= 0.02f;

			if(vRotation.z <= 0.01f)
				vRotation.z = 0.0f;
		}

		if(vRotation.z > D3DX_PI)
			vRotation.z = -D3DX_PI;
		
		if(vRotation.z < -D3DX_PI)
			vRotation.z = D3DX_PI;

		//directXClass::SetError(TEXT("%f"), vRotation.z);

		playerShip_.setRotationAboutZ(vRotation.z);
	}

	if(vRotation.x != 0.0f)
	{
		if(vRotation.x < 0.0f)
		{
			vRotation.x += 0.02f;

			if(vRotation.x >= 0.01f)
				vRotation.x = 0.0f;
		}
		else
		{
			vRotation.x -= 0.02f;

			if(vRotation.x <= 0.01f)
				vRotation.x = 0.0f;
		}

		if(vRotation.x > D3DX_PI)
			vRotation.x = -D3DX_PI;
		
		if(vRotation.x < -D3DX_PI)
			vRotation.x = D3DX_PI;

		//directXClass::SetError(TEXT("%f"), vRotation.x);

		playerShip_.setRotationAboutX(vRotation.x);
	}

	//shiptype.setRotationAboutZ(D3DX_PI);*/
}

void MainPlayerClass::SetRotation(float x, float y, float z) {
   vRotation.x = x;
   vRotation.y = y;
   vRotation.z = z;
   playerShip_.setRotationAboutX(x);
   playerShip_.setRotationAboutY(y);
   playerShip_.setRotationAboutZ(z);
   //playerShip_.setTranslateX(vPosition.x);
}

void MainPlayerClass::bankLeft(float value)
{
	vRotation.z -= value;
	playerShip_.setRotationAboutZ(vRotation.z);

	vPosition.x += value;
	playerShip_.setTranslateX(vPosition.x);
}

void MainPlayerClass::bankRight(float value)
{
	vRotation.z += value;
	playerShip_.setRotationAboutZ(vRotation.z);

	vPosition.x -= value;
	playerShip_.setTranslateX(vPosition.x);
}

bool MainPlayerClass::takeHit(int damageTaken) {
	startBlinking();
	playerShip_.setCurrentHealth(playerShip_.getCurrentHealth() - damageTaken);
	if(playerShip_.getCurrentHealth() <= 0) {
		isAlive = false;
	}
	return isAlive;
}

void MainPlayerClass::initProjectiles(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat)
{
	playerShip_.loadProjectile(mesh, meshMat, meshTex, meshNumMat);
}