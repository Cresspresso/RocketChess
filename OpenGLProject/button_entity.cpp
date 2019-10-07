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

	backgroundEntity.transform.parentModelMatrix = parent;
	textEntity.transform.parentModelMatrix = parent;


	backgroundEntity.transform.localScale = vec3(275, 80, 1);
	textEntity.transform.localPosition = vec3(-backgroundEntity.transform.localScale.x / 2 + 10, 0, 0);

	backgroundEntity.setTexture(TextureIndexer::Button);
}



void ButtonEntity::recalculate()
{
	transform.recalculate();
	backgroundEntity.recalculate();
	textEntity.recalculate();
}



ReturnCode ButtonEntity::render()
{
	recalculate();

	BEGIN_ANYALL();
	DO_ANYALL(backgroundEntity.render());
	DO_ANYALL(textEntity.render());
	return END_ANYALL();
}
