/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	screen.hpp
**
**	Summary		:	GLUT Window information.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

namespace screen
{
	int getWidth() noexcept;
	int getHeight() noexcept;
	float getAspect() noexcept;
	ivec2 getSize() noexcept;
	vec2 getHalfSizeF() noexcept;

	void update() noexcept;
}
