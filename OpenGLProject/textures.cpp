/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	textures.cpp
**
**	Summary		:	Loads OpenGL Textures and Cubemaps.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <cassert>
#include <soil/SOIL.h>

#include "textures.hpp"

GLuint loadTexture(std::string const& fileName)
{
	static char const* const dir = "Resources/Textures/";
	std::string const filePath = dir + fileName;

	int width, height;
	unsigned char* const image = SOIL_load_image(filePath.c_str(), &width, &height, nullptr, SOIL_LOAD_RGBA);
	assert(image);
	if (!image) { throw std::runtime_error(std::string("SOIL_load_image failed: ") + SOIL_last_result()); }

	GLuint t;
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return t;
}



namespace
{
	void loadCubeMapFace(int index, char const* filePath)
	{
		int width, height;
		unsigned char* const image = SOIL_load_image(filePath, &width, &height, nullptr, SOIL_LOAD_RGBA);
		assert(image);
		if (!image) { throw std::runtime_error(std::string("SOIL_load_image failed: ") + SOIL_last_result()); }

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
	}
}

GLuint loadCubeMap(
	std::string const& folderName,
	std::string const& extension,
	bool continueOnError
)
{
	static char const* const dir = "Resources/CubeMaps/";
	std::string const folderPath = dir + folderName + '/';

	constexpr int const numFaces = 6;
	// Face file names must be in order.
	static char const* const fileNames[numFaces]{ "right", "left", "top", "bottom", "back", "front" };

	GLuint cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	int numLoaded = 0;
	for (int i = 0; i < numFaces; ++i)
	{
		std::string const filePath = folderPath + fileNames[i] + extension;
		try {
			loadCubeMapFace(i, filePath.c_str());
			++numLoaded;
		}
		catch (...) {
			if (!continueOnError) {
				glDeleteTextures(1, &cubemap);
				throw;
			}
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return cubemap;
	/*if (continueOnError && numLoaded != numFaces)
	{
		return RC_PARTIAL;
	}
	return RC_SUCCESS;*/
}



void destroyTexture(GLuint& tex)
{
	if (tex)
	{
		glDeleteTextures(1, &tex);
		tex = 0;
	}
}
