#pragma once
#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include"stdafx.h"
#include<vector>
using namespace std;
class Terrain : public SceneObject
{



public:
	Vector3 center;
	int dim;
	int nr_cell;
	float offsetY;
	Vector3 height;

	GLuint iboId;
	GLuint vboId;
	std::vector<GLushort> indices;
	std::vector<std::pair<GLuint, GLuint>> lines;
	vector<Vertex> vertices;

	Terrain() { modelMat = T; };
	~Terrain() {};
	void Generate();
	virtual void Draw();
	virtual void Update(GLfloat deltaTime);
};