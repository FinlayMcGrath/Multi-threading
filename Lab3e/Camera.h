#ifndef _CAMERACLASS_H_
#define	_CAMERACLASS_H_

#include "Vec3.h"

#define PI 3.14159
class Camera
{
public:
	Camera();
	void CameraUpdate();
	Vec3 getForward() {return forward;};
	Vec3 getUp() {return up;};
	Vec3 getRight() {return right;};
	Vec3 getLook() {return look;};
	Vec3 getPosition() {return position;};
	void setForward(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setRight(float x, float y, float z); 
	void setLook(float x, float y, float z); 
	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	Vec3 getRotation() {return rotation;};
	float getVelocity(){return velocity;};
	void setVelocity(float Velocity){velocity = Velocity;};
	float getRotationalVelocity(){return rotationalVelocity;};
	void setRotationalVelocity(float RotationalVelocity){rotationalVelocity = RotationalVelocity;};
private:
	float Yaw, Pitch, Roll, velocity, rotationalVelocity;
	Vec3 forward, up, right, look, position, rotation;
};

#endif