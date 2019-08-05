/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cubemap_manager.cpp
**
**	Summary		:	Collection of cubemaps to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "cubemap_manager.hpp"

ReturnCode CubeMapManager::load(Resource& out, size_t i)
{
	return loadCubeMap(&out, names[i], ".jpg", true);
}

void CubeMapManager::destroyResource(Resource& resource) noexcept
{
	destroyTexture(resource);
}
