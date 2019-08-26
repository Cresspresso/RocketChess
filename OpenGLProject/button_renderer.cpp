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

#include "button_renderer.hpp"



ButtonRenderer::ButtonRenderer()
{
	auto* parent = &transform.modelMatrix;

	backgroundTransform.parentModelMatrix = parent;
	textTransform.parentModelMatrix = parent;



	auto& resources = singleton::getResources();

	matBG.tex1 = resources.textures[TextureIndexer::Button];

	background.material = &matBG;
	background.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	background.mesh = &(resources.meshes[MeshIndexer::Quad]);

	text.font = &(resources.fonts[FontIndexer::Arial]);
	text.renderer.material = &matText;
	text.renderer.program = resources.programs.getProgram(ProgramIndexer::Text);
	text.renderer.mesh = &(resources.meshes[MeshIndexer::Text]);
}



void ButtonRenderer::recalculate()
{
	transform.recalculate();
	backgroundTransform.recalculate();
	textTransform.recalculate();
}



ReturnCode ButtonRenderer::render()
{
	recalculate();

	BEGIN_ANYALL();

	background.modelMatrix = backgroundTransform.modelMatrix;
	DO_ANYALL(background.render());

	text.renderer.modelMatrix = textTransform.modelMatrix;
	DO_ANYALL(text.render());

	return END_ANYALL();
}
