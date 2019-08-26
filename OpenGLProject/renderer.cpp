/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	renderer.cpp
**
**	Summary		:	Renders a mesh with a shader program and a material.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

#include "renderer.hpp"

ReturnCode Renderer::render()
{
	g_modelMatrix = modelMatrix;
	g_mvp = g_cameraVPMatrix * g_modelMatrix;

	// TODO better error handling
	ASSERT1_RE(material, "material is null");
	HANDLE_ALL(material->prepare());
	
	ASSERT1_RE(mesh, "mesh is null");
	ASSERT1_RE(program, "program is null");

	glUseProgram(program);
	HANDLE_ALL(material->apply(program));
	HANDLE_ALL(mesh->draw());
	HANDLE_ALL(material->unapply(program));
	glUseProgram(0);

	return RC_SUCCESS;
}
