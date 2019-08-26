/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	button_renderer.cpp
**
**	Summary		:	Renders a button with text and background image.
**
**	Project		:	Rocket Chess
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	26/08/2019
*/

#include "resource_warehouse.hpp"
#include "singleton.hpp"

#include "button_entity.hpp"



ButtonEntity::ButtonEntity()
{
	auto* parent = &transform.modelMatrix;

	backgroundTransform.parentModelMatrix = parent;
	textEntity.transform.parentModelMatrix = parent;


	backgroundTransform.localScale = vec3(200, 80, 1);
	textEntity.transform.localPosition = vec3(-backgroundTransform.localScale.x / 2 + 10, 0, 0);
	textEntity.material.tint = vec3(0);


	auto& resources = singleton::getResources();

	matBG.tex1 = resources.textures[TextureIndexer::Button];

	background.material = &matBG;
	background.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	background.mesh = &(resources.meshes[MeshIndexer::Quad]);
}



void ButtonEntity::recalculate()
{
	transform.recalculate();
	backgroundTransform.recalculate();
	textEntity.recalculate();
}



ReturnCode ButtonEntity::render()
{
	transform.recalculate();
	backgroundTransform.recalculate();

	BEGIN_ANYALL();

	background.modelMatrix = backgroundTransform.modelMatrix;
	DO_ANYALL(background.render());

	DO_ANYALL(textEntity.render());

	return END_ANYALL();
}
