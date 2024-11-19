#include "Camera.h"
#include "SceneObject.h"
#include"Terrain.h"
#include "ResourceManager.h"
//#include <vector>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

class SceneManager
{
	static SceneManager* smInstance;

	map<int, SceneObject*> obj;

	SceneManager() {}
public:
	map<int, Camera*> cam;
	Vector3 backgroundColor, fogColor, ambLight;
	float prevTime, bigRay, smallRay;
	float ratioAmbLight;
	int activeCamera;
	bool wireframe = false;
	void Init(string path);
	static SceneManager* getInstance();
	void Draw();
	void Update(GLfloat deltaTime);
	void switchWired();
};