/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	program_manager.hpp
**
**	Summary		:	Collection of programs to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "program.hpp"
#include "resource_manager.hpp"

enum class ProgramIndexer : size_t
{
	Quad4,
	Phong,
	BlinnPhong,
	RimLighting,
	Gouraud,
	Skybox,
	Reflection,
	Model,
	Text,
	LinearFog,
	END
};
constexpr size_t ProgramIndexerEND = static_cast<size_t>(ProgramIndexer::END);

struct ProgramManager : public ResourceManager<Program, ProgramIndexerEND, ProgramIndexer>
{
	using Super = ResourceManager<Program, ProgramIndexerEND, ProgramIndexer>;
	using typename Super::Resource;
	using typename Super::Indexer;

	void load(Resource& out, size_t i) override;
	void destroyResource(Resource& resource) noexcept override;

	static constexpr std::array<char const* const, ProgramIndexerEND> names =
	{
		"quad4",
		"phong",
		"blinn_phong",
		"rim_lighting",
		"gouraud",
		"skybox",
		"reflection",
		"model",
		"text",
		"linear_fog",
	};

	GLuint getProgram(Indexer index) const { return operator[](index).program; }
};
