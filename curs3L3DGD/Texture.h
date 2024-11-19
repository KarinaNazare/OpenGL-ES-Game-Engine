#pragma once

#include "../Utilities/utilities.h"
#include "TextureResource.h"
#include <string>


class Texture
{
	TextureResource* tr;
	char *pixels;
	int width, height, bpp;
	
	GLuint id;

public:
	//Texture(std::string fileName);
	Texture();
	Texture(TextureResource* texr);
	~Texture();

	GLuint getId();

	void Load();

};
