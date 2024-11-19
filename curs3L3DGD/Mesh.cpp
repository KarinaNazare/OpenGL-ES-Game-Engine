#include <stdafx.h>
#include "Mesh.h"
#include "Shaders.h"

#include <fstream>
#include <sstream>
#include <algorithm>


Mesh::Mesh(std::string fileName, Texture &texture) : texture(texture), wireframe(false)
{
	//parse NFG file to obtain the mesh data
	parseNfgFile(fileName);

	//initialize VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//initialize IBO
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::tuple<GLuint, GLuint, GLuint>), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//initialize model matrix
	model.SetScale(Vector3(0.15f, 0.15f, 0.15f));
}


Mesh::~Mesh()
{
}


std::vector<std::string> Mesh::split(std::string &string, char delimiter)
{
	std::stringstream ss(string);
	std::string token;
	std::vector<std::string> tokens;

	while (std::getline(ss, token, delimiter))
		if (!token.empty())
			tokens.push_back(token);

	return tokens;
}


bool Mesh::endsWith(std::string &string, std::string &ending)
{
	if (string.length() < ending.length())
		return false;

	return string.compare(string.length() - ending.length(), ending.length(), ending) == 0;
}


void Mesh::parseNfgFile(std::string fileName)
{
	std::ifstream in(fileName);
	std::string line;
	std::vector<std::string> tokens;
	int nrOfVerties, nrOfIndices;

	//get number of vertices
	std::getline(in, line);
	tokens = split(line, ' ');
	nrOfVerties = std::stoi(tokens[1]);

	//parse vertices
	for (int i = 0; i < nrOfVerties; i++)
	{
		Vector3 pos;
		Vector2 uv;

		std::getline(in, line);
		tokens = split(line, ';');

		for (int j = 0; j < tokens.size(); j++)
		{
			std::vector<std::string> informationTokens = split(tokens[j], ':');
			std::vector<std::string> valueTokens = split(informationTokens[1], ',');

			//cleanup the information values
			for (int k = 0; k < valueTokens.size(); k++)
			{
				valueTokens[k].erase(std::remove(valueTokens[k].begin(), valueTokens[k].end(), '['), valueTokens[k].end());
				valueTokens[k].erase(std::remove(valueTokens[k].begin(), valueTokens[k].end(), ' '), valueTokens[k].end());
				valueTokens[k].erase(std::remove(valueTokens[k].begin(), valueTokens[k].end(), ']'), valueTokens[k].end());
			}

			//extract the information we need about the current vertex
			if (endsWith(informationTokens[0], std::string("pos")))
			{
				pos.x = std::stof(valueTokens[0]);
				pos.y = std::stof(valueTokens[1]);
				pos.z = std::stof(valueTokens[2]);
			}
			else if (endsWith(informationTokens[0], std::string("uv")))
			{
				uv.x = std::stof(valueTokens[0]);
				uv.y = std::stof(valueTokens[1]);
			}
		}

		vertices.push_back(Vertex(pos, Vector3(1.0f, 1.0f, 1.0f), uv));
	}

	//get number of indices
	std::getline(in, line);
	tokens = split(line, ' ');
	nrOfIndices = std::stoi(tokens[1]) / 3;

	//parse indices
	for (int i = 0; i < nrOfIndices; i++)
	{
		std::getline(in, line);
		tokens = split(line, ' ');

		for (int j = 0; j < tokens.size(); j++)
			tokens[j].erase(std::remove(tokens[j].begin(), tokens[j].end(), ','), tokens[j].end());

		//first token is the line number, while the following ones are the indices
		GLuint index1 = std::stoul(tokens[1]);
		GLuint index2 = std::stoul(tokens[2]);
		GLuint index3 = std::stoul(tokens[3]);

		indices.push_back(std::make_tuple(index1, index2, index3));
		lines.push_back(std::make_pair(index1, index2));
		lines.push_back(std::make_pair(index2, index3));
		lines.push_back(std::make_pair(index3, index1));
	}
}


void Mesh::setWireframe(GLboolean wireframe)
{
	if (!this->wireframe && wireframe)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, lines.size() * sizeof(std::pair<GLuint, GLuint>), &lines[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else if (this->wireframe && !wireframe)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::tuple<GLuint, GLuint, GLuint>), &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	this->wireframe = wireframe;
}


void Mesh::draw(Shaders &shaders, Matrix &view, Matrix &projection)
{
	glUseProgram(shaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glActiveTexture(GL_TEXTURE0 + texture.getId());
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	//bind shader data
	if (shaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(shaders.positionAttribute);
		glVertexAttribPointer(shaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}

	if (shaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(shaders.colorAttribute);
		glVertexAttribPointer(shaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	}

	if (shaders.uvAttribute != -1)
	{
		glEnableVertexAttribArray(shaders.uvAttribute);
		glVertexAttribPointer(shaders.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(Vector3)));
	}

	if (shaders.modelViewProjectionMatrixUniform != -1)
	{
		Matrix mvp = model * view * projection;
		glUniformMatrix4fv(shaders.modelViewProjectionMatrixUniform, 1, GL_FALSE, (GLfloat*)mvp.m);
	}

	if (shaders.textureUniform[0] != -1)
		glUniform1i(shaders.textureUniform[0], texture.getId());

	//draw mesh
	if (!wireframe)
		glDrawElements(GL_TRIANGLES, 3 * indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	else
		glDrawElements(GL_LINES, 2 * lines.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
