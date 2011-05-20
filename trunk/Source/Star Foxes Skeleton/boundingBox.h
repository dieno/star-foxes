#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

class BoundingBox
{
public:
	BoundingBox():
	  minBounds_(0.0f,0.0f,0.0f),
	  maxBounds_(0.0f,0.0f,0.0f)
	{}

	D3DXVECTOR3 minBounds_, maxBounds_;
	D3DXVECTOR3 objectBounds_[8];

	void calcObjectBounds()
	{
		objectBounds_[0] = D3DXVECTOR3( minBounds_.x, minBounds_.y, minBounds_.z ); // xyz
		objectBounds_[1] = D3DXVECTOR3( maxBounds_.x, minBounds_.y, minBounds_.z ); // Xyz
		objectBounds_[2] = D3DXVECTOR3( minBounds_.x, maxBounds_.y, minBounds_.z ); // xYz
		objectBounds_[3] = D3DXVECTOR3( maxBounds_.x, maxBounds_.y, minBounds_.z ); // XYz
		objectBounds_[4] = D3DXVECTOR3( minBounds_.x, minBounds_.y, maxBounds_.z ); // xyZ
		objectBounds_[5] = D3DXVECTOR3( maxBounds_.x, minBounds_.y, maxBounds_.z ); // XyZ
		objectBounds_[6] = D3DXVECTOR3( minBounds_.x, maxBounds_.y, maxBounds_.z ); // xYZ
		objectBounds_[7] = D3DXVECTOR3( maxBounds_.x, maxBounds_.y, maxBounds_.z ); // XYZ
	}
};

#endif