/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	button_renderer.hpp
**
**	Summary		:	Renders a button with text and background image.
**
**	Project		:	Rocket Chess
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	26/08/2019
*/

#pragma once

#include <functional>
#include <vector>

#include "text_entity.hpp"
#include "sprite_entity.hpp"



struct ButtonEntity
{
	Transform transform;
	SpriteEntity backgroundEntity;
	TextEntity textEntity;

	ButtonEntity();

	// recalculates all transform matrices
	void recalculate();

	// calls recalculate
	ReturnCode render();
};
