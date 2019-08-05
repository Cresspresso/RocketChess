/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_button.hpp
**
**	Summary		:	Button that scales when hovered.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "button.hpp"

struct MenuButtonEntity
{
	Button button;
	vec3 scaleNormal = vec3(1);
	vec3 scaleHovered = vec3(1.2f);

	ReturnCode update();
	ReturnCode render();
};
