#ifndef CAMERA_H
#define CAMERA_H

#include "directXHeader.h"

class Camera
{
public:
	Camera(): vChasePosition_(0.0f,0.0f,0.0f), 
			  vChaseDirection_(0.0f,0.0f,0.0f), 
			  vUp_(0.0f,1.0f,0.0f), 
			  vDesiredPositionOffset_(0.0f,10.0f,-20.0f),
			  vDesiredPosition_(0.0f,0.0f,0.0f), 
			  vLookAt_(0.0f,0.0f,0.0f),
			  vLookAtOffset_(0.0f,0.0f,0.0f), 
			  stiffness_(1800.0f), 
			  damping_(600.0f), 
			  mass_(50.0f),
			  vPosition_(0.0f,0.0f,0.0f),
			  vVelocity_(0.0f,0.0f,0.0f),
			  mView_() {}
	
	// updates camera position
	void Update(float timeDelta);

	// resets to original position
	void reset();

	// gettters + setters:

	// returns the current view matrix
	D3DXMATRIX getViewMatrix() {return mView_;}

	void setvChasePosition(D3DXVECTOR3 vec3) {vChasePosition_ = vec3;}

	void setvChaseDirection(D3DXVECTOR3 vec3) {vChaseDirection_ = vec3;}

	void setvUp(D3DXVECTOR3 vec3) {vUp_ = vec3;} 

private:
	// chased objects properties:

	// position of object being chased
	D3DXVECTOR3 vChasePosition_;
	
	// direction the object's moving in
	D3DXVECTOR3 vChaseDirection_;

	// object's up vector
	D3DXVECTOR3 vUp_;
	
	// desired camera positioning:

	// desired camera position in the chased object's coord system
	D3DXVECTOR3 vDesiredPositionOffset_;

	// desired camera position in world space;
	D3DXVECTOR3 vDesiredPosition_;

	// camera's look at offset
	D3DXVECTOR3 vLookAtOffset_;

	// camera's look at vector
	D3DXVECTOR3 vLookAt_;

	// camera physics:
	
	// how close will the camera stay to the object
	float stiffness_;
	
	// prevents over oscillating
	float damping_;

	// mass of the camera "body"
	float mass_;

	// camera's position in world space
	D3DXVECTOR3 vPosition_;

	// camera's velocity
	D3DXVECTOR3 vVelocity_;

	// camera's view matrix
	D3DXMATRIX mView_;

	// function prototypes:

	// rebuilds mView matrix
	void UpdateMatrix();

	void UpdateWorldPositions();
};

#endif