#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "directXHeader.h"
#include "gamecore.h"

class boundingBox {
public:
	boundingBox()
	{
	}
	boundingBox(LPD3DXMESH _boundingMesh, LPDIRECT3DDEVICE9 _g_pDevice)
	{
		g_pDevice = _g_pDevice;
		hr=_boundingMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pVertices);
		if (FAILED(hr))
		{
			
		}

		D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, _boundingMesh->GetNumVertices(), 
		D3DXGetFVFVertexSize(_boundingMesh->GetFVF()), &minBounds, &maxBounds);

		_boundingMesh->UnlockVertexBuffer();

		m_objectBounds[0] = D3DXVECTOR3( minBounds.x, minBounds.y, minBounds.z ); // xyz
		m_objectBounds[1] = D3DXVECTOR3( maxBounds.x, minBounds.y, minBounds.z ); // Xyz
		m_objectBounds[2] = D3DXVECTOR3( minBounds.x, maxBounds.y, minBounds.z ); // xYz
		m_objectBounds[3] = D3DXVECTOR3( maxBounds.x, maxBounds.y, minBounds.z ); // XYz
		m_objectBounds[4] = D3DXVECTOR3( minBounds.x, minBounds.y, maxBounds.z ); // xyZ
		m_objectBounds[5] = D3DXVECTOR3( maxBounds.x, minBounds.y, maxBounds.z ); // XyZ
		m_objectBounds[6] = D3DXVECTOR3( minBounds.x, maxBounds.y, maxBounds.z ); // xYZ
		m_objectBounds[7] = D3DXVECTOR3( maxBounds.x, maxBounds.y, maxBounds.z ); // XYZ

		mat = new D3DMATERIAL9();
		mat->Ambient= D3DXCOLOR(0.0f, 0.0f, 0.5f, 255.0f);

		setupNodes();

	}
	void drawNodes(D3DXMATRIX world);
private:
	D3DXVECTOR3 minBounds,maxBounds;
	HRESULT hr;
	BYTE* pVertices;
	D3DXVECTOR3 m_objectBounds [8];

	void setupNodes();
	LPDIRECT3DDEVICE9 g_pDevice;
	D3DMATERIAL9* mat;
	ID3DXMesh *boundingmesh[256];
	D3DXVECTOR3 *boundinglocations[256];
};
#endif