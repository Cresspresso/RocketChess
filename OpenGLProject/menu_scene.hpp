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

#include <cress/moo/once_timer.hpp>

#include "camera.hpp"
#include "main_menu_buttons.hpp"

#include "instructions.hpp"
#include "credits.hpp"
#include "outcome_screen.hpp"

#include "navigation.hpp"

#include "ChessPiece.hpp"
#include "ChessAction.hpp"



struct Scene
{
	// Entities
	Camera cameraHud;

	MainMenuButtons mainMenuButtons;
	MainMenuButtons pauseMenuButtons;
	MainMenuButtons missilePurchaseButtons;
	MainMenuButtons pawnPromotionButtons;
	std::array<SpriteEntity, 12> chessSprites;
	SpriteEntity selectionSprite;
	SpriteEntity actionSprite;
	SpriteEntity boardSprite;
	SpriteEntity CallingCard;
	SpriteEntity Background;
	SpriteEntity RPG;
	SpriteEntity CruiseMissile;
	SpriteEntity ICBM;

	TextEntity currentPlayerLabel;
	TextEntity SovietCurrency;
	TextEntity UnitedStatesCurrency;
	TextEntity TabHelp;
	TextEntity TabExit;

	Instructions instructions;
	Credits credits;
	OutcomeScreen outcomeScreen;


	void initEntities();



	// Behaviour
	std::array<OwnedChessPiece, 8 * 8> boardPieces;
	std::map<size_t, ChessAction> availableActions; // key is linear index into board
	std::optional<ivec2> selectedCoords;
	bool isCurrentPlayerTwo = false;
	bool isChoosingRocketTarget = false;
	void deselect();

	void initBehaviour();

	int GetPieceType(int x, int y);
	void onCellClicked(ivec2 cellCoords);
	void onRocketClicked(int rocket);
	void onPawnPromotion(int piece);

	static constexpr size_t const boardSize = 8;
	static bool isValidCoords(ivec2 cellCoords);
	static size_t getLinearIndex(ivec2 cellCoords);

	std::unique_ptr<Navigation> navigation;
	bool doGameOverEvent = false;

	int& getCurrentPlayerMoney() { return isCurrentPlayerTwo ? Dollars : Rubles; };

	int Rubles = 0; // Values Can be Set to equal
	int Dollars = 0; // Functional values for currency...


	void init();
	void destroy() noexcept;

	void update();
	void render();

	void LaunchedMissile();

	void MissilePosition();

	void winGame(bool winByVoyager);

	std::optional<cress::moo::OnceTimer<float>> m_restartDelay;
	bool isRestarting() const;
	void postRestartGameMessage(float delay);
	void postRestartGameMessage();
};
