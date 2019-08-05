/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cubemap_material.cpp
**
**	Summary 	:	Material for skybox/cubemap.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "cubemap_material.hpp"



ReturnCode SkyboxMaterial::prepare()
{
	mvp = g_cameraVPMatrix * g_modelMatrix;

	return RC_SUCCESS;
}

ReturnCode SkyboxMaterial::apply(GLuint program)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap); // IMPORTANT
	glUniform1i(glGetUniformLocation(program, "cubemap"), 1);

	return RC_SUCCESS;
}

ReturnCode SkyboxMaterial::unapply(GLuint program)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	return RC_SUCCESS;
}
