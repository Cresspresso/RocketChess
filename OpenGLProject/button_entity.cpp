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



ButtonEntity::ButtonEntity() : transform(std::make_shared<Transform>())
{
	backgroundEntity.transform.parent = transform;
	backgroundEntity.transform.localScale = vec3(275, 80, 1);

	textEntity.transform.parent = transform;
	textEntity.transform.localPosition = vec3(-backgroundEntity.transform.localScale.x / 2 + 10, 0, 0);

	backgroundEntity.setTexture(TextureIndexer::Button);
}



void ButtonEntity::recalculate()
{
	transform->recalculate();
	backgroundEntity.recalculate();
	textEntity.recalculate();
}



void ButtonEntity::render()
{
	recalculate();

	try { backgroundEntity.render(); }
	catch (...) { printException(); }

	try { textEntity.render(); }
	catch (...) { printException(); }
}
