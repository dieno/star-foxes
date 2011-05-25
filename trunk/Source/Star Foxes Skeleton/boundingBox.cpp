#include "boundingBox.h"
#include "directXClass.h"

void boundingBox::setupNodes()
{
	for(int i = 0; i < 8; i++)
	{
		D3DXCreateBox(g_pDevice, 1, 1, 1, &boundingmesh[i], NULL);
	}
}

void boundingBox::drawNodes(D3DXMATRIX world)
{
	for(int i = 0; i < 8; i++)
	{
		D3DXMATRIX translate = directXClass::Translate(m_objectBounds[i].x, m_objectBounds[i].y, m_objectBounds[i].z);
		g_pDevice->SetTransform(D3DTS_WORLD, &(translate * world));
		g_pDevice->SetMaterial(mat);
		boundingmesh[i]->DrawSubset(0);
	}
	g_pDevice->SetTransform(D3DTS_WORLD, &(world));
}