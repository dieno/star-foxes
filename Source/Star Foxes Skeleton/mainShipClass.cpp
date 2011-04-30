#include "directXHeader.h"

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

void MainShipClass::setupWorld() {
	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 rotationZ;
	D3DXMATRIXA16 translate;
    D3DXMATRIXA16 matWorld2;
    D3DXMATRIXA16 matWorld3;
    D3DXMATRIXA16 translate2;
	D3DXMATRIX scale = directXClass::Translate(0, 0, 0);
	scale(0,0) = 0.25f;
	scale(1,1) = 0.25f;
	scale(2,2) = 0.25f;
	D3DXMatrixRotationY( &rotationY, rotationAboutYMesh1 );
	D3DXMatrixRotationX( &rotationX, rotationAboutXMesh1 );
	D3DXMatrixRotationZ( &rotationZ, rotationAboutZMesh1 );
	translate = directXClass::Translate(translateXMesh1, translateYMesh1, translateZMesh1);
	D3DXMatrixMultiply(&translate2, &scale, &translate);
	D3DXMatrixMultiply(&matWorld2, &rotationY, &rotationX);
	D3DXMatrixMultiply(&matWorld3, &matWorld2, &rotationZ);
	D3DXMatrixMultiply(&matWorld, &matWorld3, &translate2);
    g_pDevice->SetTransform( D3DTS_WORLD, &matWorld );
}