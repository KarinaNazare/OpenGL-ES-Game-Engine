#pragma once

#include "../Utilities/utilities.h"
#include "ModelResource.h"
#include "../Utilities/utilities.h"
#include "Vertex.h"
#include "Texture.h"

#include <vector>
#include <tuple>
#include <string>


class Model {
public:
	ModelResource* mr;
	//GLuint VertexArrayID;
	GLuint iboId;
	GLuint vboId;
	//int nrIndici;

	Vector3 boxMin, boxMax;
	std::vector<std::tuple<GLushort, GLushort, GLushort>> indices;
	std::vector<std::pair<GLushort, GLushort>> lines;

	std::vector<Vertex> vertices;

	Model(string p);

	Model();

	bool Load();
private:
	void parseNfgFile(std::string fileName);
	std::vector<std::string> split(std::string &string, char delimiter);
	bool endsWith(std::string &string, std::string &ending);
};
