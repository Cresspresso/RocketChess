/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	texture_manager.hpp
**
**	Summary		:	Collection of textures to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "textures.hpp"
#include "resource_manager.hpp"

enum class TextureIndexer : size_t
{
	Button,
	White,
	BishopUS,
	BishopUSSR,
	KingUS,
	KingUSSR,
	KnightUS,
	KnightUSSR,
	PawnUS,
	PawnUSSR,
	QueenUS,
	QueenUSSR,
	RookUS,
	RookUSSR,
	ChessBoard,
	Selection,
	Action,
	Cursor,
	USSRFLAG,
	USFLAG,
	MainMenu,
	CruiseMissile,
	Title,
	END
};
constexpr size_t TextureIndexerEND = static_cast<size_t>(TextureIndexer::END);

struct TextureManager : public ResourceManager<GLuint, TextureIndexerEND, TextureIndexer>
{
	using Super = ResourceManager<GLuint, TextureIndexerEND, TextureIndexer>;
	using typename Super::Resource;
	using typename Super::Indexer;

	void load(Resource& out, size_t i) override;
	void destroyResource(Resource& resource) noexcept override;

	static constexpr std::array<char const* const, TextureIndexerEND> names =
	{
		"button.png",
		"white.png",
		"BishopUS.png",
		"BishopUSSR.png",
		"QueenUS.png",
		"QueenUSSR.png",
		"KnightUS.png",
		"KnightUSSR.png",
		"PawnUS.png",
		"PawnUSSR.png",
		"KingUS.png",
		"KingUSSR.png",
		"RookUS.png",
		"RookUSSR.png",
		"Chess-BoardO.png",
		"Selection.png",
		"Action.png",
		"Cursor.png",
		"UssrFlag.png",
		"UnitedStatesFlag.png",
		"black-world-map.jpg",
		"cruise-missile.png",
		"RocketChessTitle.png" 
	};
};
