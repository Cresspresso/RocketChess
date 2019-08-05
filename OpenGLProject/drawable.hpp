/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	drawable.hpp
**
**	Summary		:	Something that can be drawn by a Renderer.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

struct IDrawable
{
	virtual ~IDrawable() = default;

	virtual ReturnCode draw() = 0;
};
