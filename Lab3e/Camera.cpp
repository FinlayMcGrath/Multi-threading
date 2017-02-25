#include "Camera.h"

Camera::Camera()
{
	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;

	velocity = 25;
	rotationalVelocity = 50;
	position.set(0.0f, 0.0f, 250.0f);
	CameraUpdate();
}

void Camera::CameraUpdate()
{
	Yaw = rotation.getX();
	Pitch = rotation.getY();
	Roll = rotation.getZ();
	float cosR, cosP, cosY;	//temp values for sin/cos from 
	float sinR, sinP, sinY;	
	
	// Roll, Pitch and Yaw are variables stored by the camera
	// handle rotation
	// Only Want to calculate these values once. 
	// For speeds sake. 
	cosY = cosf(Yaw * PI / 180.0f);
	cosP = cosf(Pitch * PI / 180.0f);
	cosR = cosf(Roll * PI / 180.0f);
	sinY = sinf(Yaw * PI / 180.0f);
	sinP = sinf(Pitch * PI / 180.0f);
	sinR = sinf(Roll * PI / 180.0f);

	//This using the parametric equation of a sphere

	// Calculate the three vectors to put into glu Lookat
	// Look direction,  position and the up vector 
	// This function could also calculate the right vector

	forward.setX(sinY * cosP);
	forward.setY(sinP);
	forward.setZ(cosP * -cosY);

	// Look At Point
	// To calculate add Forward Vector to Camera position.	
	
	look.setX(forward.getX() + position.getX());
	look.setY(forward.getY() + position.getY());
	look.setZ(forward.getZ() + position.getZ());

	// Up Vector
	up.setX(-cosY * sinR - sinY * sinP * cosR);
	up.setY(cosP * cosR);
	up.setZ(sinY * sinR - sinP * cosR * -cosY);

	// Side Vector (right)
	// this is a cross product between the forward and up vector. 
	// If you don’t need to calculate this,  don’t do it. 
	Vec3 temp = forward.cross(up);
	right.set(temp.getX(), temp.getY(), temp.getZ());
}

void Camera::setForward(float x, float y, float z)
{
	forward.setX(x);
	forward.setY(y);
	forward.setZ(z);
}

void Camera::setUp(float x, float y, float z)
{
	up.setX(x);
	up.setY(y);
	up.setZ(z);
}

void Camera::setRight(float x, float y, float z)
{
	right.setX(x);
	right.setY(y);
	right.setZ(z);
}

void Camera::setLook(float x, float y, float z)
{
	look.setX(x);
	look.setY(y);
	look.setZ(z);
}

void Camera::setPosition(float x, float y, float z)
{
	position.setX(x);
	position.setY(y);
	position.setZ(z);
}

void Camera::setRotation(float x, float y, float z)
{
	rotation.setX(x);
	rotation.setY(y);
	rotation.setZ(z);
}