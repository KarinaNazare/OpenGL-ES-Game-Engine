#pragma once

#include "../Utilities/utilities.h"
#include "Vertex.h"
#include "Texture.h"

#include <vector>
#include <tuple>
#include <string>


class Shaders;

class Mesh
{

	std::vector<Vertex> vertices;
	std::vector<std::tuple<GLuint, GLuint, GLuint>> indices;
	std::vector<std::pair<GLuint, GLuint>> lines;
	GLuint vbo, ibo;

	Matrix model;

	Texture texture;

	GLboolean wireframe;

public:
	Mesh(std::string fileName, Texture &texture);
	~Mesh();

private:
	std::vector<std::string> split(std::string &string, char delimiter);
	bool endsWith(std::string &string, std::string &ending);
	void parseNfgFile(std::string fileName);

public:
	void setWireframe(GLboolean wireframe);

	void draw(Shaders &shaders, Matrix &view, Matrix &projection);

};
