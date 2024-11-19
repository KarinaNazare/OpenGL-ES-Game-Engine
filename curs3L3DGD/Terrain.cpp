#include"stdafx.h"
#include "Terrain.h"
#include"Vertex.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Globals.h"

using namespace std;

void Terrain::Draw()
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

	Matrix View, Projection, MVP;

	int activeCamera = SceneManager::getInstance()->activeCamera;
	Camera* cam = SceneManager::getInstance()->cam[activeCamera];

	View = cam->getViewMatrix();
	Projection.SetPerspective(cam->getFOV(), (GLfloat)Globals::screenWidth / (GLfloat)Globals::screenHeight, cam->getNear(), cam->getFar());

	MVP = modelMat * View * Projection;

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);

	vector<Texture*> texs;
	for (int i = 0;i < textures.size();i++) {
		texs.push_back(ResourceManager::getInstance()->loadTexture(textures[i]));
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texs[i]->getId());
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

	if (shader->modelViewProjectionMatrixUniform != -1)
	{
		glUniformMatrix4fv(shader->modelViewProjectionMatrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	//vector de texture uniform

	for (int i = 0;i < textures.size();i++) {
		if (shader->textureUniform[i] != -1)
			glUniform1i(shader->textureUniform[i], i);
	}



	//draw mesh
	if (!SceneManager::getInstance()->wireframe)
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);
	else
		glDrawElements(GL_LINES, 2 * lines.size(), GL_UNSIGNED_SHORT, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::Generate() {
	
	int activeCamera = SceneManager::getInstance()->activeCamera;
	Camera* cam = SceneManager::getInstance()->cam[activeCamera];
	T.SetTranslation(Vector3(cam->position.x, offsetY, cam->position.z));
	center = cam->position; center.y = offsetY;
	for (GLushort i = 0;i <= nr_cell;i++)
		for (GLushort j = 0;j <= nr_cell;j++) {
			Vector3 pos;
			pos.z = (float)dim*i-nr_cell / 2 * dim;
			pos.x = (float)dim*j - nr_cell / 2 * dim;
			pos.y = 0;

			GLfloat x = (GLfloat) i / nr_cell;
			GLfloat y = (GLfloat) j / nr_cell;
			Vector2 uv = Vector2(x,y);
			vertices.push_back(Vertex(pos, Vector3(0,1,0), Vector2(j, i), uv));
		
			
		}

	for (GLushort i = 0;i < nr_cell;i++)
		for (GLushort j = 0;j < nr_cell;j++) {
			indices.push_back(j + i*(nr_cell + 1));
			indices.push_back(j + i*(nr_cell + 1) + 1);
			indices.push_back(j + (nr_cell + 1)*(i + 1));
		
			indices.push_back(j + i*(nr_cell + 1) + 1);
			indices.push_back(j + (nr_cell + 1)*(i + 1) + 1);
			indices.push_back(j + (nr_cell + 1)*(i + 1));
		}

	//initialize VBO
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//initialize IBO
	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Update(GLfloat deltaTime) {

	int activeCamera = SceneManager::getInstance()->activeCamera;
	Camera* cam = SceneManager::getInstance()->cam[activeCamera];

	GLfloat x = cam->position.x, z = cam->position.z;
	GLfloat ii = (float)1 / nr_cell;

	if (z >= center.z + dim) { //forward (inainte in scena) //ok

		T.SetTranslation(Vector3(center.x, center.y, center.z + dim));
		center.z += dim;

		for (GLushort k = 0; k < (nr_cell + 1)*(nr_cell + 1); k++) {
			vertices[k].uvBlend.x += ii;
		}
	}


	///; stanga-dreapta - v
	// fata spate u
	//fata e minus
	if (x >= center.x + dim) { //stanga //ok

		T.SetTranslation(Vector3(center.x + dim, center.y, center.z));
		center.x += dim;

		for (GLushort k = 0; k < (nr_cell + 1)*(nr_cell + 1); k++) {
			vertices[k].uvBlend.y += ii;
		}
	}

	if (z <= center.z - dim) { //inapoi

		T.SetTranslation(Vector3(center.x, center.y, center.z - dim));
		center.z -= dim;

		for (GLushort k = 0; k < (nr_cell + 1)*(nr_cell + 1); k++) {
			vertices[k].uvBlend.x -= ii;
		}
	}

	if (x <= center.x - dim) { //dreapta

		T.SetTranslation(Vector3(center.x - dim, center.y, center.z));
		center.x -= dim;

		for (GLushort k = 0; k < (nr_cell + 1)*(nr_cell + 1); k++) {
			vertices[k].uvBlend.y -= ii;
		}
	}

	modelMat = T;

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

}