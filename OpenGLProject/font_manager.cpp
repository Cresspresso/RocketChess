/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	font_manager.cpp
**
**	Summary		:	Collection of fonts to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "font_manager.hpp"

void FontManager::load(Resource& out, size_t i)
{
	out = loadFont(names[i], sizes[i]);
}

void FontManager::destroyResource(Resource& resource) noexcept
{
	resource.destroy();
}
