/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	textures.hpp
**
**	Summary		:	Loads OpenGL Textures and Cubemaps.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <string>
#include <glew/glew.h>
#include <freeglut/freeglut.h>

GLuint loadTexture(std::string const& fileName);

GLuint loadCubeMap(
	std::string const& folderName,
	std::string const& extension = ".jpg",
	bool continueOnError = false // if true, will continue creating the cubemap regardless of unloaded faces
	);

void destroyTexture(GLuint& tex);
