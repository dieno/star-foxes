#include "terrain.h"
#include "directXClass.h"

void Terrain::renderSelf() {
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
	for(int i = 0; i < 1; i++)
	{
		setupBuilding(*buildinglocations[i],*buildingscales[i],i);
	}
}

void Terrain::setupWorld() {
	D3DXMATRIXA16 rotationY;
	D3DXMATRIXA16 rotationX;
	D3DXMATRIXA16 rotationZ;
	D3DXMATRIXA16 translate;
    D3DXMATRIXA16 translate2;
	D3DXMATRIXA16 translate3;

	D3DXMATRIX scale = directXClass::Translate(0, 0, 0);
	scale(0,0) = 1000.0f;
	scale(1,1) = 0.25f;
	scale(2,2) = 1000.0f;
	D3DXMatrixRotationY( &rotationY, rotationAboutYMesh1 );
	D3DXMatrixRotationX( &rotationX, rotationAboutXMesh1 );
	D3DXMatrixRotationZ( &rotationZ, rotationAboutZMesh1 );
	translate = directXClass::Translate(translateXMesh1, translateYMesh1, translateZMesh1);
	D3DXMatrixTranslation(&translate2, 0.0f, 0.0f, -1.5f);
	D3DXMatrixTranslation(&translate3, 0.0f, 0.0f, 1.5f);

    g_pDevice->SetTransform( D3DTS_WORLD, &(translate2 * rotationZ 
											* rotationY * rotationX 
											* translate3 * (scale * translate)));
}

void Terrain::setupBuildings()
{
	for(int i = 0; i < 1; i++)
	{
		D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmesh[i], NULL);
		buildinglocations[i] = new D3DXVECTOR3();
		buildinglocations[i]->x = 0;
		buildinglocations[i]->y = 0;
		buildinglocations[i]->z = 10;
		buildingscales[i] = new D3DXVECTOR3();
		buildingscales[i]->x = 5;
		buildingscales[i]->y = 5;
		buildingscales[i]->z = 5;
	}
}

void Terrain::setupBuilding(D3DXVECTOR3 _translate, D3DXVECTOR3 _scale, int index)
{
	D3DXMATRIX translate = directXClass::Translate(_translate.x, _translate.y, _translate.z);
	D3DXMATRIX scale = directXClass::Translate(0,0,0);
	scale(0,0) = _scale.x;
	scale(1,1) = _scale.y;
	scale(2,2) = _scale.z;
	g_pDevice->SetTransform(D3DTS_WORLD, &(scale * translate));
    D3DMATERIAL9* mat = new D3DMATERIAL9();
    mat->Ambient= D3DXCOLOR(0.5f, 0.5f, 0.5f, 255.0f);
    g_pDevice->SetMaterial(mat);
	buildingmesh[index]->DrawSubset(0);
}