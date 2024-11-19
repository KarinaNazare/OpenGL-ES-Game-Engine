#pragma once

#include "../Utilities/utilities.h"

class Shaders 
{
public:
	GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint localPositionAttribute;
	GLint colorAttribute;
	GLint uvAttribute;
	GLint uvBlendAttribute;
	GLint worldMatrixUniform;
	GLint cameraPositionUniform;
	GLint modelViewProjectionMatrixUniform;
	GLint timeUniform;
	GLint dispMaxUniform;
	GLint textureUniform[7];

	~Shaders();

	int Init(char * fileVertexShader, char * fileFragmentShader);
};
