/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cubemap_manager.hpp
**
**	Summary		:	Collection of cubemaps to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "textures.hpp"
#include "resource_manager.hpp"

enum class CubeMapIndexer : size_t
{
	Skybox,
	END
};
constexpr size_t CubeMapIndexerEND = static_cast<size_t>(CubeMapIndexer::END);

struct CubeMapManager : public ResourceManager<GLuint, CubeMapIndexerEND, CubeMapIndexer>
{
	using Super = ResourceManager<GLuint, CubeMapIndexerEND, CubeMapIndexer>;
	using typename Super::Resource;
	using typename Super::Indexer;

	void load(Resource& out, size_t i) override;
	void destroyResource(Resource& resource) noexcept override;

	static constexpr std::array<char const* const, CubeMapIndexerEND> names =
	{
		"skybox",
	};
};
