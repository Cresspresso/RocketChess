/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mvp_material.cpp
**
**	Summary 	:	Basic material with a Model-View-Projection matrix.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "mvp_material.hpp"

void MvpMaterial::prepare()
{
	mvp = g_cameraVPMatrix * g_modelMatrix;
}

void MvpMaterial::apply(GLuint program)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
}

void MvpMaterial::unapply(GLuint program)
{
}
