/*#include "directXHeader.h"

void PlayerClass::updatePosition()
{
	D3DXVECTOR3 vMovement(0.0f, 0.0f, 0.0f);

	if(isAlive)
	{
		if(vRotation.x != 0)
			vPosition.z += -(vRotation.x * currentSpeed) / 2.0f; 

		vMovement = D3DXVECTOR3(0.0f, 1.0f, 0.0f) - vPosition;

		//D3DXVec3Normalize(&vMovement, &vMovement);

		shiptype.setTranslateY(vMovement.z);
		//shiptype.setTranslateY(vMovement.y);
		//shiptype.setTranslateZ(vPosition.z);
	}
}

void PlayerClass::updateRotation()
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

		shiptype.setRotationAboutZ(vRotation.z);
	}

	if(vRotation.x != 0.0f)
	{
		if(vRotation.x < 0.0f)
		{
			vRotation.x += 0.025f;

			if(vRotation.x >= 0.01f)
				vRotation.x = 0.0f;
		}
		else
		{
			vRotation.x -= 0.025f;

			if(vRotation.x <= 0.01f)
				vRotation.x = 0.0f;
		}

		if(vRotation.x > D3DX_PI)
			vRotation.x = -D3DX_PI;
		
		if(vRotation.x < -D3DX_PI)
			vRotation.x = D3DX_PI;

		shiptype.setRotationAboutX(vRotation.x);
	}

}

void PlayerClass::bankLeft(float value)
{
	vRotation.z -= value;
	shiptype.setRotationAboutZ(vRotation.z);

	vPosition.x += value;
	shiptype.setTranslateX(vPosition.x);
}

void PlayerClass::bankRight(float value)
{
	vRotation.z += value;
	shiptype.setRotationAboutZ(vRotation.z);

	vPosition.x -= value;
	shiptype.setTranslateX(vPosition.x);
}
*/