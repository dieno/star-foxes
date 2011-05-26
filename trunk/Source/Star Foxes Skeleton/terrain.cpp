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
	for(int i = 0; i < 19; i++)
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
		D3DXCreateBox(g_pDevice, 10, 20, 10, &buildingmesh[i], NULL);
		buildinglocations[i] = new D3DXVECTOR3();
		buildinglocations[i]->x = 0;
		buildinglocations[i]->y = 40;
		buildinglocations[i]->z = 10;
		buildingscales[i] = new D3DXVECTOR3();
		buildingscales[i]->x = 10;
		buildingscales[i]->y = 20;
		buildingscales[i]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 40, 10, &buildingmesh[1], NULL);
		buildinglocations[1] = new D3DXVECTOR3();
		buildinglocations[1]->x = 30;
		buildinglocations[1]->y = 20;
		buildinglocations[1]->z = 10;
		buildingscales[1] = new D3DXVECTOR3();
		buildingscales[1]->x = 10;
		buildingscales[1]->y = 40;
		buildingscales[1]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 40, 10, &buildingmesh[2], NULL);
		buildinglocations[2] = new D3DXVECTOR3();
		buildinglocations[2]->x = -30;
		buildinglocations[2]->y = 20;
		buildinglocations[2]->z = 10;
		buildingscales[2] = new D3DXVECTOR3();
		buildingscales[2]->x = 10;
		buildingscales[2]->y = 40;
		buildingscales[2]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 20, 10, &buildingmesh[3], NULL);
		buildinglocations[3] = new D3DXVECTOR3();
		buildinglocations[3]->x = -30;
		buildinglocations[3]->y = 10;
		buildinglocations[3]->z = -20;
		buildingscales[3] = new D3DXVECTOR3();
		buildingscales[3]->x = 10;
		buildingscales[3]->y = 20;
		buildingscales[3]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 20, 10, &buildingmesh[4], NULL);
		buildinglocations[4] = new D3DXVECTOR3();
		buildinglocations[4]->x = 30;
		buildinglocations[4]->y = 10;
		buildinglocations[4]->z = -20;
		buildingscales[4] = new D3DXVECTOR3();
		buildingscales[4]->x = 10;
		buildingscales[4]->y = 20;
		buildingscales[4]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 30, 10, &buildingmesh[5], NULL);
		buildinglocations[5] = new D3DXVECTOR3();
		buildinglocations[5]->x = 0;
		buildinglocations[5]->y = 15;
		buildinglocations[5]->z = -40;
		buildingscales[5] = new D3DXVECTOR3();
		buildingscales[5]->x = 10;
		buildingscales[5]->y = 30;
		buildingscales[5]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 30, 10, &buildingmesh[6], NULL);
		buildinglocations[6] = new D3DXVECTOR3();
		buildinglocations[6]->x = -30;
		buildinglocations[6]->y = 15;
		buildinglocations[6]->z = 50;
		buildingscales[6] = new D3DXVECTOR3();
		buildingscales[6]->x = 10;
		buildingscales[6]->y = 30;
		buildingscales[6]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 30, 10, &buildingmesh[7], NULL);
		buildinglocations[7] = new D3DXVECTOR3();
		buildinglocations[7]->x = 30;
		buildinglocations[7]->y = 15;
		buildinglocations[7]->z = 50;
		buildingscales[7] = new D3DXVECTOR3();
		buildingscales[7]->x = 10;
		buildingscales[7]->y = 30;
		buildingscales[7]->z = 10;
		D3DXCreateBox(g_pDevice, 20, 10, 10, &buildingmesh[8], NULL);
		buildinglocations[8] = new D3DXVECTOR3();
		buildinglocations[8]->x = -30;
		buildinglocations[8]->y = 35;
		buildinglocations[8]->z = 50;
		buildingscales[8] = new D3DXVECTOR3();
		buildingscales[8]->x = 20;
		buildingscales[8]->y = 10;
		buildingscales[8]->z = 10;
		D3DXCreateBox(g_pDevice, 20, 10, 10, &buildingmesh[9], NULL);
		buildinglocations[9] = new D3DXVECTOR3();
		buildinglocations[9]->x = 30;
		buildinglocations[9]->y = 35;
		buildinglocations[9]->z = 50;
		buildingscales[9] = new D3DXVECTOR3();
		buildingscales[9]->x = 20;
		buildingscales[9]->y = 10;
		buildingscales[9]->z = 10;
		D3DXCreateBox(g_pDevice, 40, 70, 10, &buildingmesh[10], NULL);
		buildinglocations[10] = new D3DXVECTOR3();
		buildinglocations[10]->x = -30;
		buildinglocations[10]->y = 35;
		buildinglocations[10]->z = 70;
		buildingscales[10] = new D3DXVECTOR3();
		buildingscales[10]->x = 40;
		buildingscales[10]->y = 70;
		buildingscales[10]->z = 10;
		D3DXCreateBox(g_pDevice, 40, 70, 10, &buildingmesh[11], NULL);
		buildinglocations[11] = new D3DXVECTOR3();
		buildinglocations[11]->x = 30;
		buildinglocations[11]->y = 35;
		buildinglocations[11]->z = 70;
		buildingscales[11] = new D3DXVECTOR3();
		buildingscales[11]->x = 40;
		buildingscales[11]->y = 70;
		buildingscales[11]->z = 10;
		D3DXCreateBox(g_pDevice, 40, 70, 10, &buildingmesh[12], NULL);
		buildinglocations[12] = new D3DXVECTOR3();
		buildinglocations[12]->x = -30;
		buildinglocations[12]->y = 35;
		buildinglocations[12]->z = -70;
		buildingscales[12] = new D3DXVECTOR3();
		buildingscales[12]->x = 40;
		buildingscales[12]->y = 70;
		buildingscales[12]->z = 10;
		D3DXCreateBox(g_pDevice, 40, 70, 10, &buildingmesh[13], NULL);
		buildinglocations[13] = new D3DXVECTOR3();
		buildinglocations[13]->x = 30;
		buildinglocations[13]->y = 35;
		buildinglocations[13]->z = -70;
		buildingscales[13] = new D3DXVECTOR3();
		buildingscales[13]->x = 40;
		buildingscales[13]->y = 70;
		buildingscales[13]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 70, 40, &buildingmesh[14], NULL);
		buildinglocations[14] = new D3DXVECTOR3();
		buildinglocations[14]->x = -70;
		buildinglocations[14]->y = 35;
		buildinglocations[14]->z = 0;
		buildingscales[14] = new D3DXVECTOR3();
		buildingscales[14]->x = 10;
		buildingscales[14]->y = 70;
		buildingscales[14]->z = 40;
		D3DXCreateBox(g_pDevice, 10, 70, 40, &buildingmesh[15], NULL);
		buildinglocations[15] = new D3DXVECTOR3();
		buildinglocations[15]->x = 70;
		buildinglocations[15]->y = 35;
		buildinglocations[15]->z = 0;
		buildingscales[15] = new D3DXVECTOR3();
		buildingscales[15]->x = 10;
		buildingscales[15]->y = 70;
		buildingscales[15]->z = 40;
		D3DXCreateBox(g_pDevice, 140, 10, 10, &buildingmesh[16], NULL);
		buildinglocations[16] = new D3DXVECTOR3();
		buildinglocations[16]->x = 0;
		buildinglocations[16]->y = 55;
		buildinglocations[16]->z = 10;
		buildingscales[16] = new D3DXVECTOR3();
		buildingscales[16]->x = 140;
		buildingscales[16]->y = 10;
		buildingscales[16]->z = 10;
		D3DXCreateBox(g_pDevice, 10, 60, 50, &buildingmesh[17], NULL);
		buildinglocations[17] = new D3DXVECTOR3();
		buildinglocations[17]->x = -100;
		buildinglocations[17]->y = 30;
		buildinglocations[17]->z = 20;
		buildingscales[17] = new D3DXVECTOR3();
		buildingscales[17]->x = 10;
		buildingscales[17]->y = 60;
		buildingscales[17]->z = 50;
		D3DXCreateBox(g_pDevice, 10, 60, 50, &buildingmesh[18], NULL);
		buildinglocations[18] = new D3DXVECTOR3();
		buildinglocations[18]->x = 100;
		buildinglocations[18]->y = 30;
		buildinglocations[18]->z = 20;
		buildingscales[18] = new D3DXVECTOR3();
		buildingscales[18]->x = 10;
		buildingscales[18]->y = 60;
		buildingscales[18]->z = 50;

		for(int i = 0; i < 19; i++)
		{
			D3DXMATRIX translate = directXClass::Translate(buildinglocations[i]->x, buildinglocations[i]->y, buildinglocations[i]->z);
			D3DXMATRIX scale = directXClass::Translate(0,0,0);
			scale(0,0) = buildingscales[i]->x;
			scale(1,1) = buildingscales[i]->y;
			scale(2,2) = buildingscales[i]->z;
			g_pDevice->SetTransform(D3DTS_WORLD, &(translate));
			buildingBounds[i] = boundingBox(buildingmesh[i], g_pDevice);
			for (int i2 = 0; i2 < 12; i2++) {
				buildinglocationsEdges[i][i2] = new D3DXVECTOR3(0,0,0);
				buildingscalesEdges[i][i2] = new D3DXVECTOR3(0.5,0.5,0.5);
			}
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][0], NULL);
			buildinglocationsEdges[i][0]->x = buildinglocations[i]->x+buildingscales[i]->x/2;
			buildinglocationsEdges[i][0]->z = buildinglocations[i]->z+buildingscales[i]->z/2;
			buildinglocationsEdges[i][0]->y = buildinglocations[i]->y;
			buildingscalesEdges[i][0]->y = buildingscales[i]->y;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][1], NULL);
			buildinglocationsEdges[i][1]->x = buildinglocations[i]->x-buildingscales[i]->x/2;
			buildinglocationsEdges[i][1]->z = buildinglocations[i]->z-buildingscales[i]->z/2;
			buildinglocationsEdges[i][1]->y = buildinglocations[i]->y;
			buildingscalesEdges[i][1]->y = buildingscales[i]->y;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][2], NULL);
			buildinglocationsEdges[i][2]->x = buildinglocations[i]->x-buildingscales[i]->x/2;
			buildinglocationsEdges[i][2]->z = buildinglocations[i]->z+buildingscales[i]->z/2;
			buildinglocationsEdges[i][2]->y = buildinglocations[i]->y;
			buildingscalesEdges[i][2]->y = buildingscales[i]->y;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][3], NULL);
			buildinglocationsEdges[i][3]->x = buildinglocations[i]->x+buildingscales[i]->x/2;
			buildinglocationsEdges[i][3]->z = buildinglocations[i]->z-buildingscales[i]->z/2;
			buildinglocationsEdges[i][3]->y = buildinglocations[i]->y;
			buildingscalesEdges[i][3]->y = buildingscales[i]->y;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][4], NULL);
			buildinglocationsEdges[i][4]->x = buildinglocations[i]->x+buildingscales[i]->x/2;
			buildinglocationsEdges[i][4]->z = buildinglocations[i]->z;
			buildinglocationsEdges[i][4]->y = buildinglocations[i]->y+buildingscales[i]->y/2;
			buildingscalesEdges[i][4]->z = buildingscales[i]->z;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][5], NULL);
			buildinglocationsEdges[i][5]->x = buildinglocations[i]->x-buildingscales[i]->x/2;
			buildinglocationsEdges[i][5]->z = buildinglocations[i]->z;
			buildinglocationsEdges[i][5]->y = buildinglocations[i]->y-buildingscales[i]->y/2;
			buildingscalesEdges[i][5]->z = buildingscales[i]->z;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][6], NULL);
			buildinglocationsEdges[i][6]->x = buildinglocations[i]->x+buildingscales[i]->x/2;
			buildinglocationsEdges[i][6]->z = buildinglocations[i]->z;
			buildinglocationsEdges[i][6]->y = buildinglocations[i]->y-buildingscales[i]->y/2;
			buildingscalesEdges[i][6]->z = buildingscales[i]->z;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][7], NULL);
			buildinglocationsEdges[i][7]->x = buildinglocations[i]->x-buildingscales[i]->x/2;
			buildinglocationsEdges[i][7]->z = buildinglocations[i]->z;
			buildinglocationsEdges[i][7]->y = buildinglocations[i]->y+buildingscales[i]->y/2;
			buildingscalesEdges[i][7]->z = buildingscales[i]->z;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][8], NULL);
			buildinglocationsEdges[i][8]->x = buildinglocations[i]->x;
			buildinglocationsEdges[i][8]->z = buildinglocations[i]->z+buildingscales[i]->z/2;
			buildinglocationsEdges[i][8]->y = buildinglocations[i]->y+buildingscales[i]->y/2;
			buildingscalesEdges[i][8]->x = buildingscales[i]->x;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][9], NULL);
			buildinglocationsEdges[i][9]->x = buildinglocations[i]->x;
			buildinglocationsEdges[i][9]->z = buildinglocations[i]->z-buildingscales[i]->z/2;
			buildinglocationsEdges[i][9]->y = buildinglocations[i]->y-buildingscales[i]->y/2;
			buildingscalesEdges[i][9]->x = buildingscales[i]->x;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][10], NULL);
			buildinglocationsEdges[i][10]->x = buildinglocations[i]->x;
			buildinglocationsEdges[i][10]->z = buildinglocations[i]->z+buildingscales[i]->z/2;
			buildinglocationsEdges[i][10]->y = buildinglocations[i]->y-buildingscales[i]->y/2;
			buildingscalesEdges[i][10]->x = buildingscales[i]->x;
			D3DXCreateBox(g_pDevice, 1, 1, 1, &buildingmeshEdges[i][11], NULL);
			buildinglocationsEdges[i][11]->x = buildinglocations[i]->x;
			buildinglocationsEdges[i][11]->z = buildinglocations[i]->z-buildingscales[i]->z/2;
			buildinglocationsEdges[i][11]->y = buildinglocations[i]->y+buildingscales[i]->y/2;
			buildingscalesEdges[i][11]->x = buildingscales[i]->x;

		}
	}
	for (int index = 0; index < 19; index++) {
		D3DXVECTOR3 currentLoc = (*buildinglocations[index] * 0.25f) + D3DXVECTOR3(28,0,19);
		D3DXVECTOR3 currentScale = (*buildingscales[index] * 0.25f);
		for (int x = currentLoc.x-currentScale.x/2; x < currentLoc.x+currentScale.x/2; x++) {
			for (int y = currentLoc.y-currentScale.y/2; y < currentLoc.y+currentScale.y/2; y++) {
				for (int z = currentLoc.z-currentScale.z/2; z < currentLoc.z+currentScale.z/2; z++) {
					grid[x][y][z] = 4;
				}
			}
		}
	}
	/*for (int y = 0; y < 20; y++) {
		directXClass::SetError(TEXT("Y: %d\n"), y);
		showGrid(y);
	}*/
}

