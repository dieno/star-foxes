#include "camera.h"

void Camera::Update(float timeDelta)
{
	UpdateWorldPositions();

	D3DXVECTOR3 vStretch = vPosition_ - vDesiredPosition_;
	D3DXVECTOR3 vForce = -stiffness_ * vStretch - damping_ * vVelocity_;

	D3DXVECTOR3 vAccel = vForce / mass_;
	vVelocity_ += vAccel * timeDelta;

	vPosition_ += vVelocity_ * timeDelta;

	UpdateMatrix();
}

void Camera::UpdateWorldPositions()
{
	//vChaseDirection_ *= -1.0f;

	D3DXMATRIX transform;
	D3DXMatrixIdentity(&transform);
	
	D3DXVec3Normalize(&vUp_, &vUp_);
	D3DXVec3Normalize(&vChaseDirection_, &vChaseDirection_);

	D3DXVECTOR3 vRight;
	D3DXVec3Cross(&vRight, &vUp_, &vChaseDirection_);

	transform(0, 0) = vRight.x; transform(0, 1) = vUp_.x; transform(0, 2) = -vChaseDirection_.x; //transform(0, 3) = 0.0f;
	transform(1, 0) = vRight.y; transform(1, 1) = vUp_.y; transform(1, 2) = vChaseDirection_.y;// transform(1, 3) = 0.0f;
	transform(2, 0) = vRight.z; transform(2, 1) = vUp_.z; transform(2, 2) = vChaseDirection_.z; //transform(2, 3) = 0.0f;
	//transform(3, 0) = 0.0f;     transform(3, 1) = 0.0f;   transform(3, 2) = 0.0f;       transform(3, 3) = 1.0f;

	D3DXMATRIX mTransposedT;
	D3DXMatrixTranspose(&mTransposedT, &transform);

	D3DXVECTOR3 vDesiredOffset;
	D3DXVec3TransformNormal(&vDesiredOffset, &vDesiredPositionOffset_, &mTransposedT);
	vDesiredPosition_ = vChasePosition_ + vDesiredOffset;

	D3DXVECTOR3 vLookOffset;
	D3DXVec3TransformNormal(&vLookOffset, &vLookAtOffset_, &mTransposedT);
	vLookAt_ = vChasePosition_ + vLookOffset;	

}

void Camera::UpdateMatrix()
{
	D3DXMatrixLookAtLH(&mView_,
						&vPosition_, // the camera position
						&vLookAt_,	 // the look-at position
						&vUp_);      // the up direction
	//directXClass::SetError(TEXT("cam: X: %f, Y: %f, Z: %f"), vLookAt_.x, vLookAt_.y, vLookAt_.z);
}

void Camera::reset()
{
	UpdateWorldPositions();

	vVelocity_ = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	vPosition_ = vDesiredPosition_;

	UpdateMatrix();
}
