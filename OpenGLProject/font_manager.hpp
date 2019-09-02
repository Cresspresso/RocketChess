/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	font_manager.hpp
**
**	Summary		:	Collection of fonts to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "font.hpp"
#include "resource_manager.hpp"

enum class FontIndexer : size_t
{
	Arial,
	SnackerComic,
	WaltographUI,
	END
};
constexpr size_t FontIndexerEND = static_cast<size_t>(FontIndexer::END);

struct FontManager : public ResourceManager<Font, FontIndexerEND, FontIndexer>
{
	using Super = ResourceManager<Font, FontIndexerEND, FontIndexer>;
	using typename Super::Resource;
	using typename Super::Indexer;

	ReturnCode load(Resource& out, size_t i) override;
	void destroyResource(Resource& resource) noexcept override;

	static constexpr std::array<char const* const, FontIndexerEND> names =
	{
		"arial",
		"SnackerComic",
		"waltographUI",
		// "BankGothicMedium" // To Be Added
	};
	static constexpr std::array<int, FontIndexerEND> sizes =
	{
		24,
		24,
		24,
	};
};
