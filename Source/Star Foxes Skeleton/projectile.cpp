#include "projectile.h"

void Projectile::Update(float timeDelta)
{
	timeToLive_ -= timeDelta;
	
	D3DXVECTOR3 force = vDirection_ * ThrustAmount * MaxForce;

	D3DXVECTOR3 acceleration = force / Mass;

	vVelocity_ += acceleration * timeDelta;

	vVelocity_ *= Drag;

	vPosition_ += vVelocity_ * timeDelta;
}

void Projectile::Draw()
{
	D3DXMATRIX mTranslateOffset;
	D3DXMatrixTranslation(&mTranslateOffset, vOffset_.x, vOffset_.y, vOffset_.z);

	D3DXMATRIX mTranslate;
	D3DXMatrixTranslation(&mTranslate, vPosition_.x, vPosition_.y, vPosition_.z);
	pDevice->SetTransform(D3DTS_WORLD, &(mWorld_ * mTranslateOffset * mTranslate));

	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for( DWORD i=0; i<dwNumMaterials; i++ )
	{
		// Set the material and texture for this subset
		pDevice->SetMaterial( &pMeshMaterials[i] );
		pDevice->SetTexture( 0, pMeshTextures[i] );
        
		// Draw the mesh subset
		pMesh->DrawSubset( i );
	}
}

void Projectile::setupWorldMatrix(D3DXMATRIX mWorld)
{
	mWorld_ = mWorld;
}