void Terrain::setupBuilding(D3DXVECTOR3 _translate, D3DXVECTOR3 _scale, int index)
{
	D3DXMATRIX translate = directXClass::Translate(_translate.x, _translate.y, _translate.z);
	D3DXMATRIX scale = directXClass::Translate(0,0,0);
	scale(0,0) = _scale.x;
	scale(1,1) = _scale.y;
	scale(2,2) = _scale.z;
	g_pDevice->SetTransform(D3DTS_WORLD, &(translate));
    D3DMATERIAL9* mat = new D3DMATERIAL9();
    mat->Ambient= D3DXCOLOR(0.5f, 0.5f, 0.5f, 255.0f);
    g_pDevice->SetMaterial(mat);
	buildingmesh[index]->DrawSubset(0);

	buildingBounds[index].drawNodes(translate, _translate);
	for (int index2 = 0; index2 < 12; index2++) {
		D3DXMATRIX translate = directXClass::Translate(buildinglocationsEdges[index][index2]->x, buildinglocationsEdges[index][index2]->y, buildinglocationsEdges[index][index2]->z);
		D3DXMATRIX scale = directXClass::Translate(0,0,0);
		scale(0,0) = buildingscalesEdges[index][index2]->x;
		scale(1,1) = buildingscalesEdges[index][index2]->y;
		scale(2,2) = buildingscalesEdges[index][index2]->z;
		g_pDevice->SetTransform(D3DTS_WORLD, &(scale * translate));
		D3DMATERIAL9* mat = new D3DMATERIAL9();
		mat->Ambient= D3DXCOLOR(0.0f, 1.0f, 0.0f, 255.0f);
		g_pDevice->SetMaterial(mat);
		buildingmeshEdges[index][index2]->DrawSubset(0);
	}
}

void Terrain::showGrid(int y) {
	for (int z = 0; z < 38; z++) {
			for (int x = 0; x < 55; x++) {
				directXClass::SetError(TEXT("%d"), grid[x][y][z]);
			}
		directXClass::SetError(TEXT("\n"));
	}
}