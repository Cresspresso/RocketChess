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

#include <cress/moo/final_act.hpp>

#include "globals.hpp"

#include "renderer.hpp"

void Renderer::render()
{
	g_modelMatrix = modelMatrix;
	g_mvp = g_cameraVPMatrix * g_modelMatrix;

	assert(material);
	if (!material) { throw std::runtime_error("material is null"); }
	material->prepare();

	assert(program);
	if (!program) { throw std::runtime_error("program is null"); }
	glUseProgram(program);
	CRESS_MOO_FINAL_ACT_SINGLE(pfa, glUseProgram(0));

	material->apply(program);
	CRESS_MOO_FINAL_ACT_SINGLE(mafa, material->unapply(program));

	assert(mesh);
	if (!mesh) { throw std::runtime_error("mesh is null"); }
	mesh->draw();
}
