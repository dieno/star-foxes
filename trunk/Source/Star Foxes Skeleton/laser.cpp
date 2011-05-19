#include "laser.h"

void Laser::drawSelf() {
	// Setup the world, view, and projection matrices
	setupWorld();

	// Meshes are divided into subsets, one for each material. Render them in
	// a loop
	for( DWORD i=0; i<meshStruct.g_dwNumMaterials; i++ )
	{
		// Set the material and texture for this subset
		g_pDevice->SetMaterial( &meshStruct.g_pMeshMaterials[i] );
		g_pDevice->SetTexture( 0, meshStruct.g_pMeshTextures[i] );
        
		// Draw the mesh subset
		meshStruct.g_pMesh->DrawSubset( i );
	}
}

void Laser::setupWorld() {
	//needs implementation....
}