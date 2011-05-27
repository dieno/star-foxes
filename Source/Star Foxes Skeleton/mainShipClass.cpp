#include "mainShipClass.h"
#include "directXClass.h"

float MainShipClass::afterburnerSpeed_ =  5.0f;
/*
D3DXVECTOR3 MainShipClass::vRight_	   = D3DXVECTOR3(1.0f,0.0f,0.0f);
D3DXVECTOR3 MainShipClass::vUp_		   = D3DXVECTOR3(0.0f,1.0f,0.0f);
D3DXVECTOR3 MainShipClass::vDirection_ = D3DXVECTOR3(0.0f,0.0f,1.0f);
*/
void MainShipClass::Draw(D3DXMATRIX mView, bool displayHealth)
{
	renderSelf();
	drawProjectiles();

	if(displayHealth)
		drawHealthbar(mView);
}

void MainShipClass::renderSelf() {
	if(!isBlinkedOut) {
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
}

void MainShipClass::Update(float timeDelta)
{
	if (blinkState != 0 && timeGetTime() - blinkStartTime > 100.0f) {
		isBlinkedOut = false;
		blinkStartTime = timeGetTime();
		blinkState == 1;
		//directXClass::SetError(TEXT("end blink!"));
	} else if (blinkState == 1 && timeGetTime() - blinkStartTime > 100.0f) {
		isBlinkedOut = true;
		blinkStartTime = timeGetTime();
		blinkState == 2;
		//directXClass::SetError(TEXT("end blink!"));
	} else if (blinkState == 2 && timeGetTime() - blinkStartTime > 100.0f) {
		isBlinkedOut = false;
		//blinkStartTime = timeGetTime();
		blinkState == 0;
		//directXClass::SetError(TEXT("end blink!"));
	}
	//directXClass::SetError(TEXT("curTime: %f startTime: %f"), timeGetTime(), blinkStartTime);
	D3DXVECTOR3 vRotationAmount(0.0f, 0.0f, 0.0f);

	vRotationAmount = vRotation_ * RotationRate_ * timeDelta;

	//if(vUp_.y < 0)
		//vRotationAmount.x = -vRotationAmount.x;

	yaw(vRotationAmount.x);
	pitch(vRotationAmount.y);
	
	//directXClass::SetError(TEXT("vRot: x: %f, y: %f, z: %f"),vRotationAmount.x,vRotationAmount.y,vRotationAmount.z);

	//updateRotation(&vRotationAmount);

	D3DXVECTOR3 force = vDirection_ * thrustAmount_ * MaxForce_;

	D3DXVECTOR3 acceleration = force / Mass_;
	
	vVelocity_ += acceleration * timeDelta;

	vVelocity_ *= Drag_;
	
	vPosition_ += vVelocity_ * timeDelta;

	updateProjectiles(timeDelta);
	updateHealthbar();
}

void MainShipClass::updateRotation(D3DXVECTOR3 *vRotation)
{
	if((*vRotation).z != 0.0f)
	{
		if((*vRotation).z < 0.0f)
		{
			(*vRotation).z += 0.01f;

			if((*vRotation).z >= 0.01f)
				(*vRotation).z = 0.0f;
		}
		else
		{
			(*vRotation).z -= 0.01f;

			if((*vRotation).z <= 0.01f)
				(*vRotation).z = 0.0f;
		}

		if((*vRotation).z > D3DX_PI)
			(*vRotation).z = -D3DX_PI;
		
		if((*vRotation).z < -D3DX_PI)
			(*vRotation).z = D3DX_PI;
	}

	if((*vRotation).x != 0.0f)
	{
		if((*vRotation).x < 0.0f)
		{
			(*vRotation).x += 0.01f;

			if((*vRotation).x >= 0.01f)
				(*vRotation).x = 0.0f;
		}
		else
		{
			(*vRotation).x -= 0.01f;

			if((*vRotation).x <= 0.01f)
				(*vRotation).x = 0.0f;
		}

		if((*vRotation).x > D3DX_PI)
			(*vRotation).x = -D3DX_PI;
		
		if((*vRotation).x < -D3DX_PI)
			(*vRotation).x = D3DX_PI;
	}
}

void MainShipClass::pitch(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vRight_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);
}

void MainShipClass::yaw(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vUp_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vDirection_, &vDirection_, &T);
}

void MainShipClass::roll(float angle)
{
	D3DXQUATERNION Q;
	D3DXQuaternionRotationAxis(&Q, &vDirection_, angle);

	D3DXMATRIX T;
	D3DXMatrixRotationQuaternion(&T, &Q);

	D3DXVec3TransformCoord(&vRight_, &vRight_, &T);
	D3DXVec3TransformCoord(&vUp_, &vUp_, &T);
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
	D3DXMatrixScaling(&mScale, vScale_.x, vScale_.y, vScale_.z);

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
	shipBound.drawNodes(mWorld_, vPosition_);
}

