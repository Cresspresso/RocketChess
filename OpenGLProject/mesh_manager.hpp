/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mesh_manager.hpp
**
**	Summary		:	Collection of meshes to be generated at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "mesh.hpp"
#include "resource_manager.hpp"

enum class MeshIndexer : size_t
{
	Quad,
	Pyramid,
	Sphere,
	Skybox,
	Text,
	END
};
constexpr size_t MeshIndexerEND = static_cast<size_t>(MeshIndexer::END);

struct MeshManager : public ResourceManager<Mesh, MeshIndexerEND, MeshIndexer>
{
	using Super = ResourceManager<Mesh, MeshIndexerEND, MeshIndexer>;
	using typename Super::Resource;
	using typename Super::Indexer;

	void load(Resource& out, size_t i) override;
	void destroyResource(Resource& resource) noexcept override;
};
