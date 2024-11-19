#include <stdafx.h>
#include "Shaders.h"


Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);
	if (vertexShader == 0)
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);
	if (fragmentShader == 0)
	{
		glDeleteShader(vertexShader);
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_pos");
	localPositionAttribute = glGetAttribLocation(program, "a_posL");
	colorAttribute = glGetAttribLocation(program, "a_color");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	uvBlendAttribute = glGetAttribLocation(program, "a_uvBlend");
	modelViewProjectionMatrixUniform = glGetUniformLocation(program, "u_mvp");
	worldMatrixUniform = glGetUniformLocation(program, "u_world");
	cameraPositionUniform = glGetUniformLocation(program, "u_camPos");
	timeUniform = glGetUniformLocation(program, "u_time");
	dispMaxUniform = glGetUniformLocation(program, "u_dispMax");
	//textureUniform = glGetUniformLocation(program, "u_texture");
	textureUniform[0] = glGetUniformLocation(program, "u_texture0");
	textureUniform[1] = glGetUniformLocation(program, "u_texture1");
	textureUniform[2] = glGetUniformLocation(program, "u_texture2");
	textureUniform[3] = glGetUniformLocation(program, "u_texture3");
	textureUniform[4] = glGetUniformLocation(program, "u_texture4");
	textureUniform[5] = glGetUniformLocation(program, "u_texture5");
	textureUniform[6] = glGetUniformLocation(program, "u_texture6");
	return 0;
}