void MainShipClass::setupWorld() 
{
	updateWorldMatrix();

	g_pDevice->SetTransform(D3DTS_WORLD, &mWorld_);
	shipBound.drawNodes(mWorld_, vPosition_);

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
      //directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
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
      //directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
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
     // directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
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
      //directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
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
      //directXClass::SetError(TEXT("up: X: %f, Y: %f, Z: %f dir: X: %f, Y: %f, Z: %f right: X: %f, Y: %f, Z: %f"), vUp_.x, vUp_.y, vUp_.z, vDirection_.x, vDirection_.y, vDirection_.z, vRight_.x, vRight_.y, vRight_.z);
	}
	else
	{
		thrustAmount_ = 0.000f;
	}   
}

void MainShipClass::loadProjectile(LPD3DXMESH mesh, D3DMATERIAL9* meshMat, LPDIRECT3DTEXTURE9* meshTex, DWORD meshNumMat)
{
	g_pMesh2 = mesh; 
	g_pMeshMaterials2 = g_pMeshMaterials; 
	g_pMeshTextures2 = meshTex; 
	g_dwNumMaterials2 = meshNumMat; 
}

void MainShipClass::shoot(float timeDelta)
{
	if(timeToShoot <= 0.0f)
	{
		updateWorldMatrix();
		Projectile newProjectile(g_pDevice, g_pMesh2, g_pMeshMaterials2, g_pMeshTextures2, g_dwNumMaterials2, vDirection_, mWorld_);
		projectileList.push_back(newProjectile);
		timeToShoot = SHOOT_SPEED;
	}
}

void MainShipClass::updateProjectiles(float timeDelta)
{
	timeToShoot -= timeDelta;

	std::list<Projectile>::iterator it = projectileList.begin();

	while(it != projectileList.end())
	{
		(*it).Update(timeDelta);

		if((*it).getTimeToLive() < 0.0f)
		{
			it = projectileList.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void MainShipClass::drawProjectiles()
{
	for(std::list<Projectile>::iterator it = projectileList.begin(); it != projectileList.end(); ++it)
	{
		(*it).Draw();
	}
}

void MainShipClass::startBlinking() {
	if(!isBlinkedOut) {
		blinkStartTime = timeGetTime();
		isBlinkedOut = true;
		blinkState = 8;
		//directXClass::SetError(TEXT("start blink!"));
	}
}

void MainShipClass::initHealthbar()
{
	D3DXCreateTextureFromFile(g_pDevice, TEXT("EnemyHealth.bmp"), &healthbarTex);

	g_pDevice->SetFVF(CUSTOMFVF);

	CUSTOMVERTEX vertices[] = 
    {
        { -1.7f, 1.7f, 0.0f, 0.0f, 0.0f, },
        { 1.7f, 1.7f, 0.0f, 1.0f, 0.0f,  },
        { -1.7f, 1.2f, 0.0f, 0.0f, 1.0f,  },
        { 1.7f, 1.2f, 0.0f, 1.0f, 1.0f,  },
    };

	// create a vertex buffer interface called i_buffer
    g_pDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
								   0,
								   CUSTOMFVF,
								   D3DPOOL_MANAGED,
								   &v_buffer,
								   NULL);

	VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    v_buffer->Unlock();
}

void MainShipClass::drawHealthbar(D3DXMATRIX mView)
{
	D3DXMATRIX mTranslate;
	D3DXMatrixTranslation(&mTranslate, vPosition_.x, vPosition_.y, vPosition_.z);

	D3DXMATRIX orientation;
	D3DXMatrixInverse(&orientation, NULL, &mView); //view is your camera view matrix.

	orientation._41=0;
	orientation._42=0;
	orientation._43=0;
	orientation._14=0;orientation._24=0;orientation._34=0;orientation._44=1;

	//now orientation is the rotation matrix for the cube to keep it facing the camera.
	//it basicly just "unrotates"

	g_pDevice->SetMaterial( &g_pMeshMaterials[0] );

	g_pDevice->SetTransform(D3DTS_WORLD, &(orientation * mTranslate));//&(mWorld * mTranslate));	
	g_pDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	g_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

void MainShipClass::updateHealthbar()
{
	currentHealthPos = (((float) currentHealth / (float) maxHealth) * 3.4f) - 1.7f;

	CUSTOMVERTEX vertices[] = 
    {
        { -1.7f, 1.7f, 0.0f, 0.0f, 0.0f, },
        { currentHealthPos, 1.7f, 0.0f, 1.0f, 0.0f,  },
        { -1.7f, 1.2f, 0.0f, 0.0f, 1.0f, },
        { currentHealthPos, 1.2f, 0.0f, 1.0f, 1.0f,  },
    };

	// create a vertex buffer interface called i_buffer
    g_pDevice->CreateVertexBuffer( 4*sizeof(CUSTOMVERTEX),
								   0,
								   CUSTOMFVF,
								   D3DPOOL_MANAGED,
								   &v_buffer,
								   NULL);

	VOID* pVoid;    // a void pointer

    // lock v_buffer and load the vertices into it
    v_buffer->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, vertices, sizeof(vertices));
    v_buffer->Unlock();
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

