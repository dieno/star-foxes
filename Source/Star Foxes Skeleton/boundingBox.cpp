#include "boundingBox.h"
#include "directXClass.h"

void boundingBox::setupNodes()
{
	for(int i = 0; i < 16; i++)
	{
		D3DXCreateBox(g_pDevice, 1, 1, 1, &boundingmesh[i], NULL);
	}
}

void boundingBox::drawNodes(D3DXMATRIX world, D3DXVECTOR3 position)
{

	axisMinBounds.x = m_objectBounds[0].x + position.x;
	axisMaxBounds.x = m_objectBounds[0].x + position.x;
	axisMinBounds.y = m_objectBounds[0].y + position.y;
	axisMaxBounds.y = m_objectBounds[0].y + position.y;
	axisMinBounds.z = m_objectBounds[0].z + position.z;
   	axisMaxBounds.z = m_objectBounds[0].z + position.z;

	for(int i = 1; i < 8; i++)
	{
		if ((m_objectBounds[i].x + position.x) < axisMinBounds.x) axisMinBounds.x = (m_objectBounds[i].x + position.x);
        if ((m_objectBounds[i].x + position.x) > axisMaxBounds.x) axisMaxBounds.x = (m_objectBounds[i].x + position.x);
        if ((m_objectBounds[i].y + position.y) < axisMinBounds.y) axisMinBounds.y = (m_objectBounds[i].y + position.y);
        if ((m_objectBounds[i].y + position.y) > axisMaxBounds.y) axisMaxBounds.y = (m_objectBounds[i].y + position.y);
        if ((m_objectBounds[i].z + position.z) < axisMinBounds.z) axisMinBounds.z = (m_objectBounds[i].z + position.z);
        if ((m_objectBounds[i].z + position.z) > axisMaxBounds.z) axisMaxBounds.z = (m_objectBounds[i].z + position.z);
	}

	axis_objectBounds[0] = D3DXVECTOR3( axisMinBounds.x, axisMinBounds.y, axisMinBounds.z ); // xyz
	axis_objectBounds[1] = D3DXVECTOR3( axisMaxBounds.x, axisMinBounds.y, axisMinBounds.z ); // Xyz
	axis_objectBounds[2] = D3DXVECTOR3( axisMinBounds.x, axisMaxBounds.y, axisMinBounds.z ); // xYz
	axis_objectBounds[3] = D3DXVECTOR3( axisMaxBounds.x, axisMaxBounds.y, axisMinBounds.z ); // XYz
	axis_objectBounds[4] = D3DXVECTOR3( axisMinBounds.x, axisMinBounds.y, axisMaxBounds.z ); // xyZ
	axis_objectBounds[5] = D3DXVECTOR3( axisMaxBounds.x, axisMinBounds.y, axisMaxBounds.z ); // XyZ
	axis_objectBounds[6] = D3DXVECTOR3( axisMinBounds.x, axisMaxBounds.y, axisMaxBounds.z ); // xYZ
	axis_objectBounds[7] = D3DXVECTOR3( axisMaxBounds.x, axisMaxBounds.y, axisMaxBounds.z ); // XYZ

	for(int i = 0; i < 8; i++)
	{
		D3DXMATRIX axistranslate = directXClass::Translate(axis_objectBounds[i].x, axis_objectBounds[i].y, axis_objectBounds[i].z);
		g_pDevice->SetTransform(D3DTS_WORLD, &axistranslate);
		g_pDevice->SetMaterial(axisMat);
		//boundingmesh[(i + 8)]->DrawSubset(0);
	}
	g_pDevice->SetTransform(D3DTS_WORLD, &(world));
}

bool boundingBox::checkCollision(D3DXVECTOR3 position, boundingBox object2)
{
	bool position_in_ob2 = false;

	for(int i = 0; i < 8; i++)
	{
		if(position.x > object2.axisMinBounds.x &&
		   position.x < object2.axisMaxBounds.x &&
		   position.y > object2.axisMinBounds.y &&
		   position.y < object2.axisMaxBounds.y &&
		   position.z > object2.axisMinBounds.z &&
		   position.z < object2.axisMaxBounds.z)
		{
			position_in_ob2 = true;
		}
	}
	
	return position_in_ob2;
}