/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	quad4_material.cpp
**
**	Summary 	:	Unlit material.
**					Works with programs:
**						quad4
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "quad4_material.hpp"

void Quad4Material::prepare()
{
	mvp = g_cameraVPMatrix * g_modelMatrix;
}

void Quad4Material::apply(GLuint program)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glUniform1i(glGetUniformLocation(program, "tex1"), 1);
}

void Quad4Material::unapply(GLuint program)
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
