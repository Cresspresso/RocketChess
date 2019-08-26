/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_scene.hpp
**
**	Summary		:	Main Menu scene. Jumpboard to other scenes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <array>
#include <map>
#include <optional>

#include "phong_material.hpp"
#include "quad4_material.hpp"
#include "reflection_material.hpp"
#include "rim_lighting_material.hpp"
#include "fog_material.hpp"

#include "camera_orbit_entity.hpp"
#include "skybox_entity.hpp"
#include "simple_entity.hpp"
#include "text_renderer.hpp"
#include "menu_button.hpp"

#include "Model.h"

#include "ChessPiece.hpp"
#include "ChessAction.hpp"
#include "main_menu_buttons.hpp"

#include "navigation.hpp"



struct Scene
{
	// Materials
	Quad4Material matButtonBackground;
	TextMaterial matButtonText;

	std::array<SpriteEntity, 12> chessSprites;

	ReturnCode initMaterials();



	// Entities
	Camera cameraHud;

	MainMenuButtons mainMenuButtons;
	MainMenuButtons pauseMenuButtons;

	ReturnCode initEntities();



	// Behaviour
	std::array<OwnedChessPiece, 8 * 8> boardPieces;
	std::map<ivec2, ChessAction> availableActions;
	std::optional<ivec2> selectedCoords;
	bool isCurrentPlayerTwo = false;

	ReturnCode initBehaviour();

	int GetPieceType(int x, int y);
	ReturnCode onCellClicked(int x, int y);

	std::unique_ptr<Navigation> navigation;



	ReturnCode init();
	void destroy() noexcept { navigation.reset(); }

	ReturnCode update();
	ReturnCode render();

	void initTextRenderer(TextRenderer& entity);
	void initButton(Button& button);
};
