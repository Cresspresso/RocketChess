/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	reflection_material.cpp
**
**	Summary 	:	Fully reflecive material.
**					Works with programs:
**						reflection
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "reflection_material.hpp"

ReturnCode ReflectionMaterial::prepare()
{
	ReturnCode const r = Super::prepare();

	eye = g_cameraEye;

	return r;
}

ReturnCode ReflectionMaterial::apply(GLuint program)
{
	ReturnCode const r = Super::apply(program);

	glUniform3f(glGetUniformLocation(program, "eye"), eye.x, eye.y, eye.z);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
	glUniform1i(glGetUniformLocation(program, "skybox"), 1);

	return r;
}

ReturnCode ReflectionMaterial::unapply(GLuint program)
{
	ReturnCode const r = Super::unapply(program);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	return r;
}
