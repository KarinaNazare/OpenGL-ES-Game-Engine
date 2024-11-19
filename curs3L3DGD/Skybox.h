#pragma once
#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include"stdafx.h"
#include<vector>
using namespace std;
class Skybox : public SceneObject
{



public:
	//Vector3 center;

	float offset;
	float rotTime;

	Skybox() { rotTime = 0; };
	~Skybox() {};

	virtual void Draw();
	virtual void Update(GLfloat deltaTime);
};