/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	phong_material.cpp
**
**	Summary 	:	Material for Full Phong lighting model.
**					Works with programs:
**						phong
**						blinn_phong
**						gouraud
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "nvToolsExt.h"

#include "phong_material.hpp"

void PhongMaterial::prepare()
{
	Super::prepare();

	eye = g_cameraEye;
	lightPos = g_lightPos;
}

void PhongMaterial::apply(GLuint program)
{
	nvtxRangePush(__FUNCTIONW__);

	Super::apply(program);

	glUniform3f(glGetUniformLocation(program, "eye"), eye.x, eye.y, eye.z);

	glUniform3f(glGetUniformLocation(program, "ambientColor"), ambientColor.x, ambientColor.y, ambientColor.z);
	glUniform3f(glGetUniformLocation(program, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(program, "lightSpecularStrength"), lightSpecularStrength);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glUniform1i(glGetUniformLocation(program, "tex0"), 1);

	glUniform1f(glGetUniformLocation(program, "shininess"), shininess);

	nvtxRangePop();
}

void PhongMaterial::unapply(GLuint program)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
