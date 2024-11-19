#include "stdafx.h"
#include "ResourceManager.h"
#include "../Utilities/rapidxml/rapidxml.hpp"
#include <sstream>
#include <fstream>

ResourceManager* ResourceManager::rmInstance = NULL;

ResourceManager* ResourceManager::getInstance()
{
	if (!rmInstance)
		rmInstance = new ResourceManager();
	return rmInstance;
}

void ResourceManager::Init(string path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		cout << "Unable to open scene Manager file";
	}

	rapidxml::xml_document<> doc;
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	doc.parse<0>(&content[0]);

	rapidxml::xml_node<> *pRoot = doc.first_node();

	rapidxml::xml_node<> *pResourceNode = pRoot->first_node("models");
	for (rapidxml::xml_node<> *pNode = pResourceNode->first_node("model"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> *attr = pNode->first_attribute("id");
		ModelResource* mr = new ModelResource;
		mr->path = pNode->first_node("path")->value();
		mapMR[atoi(attr->value())] = mr;
	}

	pResourceNode = pRoot->first_node("shaders");
	for (rapidxml::xml_node<> *pNode = pResourceNode->first_node("shader"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> *attr = pNode->first_attribute();
		ShaderResource* sr = new ShaderResource;
		sr->vs = pNode->first_node("vs")->value();
		sr->fs = pNode->first_node("fs")->value();
		mapSR[atoi(attr->value())] = sr;
	}

	pResourceNode = pRoot->first_node("textures");
	for (rapidxml::xml_node<> *pNode = pResourceNode->first_node("texture"); pNode; pNode = pNode->next_sibling())
	{
		rapidxml::xml_attribute<> *id = pNode->first_attribute("id");
		rapidxml::xml_attribute<> *type = pNode->first_attribute("type");

		TextureResource* tr = new TextureResource;

		if (strcmp(type->value(), "2d") == 0)
			tr->type = GL_TEXTURE_2D;
		else
			if (strcmp(type->value(), "cube") == 0)
				tr->type = GL_TEXTURE_CUBE_MAP;

		tr->path = pNode->first_node("path")->value();
		if (strcmp(pNode->first_node("min_filter")->value(), "LINEAR") == 0)
			tr->min_filter = GL_LINEAR;

		if (strcmp(pNode->first_node("mag_filter")->value(), "LINEAR") == 0)
			tr->mag_filter = GL_LINEAR;

		if (strcmp(pNode->first_node("wrap_s")->value(), "CLAMP_TO_EDGE") == 0)
			tr->wrap_s = GL_CLAMP_TO_EDGE;
		else if (strcmp(pNode->first_node("wrap_s")->value(), "GL_REPEAT") == 0)
			tr->wrap_s = GL_REPEAT;


		if (strcmp(pNode->first_node("wrap_t")->value(), "CLAMP_TO_EDGE") == 0)
			tr->wrap_t = GL_CLAMP_TO_EDGE;
		else if (strcmp(pNode->first_node("wrap_t")->value(), "GL_REPEAT") == 0)
			tr->wrap_t = GL_REPEAT;

		mapTR[atoi(id->value())] = tr;
	}
}

//Aceste functii vor fi apelate atunci cand un obiect din scena are nevoie de o resursa
Model* ResourceManager::loadModel(int id)
{
	map<int, Model*>::iterator it;
	it = mapM.find(id);
	if (it != mapM.end()) //found
		return mapM[id];
	else
	{
		ModelResource* mr = mapMR[id];

		//se creeaza resursa
		Model* model = new Model(mr->path);

		//se apeleaza load pentru ea
		if (!model->Load())
			fprintf(stderr, "Model loading error\n");

		//se adauga in vector
		mapM[id] = model;

		//e returnata o referinta pentru ea
		return mapM[id];
	}
}

Texture* ResourceManager::loadTexture(int id)
{
	map<int, Texture*>::iterator it;
	it = mapT.find(id);
	if (it != mapT.end()) //found
		return mapT[id];
	else
	{
		TextureResource* tr = mapTR[id];

		//se creeaza resursa
		Texture* tex = new Texture(tr);

		//se apeleaza load pentru ea
		tex->Load();

		//se adauga in vector
		mapT[id] = tex;

		//e returnata o referinta pentru ea
		return mapT[id];
	}
}

Shaders* ResourceManager::loadShader(int id)
{
	map<int, Shaders*>::iterator it;
	it = mapS.find(id);
	if (it != mapS.end()) //found
		return mapS[id];
	else
	{
		ShaderResource* sr = mapSR[id];

		//se creeaza resursa
		Shaders* s = new Shaders;

		//se apeleaza load pentru ea
		char *vs = new char[sr->vs.length() + 1];
		strcpy(vs, sr->vs.c_str());

		char *fs = new char[sr->fs.length() + 1];
		strcpy(fs, sr->fs.c_str());

		if (s->Init(vs, fs) != 0)
			fprintf(stderr, "Shader loading error\n");

		//se adauga in vector
		mapS[id] = s;

		delete[] vs;
		delete[] fs;

		//e returnata o referinta pentru ea
		return mapS[id];
	}
}