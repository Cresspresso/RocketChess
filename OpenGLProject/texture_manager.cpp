/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	texture_manager.cpp
**
**	Summary		:	Collection of textures to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "texture_manager.hpp"

ReturnCode TextureManager::load(Resource& out, size_t i)
{
	return loadTexture(&out, names[i]);
}

void TextureManager::destroyResource(Resource& resource) noexcept
{
	destroyTexture(resource);
}
