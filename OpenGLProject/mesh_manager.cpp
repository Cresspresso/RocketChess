/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mesh_manager.cpp
**
**	Summary		:	Collection of meshes to be generated at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "quad.hpp"
#include "font.hpp"

#include "mesh_manager.hpp"

void MeshManager::load(Resource& out, size_t i)
{
	switch (static_cast<MeshIndexer>(i))
	{
	case MeshIndexer::Quad: out = makeQuadMesh(); break;
	case MeshIndexer::Text: out = makeFontMesh(); break;
	default: throw std::runtime_error("invalid MeshIndexer enum value");
	}
}

void MeshManager::destroyResource(Resource& resource) noexcept
{
	resource.destroy();
}
