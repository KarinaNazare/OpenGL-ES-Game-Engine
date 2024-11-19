#pragma once
#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include"stdafx.h"
#include<vector>
#include <time.h>
using namespace std;
class Fire : public SceneObject
{
public:

	float dispMax;
	float time;
	Vector3 height;

	
	Fire() ;
	~Fire() {};

	virtual void Draw();
	virtual void Update(GLfloat deltaTime);
};