#pragma once
#include "../Utilities/rapidxml/rapidxml.hpp"
#include "ModelResource.h"
#include "Model.h"
#include "TextureResource.h"
#include "Texture.h"
#include "ShaderResource.h"
#include "Shaders.h"
#include <map>

class ResourceManager
{
private:
	static ResourceManager* rmInstance;
	std::map<int, ModelResource*> mapMR;

	std::map<int, ShaderResource*> mapSR;

	ResourceManager() {}
public:
	std::map<int, TextureResource*> mapTR;
	std::map<int, Model*> mapM;
	std::map<int, Texture*> mapT;
	std::map<int, Shaders*> mapS;

	static ResourceManager* getInstance();
	void Init(string path);
	Model* loadModel(int id);
	Texture* loadTexture(int id);
	Shaders* loadShader(int id);
};