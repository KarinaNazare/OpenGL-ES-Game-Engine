#include"stdafx.h"
#include "Skybox.h"
#include"Vertex.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Globals.h"

using namespace std;

void Skybox::Draw()
{
	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if (blend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);

	glUseProgram(shader->program);

	Matrix  View, Projection, MVP;

	int activeCamera = SceneManager::getInstance()->activeCamera;
	Camera* cam = SceneManager::getInstance()->cam[activeCamera];

	View = cam->getViewMatrix();
	Projection.SetPerspective(cam->getFOV(), (GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight, cam->getNear(), cam->getFar());

	MVP = modelMat * View * Projection;

	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);

	vector<Texture*> texs;
	for (int i = 0;i < textures.size();i++) {
		texs.push_back(ResourceManager::getInstance()->loadTexture(textures[i]));
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texs[i]->getId());
	}


	//bind shader data

	if (shader->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}

	if (shader->colorAttribute != -1)
	{
		glEnableVertexAttribArray(shader->colorAttribute);
		glVertexAttribPointer(shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	}

	if (shader->uvAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(Vector3)));
	}

	if (shader->uvBlendAttribute != -1)
	{
		glEnableVertexAttribArray(shader->uvBlendAttribute);
		glVertexAttribPointer(shader->uvBlendAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(Vector3) + sizeof(Vector2)));
	}
	if (shader->localPositionAttribute != -1)
	{
		glEnableVertexAttribArray(shader->localPositionAttribute);
		glVertexAttribPointer(shader->localPositionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}

	if (shader->modelViewProjectionMatrixUniform != -1)
	{
		glUniformMatrix4fv(shader->modelViewProjectionMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	//vector de texture uniform

	for (int i = 0;i < textures.size();i++) {
		if (shader->textureUniform[i] != -1)
			glUniform1i(shader->textureUniform[i], i);
	}


	glDrawElements(GL_TRIANGLES, 3 * model->indices.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Skybox::Update(GLfloat deltaTime)
{

	int activeCamera = SceneManager::getInstance()->activeCamera;
	Camera* cam = SceneManager::getInstance()->cam[activeCamera];
	position.x = cam->position.x;
	position.y = offset;
	position.z = cam->position.z;

	Matrix rot;
	rot.SetRotationY(rotTime);
	rotTime = rotTime + 0.02;

	Matrix m1, m2, m3, m4, m5;
	m1.SetTranslation(position);
	m2.SetRotationX(rotation.x);
	m4.SetRotationZ(rotation.z);
	m5.SetScale(scale);

	modelMat = m5*m4*rot*m2*m1;
}