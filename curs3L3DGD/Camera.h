#pragma once

#include "../Utilities/utilities.h"

#undef near
#undef far


class Camera
{

	Vector3 target, up;
	GLfloat fov, near, far;
	GLfloat deltaTime;
	Vector3 xAxis, yAxis, zAxis;
	Matrix worldMatrix, viewMatrix;

public:
	Vector3 position;
	Camera(Vector3 position, Vector3 target, Vector3 up, GLfloat fov, GLfloat near, GLfloat far);
	Camera(Vector3 position, Vector3 target, Vector3 up);
	Camera();

	void moveOx(GLfloat speed);
	void moveOy(GLfloat speed);
	void moveOz(GLfloat speed);

	void rotateOx(GLfloat speed);
	void rotateOy(GLfloat speed);
	void rotateOz(GLfloat speed);

	void updateWorldView();

	GLfloat getFOV();
	void setFOV(GLfloat fov);

	GLfloat getNear();
	void setNear(GLfloat near);

	GLfloat getFar();
	void setFar(GLfloat far);

	void setDeltaTime(GLfloat deltaTime);

	Matrix getWorldMatrix();
	Matrix getViewMatrix();

};
