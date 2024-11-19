#include "stdafx.h"
#include "../Utilities/rapidxml/rapidxml.hpp"
#include <fstream>
#include "SceneManager.h"
#include "Terrain.h"
#include "Fire.h"
#include "Skybox.h"


SceneManager* SceneManager::smInstance = NULL;

SceneManager* SceneManager::getInstance()
{
	if (!smInstance)
		smInstance = new SceneManager();
	return smInstance;
}

//parse XML
void SceneManager::Init(string path)
{
	prevTime = clock();
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

	rapidxml::xml_node<> *pResourceNode = pRoot->first_node("backgroundColor");

		backgroundColor = Vector3(atof(pResourceNode->first_node("r")->value()),
		atof(pResourceNode->first_node("g")->value()),
		atof(pResourceNode->first_node("b")->value()));

	pResourceNode = pRoot->first_node("fog");
	bigRay = atof(pResourceNode->first_node("bigRay")->value());
	smallRay = atof(pResourceNode->first_node("smallRay")->value());
	rapidxml::xml_node<> *prop = pResourceNode->first_node("color");
	fogColor = Vector3(atof(prop->first_node("r")->value()),
		atof(prop->first_node("g")->value()),
		atof(prop->first_node("b")->value()));

	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 0.0f);

	pResourceNode = pRoot->first_node("cameras");
	for (rapidxml::xml_node<> *pNode = pResourceNode->first_node("camera"); pNode; pNode = pNode->next_sibling())
	{
		Vector3 position, target, up;
		GLfloat translationSpeed, rotationSpeed, camNear, camFar, fov;

		//id
		rapidxml::xml_attribute<> *attr = pNode->first_attribute();
		int id = atoi(attr->value());

		rapidxml::xml_node<> *prop = pNode->first_node("position");
		position = Vector3(atoi(prop->first_node("x")->value()),
			atoi(prop->first_node("y")->value()),
			atoi(prop->first_node("z")->value()));

		prop = pNode->first_node("target");
		target = Vector3(atoi(prop->first_node("x")->value()),
			atoi(prop->first_node("y")->value()),
			atoi(prop->first_node("z")->value()));

		prop = pNode->first_node("up");
		up = Vector3(atoi(prop->first_node("x")->value()),
			atoi(prop->first_node("y")->value()),
			atoi(prop->first_node("z")->value()));

		prop = pNode->first_node("translationSpeed");
		translationSpeed = atof(prop->value());

		prop = pNode->first_node("rotationSpeed");
		rotationSpeed = atof(prop->value());

		prop = pNode->first_node("fov");
		fov = atoi(prop->value());

		prop = pNode->first_node("near");
		camNear = atof(prop->value());

		prop = pNode->first_node("far");
		camFar = atof(prop->value());

		Camera* camera = new Camera(position, target, up, fov, camNear, camFar);
		cam[id] = camera;
	}

	pResourceNode = pRoot->first_node("activeCamera");
	activeCamera = atoi(pResourceNode->value());

	pResourceNode = pRoot->first_node("objects");
	for (rapidxml::xml_node<> *pNode = pResourceNode->first_node("object"); pNode; pNode = pNode->next_sibling())
	{
		int idObj, idModel, idShader;
		string type, name;
		bool depthTest, blend, wired;
		vector<int> textures, followingCameras;
		Vector3 color, position, rotation, scale;
		float selfRotateSpeed;
		bool camox, camoy, camoz;
		Vector3 point;
		float speed;

		rapidxml::xml_attribute<> *attr = pNode->first_attribute();
		rapidxml::xml_node<> *prop;

		SceneObject* sceneObject;

		prop = pNode->first_node("model");
		if (strcmp(prop->value(), "generated") == 0)
		{
			prop = pNode->first_node("type");
			if (strcmp(prop->value(), "terrain") == 0)
			{
			Terrain * ter = new Terrain();
			ter->type = prop->value();

			prop = pNode->first_node("nr_cell");
			ter->nr_cell = atoi(prop->value());

			prop = pNode->first_node("dim_cell");
			ter->dim = atof(prop->value());

			prop = pNode->first_node("offsetY");
			ter->offsetY = atof(prop->value());

			prop = pNode->first_node("height");
			Vector3 h = Vector3(atof(prop->first_node("r")->value()),
			atof(prop->first_node("g")->value()),
			atof(prop->first_node("b")->value()));
			ter->height = h;

			ter->center = cam[activeCamera]->position;

			ter->Generate();
			sceneObject = ter;
			}
		}
		else // normal
		{
			int idModel = atoi(prop->value());
			prop = pNode->first_node("type");

			if (strcmp(prop->value(), "skybox") == 0)
			{
				Skybox * sb = new Skybox();

				sb->idModel = idModel;
				sb->type = prop->value();

				prop = pNode->first_node("offset");
				sb->offset = atof(prop->value());
				sceneObject = sb;
			}
			else
				if (strcmp(prop->value(), "fire") == 0)
				{
					Fire * f = new Fire();

					f->idModel = idModel;
					f->type = prop->value();

					prop = pNode->first_node("height");
					f->dispMax = atof(prop->value());
					
					sceneObject = f;
				}
				else
			{
				sceneObject = new SceneObject();

				sceneObject->idModel = idModel;
				sceneObject->type = prop->value();
			}

			prop = pNode->first_node("position");
			position = Vector3(atof(prop->first_node("x")->value()),
				atof(prop->first_node("y")->value()),
				atof(prop->first_node("z")->value()));
			sceneObject->position = position;

			prop = pNode->first_node("rotation");
			rotation = Vector3(atof(prop->first_node("x")->value()),
				atof(prop->first_node("y")->value()),
				atof(prop->first_node("z")->value()));
			sceneObject->rotation = rotation;

			prop = pNode->first_node("scale");
			scale = Vector3(atof(prop->first_node("x")->value()),
				atof(prop->first_node("y")->value()),
				atof(prop->first_node("z")->value()));
			sceneObject->scale = scale;

			sceneObject->model = ResourceManager::getInstance()->loadModel(sceneObject->idModel);
		}

		//common
		idObj = atoi(attr->value());

		prop = pNode->first_node("shader");
		sceneObject->idShader = atoi(prop->value());
		sceneObject->shader = ResourceManager::getInstance()->loadShader(sceneObject->idShader);

		prop = pNode->first_node("depthTest");
		if (prop != NULL)
		{
			if (strcmp(prop->value(), "true") == 0)
				sceneObject->depthTest = true;
			else
				sceneObject->depthTest = false;
		}

		prop = pNode->first_node("blend");
		if (prop != NULL)
		{
			if (strcmp(prop->value(), "true") == 0)
				sceneObject->blend = true;
			else
				sceneObject->blend = false;
		}

		prop = pNode->first_node("name");
		if (prop != NULL)
			sceneObject->name = prop->value();

		prop = pNode->first_node("textures");
		if (prop != NULL)
			for (rapidxml::xml_node<> *pNode1 = prop->first_node("texture"); pNode1; pNode1 = pNode1->next_sibling())
			{
				rapidxml::xml_attribute<> *attr = pNode1->first_attribute();
				sceneObject->textures.push_back(atoi(attr->value()));
			}

		prop = pNode->first_node("lights");
		//active lights
		if (prop != NULL)
			for (rapidxml::xml_node<> *pNode1 = prop->first_node("light"); pNode1; pNode1 = pNode1->next_sibling())
				sceneObject->lights.push_back(atoi(pNode1->value()));

		obj[idObj] = sceneObject;
	}

	//ambiental light
	pResourceNode = pRoot->first_node("ambientalLight");
	ratioAmbLight = atof(pResourceNode->first_node("ratio")->value());

	pResourceNode = pResourceNode->first_node("color");
	ambLight = Vector3(atof(pResourceNode->first_node("r")->value()),
		atof(pResourceNode->first_node("g")->value()),
		atof(pResourceNode->first_node("b")->value()));
	
	//Load models, shaders, textures
	for (map<int, SceneObject*>::iterator it = obj.begin(); it != obj.end(); ++it)
	{
		if (strcmp(it->second->type.c_str(), "normal") == 0 ||
			strcmp(it->second->type.c_str(), "fire") == 0 ||
			strcmp(it->second->type.c_str(), "skybox") == 0
			)
		{
			int idModel = it->second->idModel;
			ResourceManager::getInstance()->loadModel(idModel);
		}

		int idShader = it->second->idShader;
		vector<int> idTextures = it->second->textures;

		ResourceManager::getInstance()->loadShader(idShader);
		for (vector<int>::iterator it = idTextures.begin(); it != idTextures.end(); ++it)
			ResourceManager::getInstance()->loadTexture(*it);
	}

}

void SceneManager::Draw()
{
	for (map<int, SceneObject*>::iterator it = obj.begin(); it != obj.end(); ++it)
		it->second->Draw();
}

void SceneManager::Update(GLfloat deltaTime)
{
	for (map<int, SceneObject*>::iterator it = obj.begin(); it != obj.end(); ++it)
		it->second->Update(deltaTime);
}