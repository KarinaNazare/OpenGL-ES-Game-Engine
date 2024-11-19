#include <stdafx.h>
#include "Camera.h"


Camera::Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat fov, GLfloat near, GLfloat far) : position(position), target(target), up(up), fov(fov), near(near), far(far)
{
	updateWorldView();
}


Camera::Camera(Vector3 position, Vector3 target, Vector3 up) : Camera(position, target, up, 45.0f, 0.1f, 200.0f)
{
}


Camera::Camera() : Camera(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 1, 0))
{
}


void Camera::moveOx(GLfloat speed)
{
	position += xAxis * speed * deltaTime;
	target += xAxis * speed * deltaTime;

	updateWorldView();
}


void Camera::moveOy(GLfloat speed)
{
	position += yAxis * speed * deltaTime;
	target += yAxis * speed * deltaTime;

	updateWorldView();
}


void Camera::moveOz(GLfloat speed)
{
	position += zAxis * speed * deltaTime;
	target += zAxis * speed * deltaTime;

	updateWorldView();
}


void Camera::rotateOx(GLfloat speed)
{
	Matrix mRotationOx;
	Vector4 rotatedUp;
	Vector4 localTarget, rotatedTarget;

	mRotationOx.SetRotationX(speed * deltaTime);
	rotatedUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * mRotationOx * worldMatrix;
	up = Vector3(rotatedUp.x, rotatedUp.y, rotatedUp.z).Normalize();

	localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	rotatedTarget = localTarget * mRotationOx * worldMatrix;
	target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	updateWorldView();
}


void Camera::rotateOy(GLfloat speed)
{
	Matrix mRotationOy;
	Vector4 localTarget, rotatedTarget;

	mRotationOy.SetRotationY(speed * deltaTime);
	localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	rotatedTarget = localTarget * mRotationOy * worldMatrix;
	target = Vector3(rotatedTarget.x, rotatedTarget.y, rotatedTarget.z);

	updateWorldView();
}


void Camera::rotateOz(GLfloat speed)
{
	Matrix mRotationOz;
	Vector4 rotatedUp;

	mRotationOz.SetRotationZ(speed * deltaTime);
	rotatedUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f) * mRotationOz * worldMatrix;
	up = Vector3(rotatedUp.x, rotatedUp.y, rotatedUp.z).Normalize();

	updateWorldView();
}


void Camera::updateWorldView()
{
	Matrix R, RInverse, T, TInverse;

	//update camera axes
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	//initialize matrix R and R^-1 (which is the transpose, because R is an orthogonal matrix)
	R.m[0][0] = xAxis.x;  R.m[0][1] = xAxis.y;  R.m[0][2] = xAxis.z;  R.m[0][3] = 0;
	R.m[1][0] = yAxis.x;  R.m[1][1] = yAxis.y;  R.m[1][2] = yAxis.z;  R.m[1][3] = 0;
	R.m[2][0] = zAxis.x;  R.m[2][1] = zAxis.y;  R.m[2][2] = zAxis.z;  R.m[2][3] = 0;
	R.m[3][0] = 0;        R.m[3][1] = 0;        R.m[3][2] = 0;        R.m[3][3] = 1;

	RInverse = R.Transpose();

	//initialize matrix T (translation with position) and T^-1 (translation with -position)
	T.SetTranslation(position);
	TInverse.SetTranslation(-position);

	//update world matrix and view matrix
	worldMatrix = R * T;
	viewMatrix = TInverse * RInverse;
}


GLfloat Camera::getFOV()
{
	return fov;
}


void Camera::setFOV(GLfloat fov)
{
	this->fov = fov;
}


GLfloat Camera::getNear()
{
	return near;
}


void Camera::setNear(GLfloat near)
{
	this->near = near;
}


GLfloat Camera::getFar()
{
	return far;
}


void Camera::setFar(GLfloat far)
{
	this->far = far;
}


void Camera::setDeltaTime(GLfloat deltaTime)
{
	this->deltaTime = deltaTime;
}


Matrix Camera::getWorldMatrix()
{
	return worldMatrix;
}


Matrix Camera::getViewMatrix()
{
	return viewMatrix;
}
