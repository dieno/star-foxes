#include "directXHeader.h"

void MainPlayerClass::updatePosition()
{
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
}

void MainPlayerClass::updateRotation()
{
	if(vRotation.z != 0.0f)
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

		directXClass::SetError(TEXT("%f"), vRotation.z);

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

		directXClass::SetError(TEXT("%f"), vRotation.x);

		playerShip_.setRotationAboutX(vRotation.x);
	}

	//shiptype.setRotationAboutZ(D3DX_PI);
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
	playerShip_.setCurrentHealth(playerShip_.getCurrentHealth() - damageTaken);
	if(playerShip_.getCurrentHealth() <= 0) {
		isAlive = false;
		return isAlive;
	}
}