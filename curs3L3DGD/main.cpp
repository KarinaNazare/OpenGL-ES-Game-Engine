// main.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Mesh.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"
#include "Globals.h"
#include"ResourceManager.h"
#include"SceneManager.h"
#include <cmath>
#include <vector>

//std::vector<Mesh> meshes;
//Camera camera;

//Matrix projection;


int activeCamera;
Camera* Cam;

Shaders shaders;

GLboolean wireframe = false;

GLfloat time = 0.0f;
GLfloat limit = 0.5f;


int Init(ESContext *esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	string rmPath = "../Resources/resourceManager.xml";
	ResourceManager::getInstance()->Init(rmPath);

	string smPath = "../Resources/sceneManager.xml";
	SceneManager::getInstance()->Init(smPath);

	activeCamera = SceneManager::getInstance()->activeCamera;
	Cam = SceneManager::getInstance()->cam[activeCamera];

	return 0;

	//meshes.push_back(Mesh("../Resources/Models/Croco.nfg", Texture("../Resources/Textures/Croco.tga")));
	//camera = Camera(Vector3(0, 0, -50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	//projection.SetPerspective(camera.getFOV(), (GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight, camera.getNear(), camera.getFar());

	////creation of shaders and program
	//return shaders.Init("../Resources/Shaders/VertexShader.vs", "../Resources/Shaders/FragmentShader.fs");
}


void Draw(ESContext *esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//for (int i = 0; i < meshes.size(); i++)
	//	meshes[i].draw(shaders, camera.getViewMatrix(), projection);

	SceneManager::getInstance()->Draw();

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}


void Update(ESContext *esContext, GLfloat deltaTime)
{
	//limit updates
	time += deltaTime;
	if (time < limit)
		return;
	else
		time = 0.0f;

	Cam->setDeltaTime(deltaTime);
	SceneManager::getInstance()->Update(deltaTime);
}


void Key(ESContext *esContext, unsigned char key, bool isPressed)
{
	float moveSpeed = 20.0f;
	float rotateSpeed = 2.0f;

	if (isPressed)
		switch (key)
		{
			//tasta X
			case 88:
				PostMessage(esContext->hWnd, WM_CLOSE, 0, 0);
				break;

			//tasta F
			case 70:
				////toggle wireframe mode
				//wireframe = !wireframe;
				//for (int i = 0; i < meshes.size(); i++)
				//	meshes[i].setWireframe(wireframe);
				//break;

			//tasta W
			case 87:
				Cam->moveOz(-moveSpeed);
				break;

			//tasta S
			case 83:
				Cam->moveOz(moveSpeed);
				break;

			//tasta A
			case 65:
				Cam->moveOx(-moveSpeed);
				break;

			//tasta D
			case 68:
				Cam->moveOx(moveSpeed);
				break;

			//tasta Q
			case 81:
				Cam->moveOy(-moveSpeed);
				break;

			//tasta E
			case 69: 
				Cam->moveOy(moveSpeed);
				break;

			//tasta I
			case 73:
				Cam->rotateOx(rotateSpeed);
				break;

			//tasta K
			case 75:
				Cam->rotateOx(-rotateSpeed);
				break;

			//tasta J
			case 74:
				Cam->rotateOy(rotateSpeed);
				break;

			//tasta L
			case 76:
				Cam->rotateOy(-rotateSpeed);
				break;

			//tasta U
			case 85:
				Cam->rotateOz(-rotateSpeed);
				break;

			//tasta O
			case 79:
				Cam->rotateOz(rotateSpeed);
				break;
		}
}


void CleanUp()
{
}


int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);
	esCreateWindow(&esContext, "MainWindow", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);

	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	return 0;
}
