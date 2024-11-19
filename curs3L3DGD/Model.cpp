#include "stdafx.h"
#include "Model.h"
#include "Shaders.h"

#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Model::Model(string p)
{
	mr = new ModelResource;
	mr->path = p;
}

Model::Model() {}

bool Model::Load()
{
	//parse NFG file to obtain the mesh data
	parseNfgFile(mr->path);

	//initialize VBO
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//initialize IBO
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(tuple<GLushort, GLushort, GLushort>), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

vector<string> Model::split(string &stg, char delimiter)
{
	stringstream ss(stg);
	string token;
	vector<std::string> tokens;

	while (getline(ss, token, delimiter))
		if (!token.empty())
			tokens.push_back(token);

	return tokens;
}

bool Model::endsWith(std::string &str, std::string &ending)
{
	if (str.length() < ending.length())
		return false;

	return str.compare(str.length() - ending.length(), ending.length(), ending) == 0;
}

void Model::parseNfgFile(std::string fileName)
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
		Vector3 max, min;
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
				if (pos.x > max.x) max.x = pos.x;
				if (pos.y > max.y) max.y = pos.y;
				if (pos.z > max.z) max.z = pos.z;
				if (pos.x < min.x) min.x = pos.x;
				if (pos.y < min.y) min.y = pos.y;
				if (pos.z < min.z) min.z = pos.z;
			}
			else if (endsWith(informationTokens[0], std::string("uv")))
			{
				uv.x = std::stof(valueTokens[0]);
				uv.y = std::stof(valueTokens[1]);
			}
		}
		boxMax = max;
		boxMin = min;
		vertices.push_back(Vertex(pos, Vector3(1.0f, 1.0f, 1.0f), uv));
	}

	//pentru skybox avem coordonate locale
	//https://en.wikipedia.org/wiki/Cube_mapping#Skyboxes

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