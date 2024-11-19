#pragma once
#include"stdafx.h"
#include"Model.h"
#include"Texture.h"
#include "../Utilities/utilities.h"
#include <string>
#include<vector>
#include "Shaders.h"
#include "Camera.h"

using namespace std;

class SceneObject {
public:
	Model *model;
	Shaders *shader;
	//Camera *camera;
	Matrix modelMat;

	int idModel, idShader;
	vector<int> textures;
	vector<int> lights;

	string type, name;
	bool blend = 0, depthTest = 0;

	Vector3 position, rotation, scale;

	Matrix Rx, Ry, Rz, S, T;

	bool isRendered();
	bool intersects(SceneObject* so);
	SceneObject() {}
	virtual void Draw();
	virtual void Update(GLfloat deltaTime);
};
