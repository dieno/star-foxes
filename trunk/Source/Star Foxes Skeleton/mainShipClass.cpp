#include "mainShipClass.h"
#include "directXClass.h"

float MainShipClass::afterburnerSpeed_ =  5.0f;
float MainShipClass::damagePerShot =  1.0f;

D3DXVECTOR3 MainShipClass::vRight_	   = D3DXVECTOR3(1.0f,0.0f,0.0f);
D3DXVECTOR3 MainShipClass::vUp_		   = D3DXVECTOR3(0.0f,1.0f,0.0f);
D3DXVECTOR3 MainShipClass::vDirection_ = D3DXVECTOR3(0.0f,0.0f,1.0f);

void MainShipClass::renderSelf() {
	// Setup the world, view, and projection matrices
	setupWorld();

	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for( DWORD i=0; i<g_dwNumMaterials; i++ )
	{
		// Set the material and texture for this subset
		g_pDevice->SetMaterial( &g_pMeshMaterials[i] );
		g_pDevice->SetTexture( 0, g_pMeshTextures[i] );
        
		// Draw the mesh subset
		g_pMesh->DrawSubset( i );
	}
}

void MainShipClass::Update(float timeDelta)
{
	D3DXVECTOR3 vRotationAmount(0.0f, 0.0f, 0.0f);

	vRotationAmount = vRotation_ * RotationRate_ * timeDelta;

	//if(vUp_.y < 0)
		//vRotationAmount.x = -vRotationAmount.x;

	yaw(vRotationAmount.x);
	pitch(vRotationAmount.y);

	D3DXVECTOR3 force = vDirection_ * thrustAmount_ * MaxForce_;

	D3DXVECTOR3 acceleration = force / Mass_;
	
	vVelocity_ += acceleration * timeDelta;

	vVelocity_ *= Drag_;
	
	vPosition_ += vVelocity_ * timeDelta;
}

void MainShipClass::pitch(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vRight_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);

	/*D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &vRight_, angle);

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);*/
}

void MainShipClass::yaw(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vUp_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);
	/*D3DXMATRIX T;

	// rotate around own up vector 
	D3DXMatrixRotationAxis(&T, &vUp_, angle);

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);*/
}

void MainShipClass::roll(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vDirection_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);

	/*D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &vDirection_, angle);

	// rotate _up and _right around _look vector
	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);*/
}

void MainShipClass::updateWorldMatrix()
{
	D3DXVec3Normalize(&vDirection_, &vDirection_);

	D3DXVec3Cross(&vUp_, &vDirection_, &vRight_);
	D3DXVec3Normalize(&vUp_, &vUp_);

	D3DXVec3Cross(&vRight_, &vUp_, &vDirection_);
	D3DXVec3Normalize(&vRight_, &vRight_);

	//directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	//directXClass::SetError(TEXT("----------------------"));

	D3DXMATRIX mScale;
	D3DXMatrixIdentity(&mScale);
	D3DXMatrixScaling(&mScale, 1.0f,1.0f,1.0f);

	D3DXMATRIX mRotate;
	D3DXMatrixIdentity(&mRotate);
	mRotate(0, 0) = vRight_.x; mRotate(0, 1) = vUp_.x; mRotate(0, 2) = vDirection_.x; mRotate(0, 3) = 0.0f;
	mRotate(1, 0) = vRight_.y; mRotate(1, 1) = vUp_.y; mRotate(1, 2) = vDirection_.y; mRotate(1, 3) = 0.0f;
	mRotate(2, 0) = vRight_.z; mRotate(2, 1) = vUp_.z; mRotate(2, 2) = vDirection_.z; mRotate(2, 3) = 0.0f;
	mRotate(3, 0) = 0.0f;	   mRotate(3, 1) =  0.0f;  mRotate(3, 2) =  0.0f;         mRotate(3, 3) = 1.0f;

	D3DXMATRIX mTransposedRotate;
	D3DXMatrixTranspose(&mTransposedRotate, &mRotate);

	D3DXMATRIX mTranslate;
	D3DXMatrixIdentity(&mTranslate);
	D3DXMatrixTranslation(&mTranslate, vPosition_.x, vPosition_.y, vPosition_.z);

	mWorld_ = mScale * mTransposedRotate * mTranslate;//mScale * mRotate ;
}

void MainShipClass::setupWorld() 
{
	updateWorldMatrix();

	g_pDevice->SetTransform(D3DTS_WORLD, &mWorld_);
	D3DXMatrixIdentity(&mWorld_);
}

void MainShipClass::reset() 
{
	vScale_ = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	vPosition_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vRotation_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vDirection_ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vUp_ = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vRight_ = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vVelocity_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&mWorld_);
}


void MainShipClass::bankRight(bool active)
{
	if(active)
	{
		vRotation_.x = 1.0f;
      directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		vRotation_.x = 0.0f;
	}   
}

void MainShipClass::bankLeft(bool active)
{
	if(active)
	{
		vRotation_.x = -1.0f;
      directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		vRotation_.x = 0.0f;
	}   
}

void MainShipClass::bankUp(bool active)
{
	if(active)
	{
		if(Y_AXIS_INVERTED)
			vRotation_.y = -1.0f;
		else
			vRotation_.y = 1.0f;
      directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		vRotation_.y = 0.0f;
	}
   
}

void MainShipClass::bankDown(bool active)
{
	if(active)
	{
		if(Y_AXIS_INVERTED)
			vRotation_.y = 1.0f;
		else
			vRotation_.y = -1.0f;
      directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		vRotation_.y = 0.0f;
	}   
}

void MainShipClass::boost(bool active)
{
	if(active)
	{
		thrustAmount_ = 0.001f;
      directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		thrustAmount_ = 0.0f;
	}   
}


/*void MainShipClass::setupWorld() {
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 rotationZ;
	D3DXMATRIXA16 translate;
    D3DXMATRIXA16 translate2;
	D3DXMATRIXA16 translate3;

	D3DXMATRIX scale = directXClass::Translate(0, 0, 0);
	scale(0,0) = 0.25f;
	scale(1,1) = 0.25f;
	scale(2,2) = 0.25f;
	D3DXMatrixRotationY( &rotationY, rotationAboutYMesh1 );
	D3DXMatrixRotationX( &rotationX, rotationAboutXMesh1 );
	D3DXMatrixRotationZ( &rotationZ, rotationAboutZMesh1 );
	translate = directXClass::Translate(translateXMesh1, translateYMesh1, translateZMesh1);
	D3DXMatrixTranslation(&translate2, 0.0f, 0.0f, -1.5f);
	D3DXMatrixTranslation(&translate3, 0.0f, 0.0f, 1.5f);

    g_pDevice->SetTransform( D3DTS_WORLD, &(translate2 * rotationZ 
											* rotationY * rotationX 
											* translate3 * (scale * translate)));
}*/

