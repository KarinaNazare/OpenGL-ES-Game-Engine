#include "stdafx.h"
#include "Texture.h"

Texture::Texture(TextureResource* texr)
{
	tr = new TextureResource;
	tr = texr;
}


void Texture::Load()
{
	pixels = LoadTGA(tr->path.c_str(), &width, &height, &bpp);

	glGenTextures(1, &id);
	glBindTexture(tr->type, id);

	GLuint format;
	format = (bpp == 24) ? GL_RGB : GL_RGBA;

	if (tr->type == GL_TEXTURE_2D)
	{
		glTexImage2D(tr->type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
	}
	else if (tr->type == GL_TEXTURE_CUBE_MAP)
	{
		char *subimg = new char[(width / 4) * (bpp / 8) * (height / 3)];
		int it = 0;
		int offsetW = 2 * width / 4 * bpp / 8;
		int offsetH = height /  3 ;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		it = 0;
		offsetW = 0 * width / 4 * bpp / 8;
		offsetH = height / 3 ;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		it = 0;
		offsetW = 1 * width / 4 * bpp / 8;
		offsetH = 0 * height / 3 ;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		it = 0;
		offsetW = 1 * width / 4 * bpp / 8;
		offsetH = 2 * height / 3;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		it = 0;
		offsetW = 1 * width / 4 * bpp / 8;
		offsetH = 1 * height / 3 ;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		it = 0;
		offsetW = 3 * width / 4 * bpp / 8;
		offsetH = 1 * height / 3;
		for (int j = offsetH; j < offsetH + height / 3; j++)
			for (int i = 0; i < (bpp / 8) * width / 4; i++)
				subimg[it++] = pixels[j * width * bpp / 8 + offsetW + i];
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, width / 4, height / 3, 0, format, GL_UNSIGNED_BYTE, subimg);

		int err = glGetError();
	}
	glTexParameteri(tr->type, GL_TEXTURE_MIN_FILTER, tr->min_filter);
	glTexParameteri(tr->type, GL_TEXTURE_MAG_FILTER, tr->mag_filter);
	glTexParameteri(tr->type, GL_TEXTURE_WRAP_S, tr->wrap_s);
	glTexParameteri(tr->type, GL_TEXTURE_WRAP_T, tr->wrap_t);

	glBindTexture(tr->type, 0);

}
/*
Texture::Texture(std::string fileName)
{
	//parse TGA file to obtain the texture image data
	pixels = LoadTGA(fileName.c_str(), &width, &height, &bpp);

	//generate a texture id
	glGenTextures(1, &id);

	//setup the texture
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
}
*/

Texture::~Texture()
{
}


GLuint Texture::getId()
{
	return id;
}
