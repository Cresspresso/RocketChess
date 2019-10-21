/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_scene.cpp
**
**	Summary		:	Main Menu scene. Jumpboard to other scenes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <cress/moo/final_act.hpp>

#include "singleton.hpp"
#include "screen.hpp"

#include "audio.hpp"
#include "toggle_music.hpp"
#include "input.hpp"
#include "time.hpp"

#include "resource_warehouse.hpp"

#include "debug_log.hpp"
#include "MissileManager.h"
#include "menu_scene.hpp"

#include <iostream>



ivec2 globalPosition;

#pragma region Scene::Builder



	void Scene::initEntities()
	{


#ifdef _DEBUG
		Rubles = 40;
		Dollars = 40;
#endif // _DEBUG

		
		auto& resources = singleton::getResources();

		// hud camera
		{
			auto& proj = cameraHud.projection;
			proj.isOrthographic = true;
			proj.halfHeight = 400;

			cameraHud.transform.eye = vec3(0, 0, proj.nearClip + (0.5f * proj.farClip));

			cameraHud.recalculate();
		}

		//Title.render();

		// main menu buttons
		mainMenuButtons = MainMenuButtons{
			vec3(0, 200, 0),
			vec3(0, -100, 0),
			{
			"      New Game",
			"    Instructions",
			"        Credits",
			"           Exit",
			},
		};

		mainMenuButtons.buttons[0].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		mainMenuButtons.buttons[1].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		mainMenuButtons.buttons[2].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		mainMenuButtons.buttons[3].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);

		pauseMenuButtons = MainMenuButtons{
			vec3(-200, 100, 0),
			vec3(0, -100, 0),
			{
			"Continue",
			"Exit to Menu",
			"    Exit",
			},
		};

		pauseMenuButtons.buttons[0].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		pauseMenuButtons.buttons[1].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		pauseMenuButtons.buttons[2].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);


		TabHelp.textRenderer.text = "<- TAB ->";
		TabHelp.transform.localPosition = vec3(335, 185, 0);

		TabExit.textRenderer.text = "<- EXIT ->";
		TabExit.transform.localPosition = vec3(-485, 365, 0);

		missilePurchaseButtons = MainMenuButtons{
			vec3(400, 100, 0),
			vec3(0, -100, 0),
			{
			"     Cancel",
			"   $3  RPG",
			"$8 Cruise Missile",
			"$13 S8TAN [NUKE]",
			"$22  Voyager 1",
			},
		};


		missilePurchaseButtons.buttons[0].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		missilePurchaseButtons.buttons[1].buttonEntity.textEntity.material.tint = glm::vec3(0.0f, 0.85f, 0.0f);
		missilePurchaseButtons.buttons[2].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 0.75f, 0.0f);
		missilePurchaseButtons.buttons[3].buttonEntity.textEntity.material.tint = glm::vec3(0.85f, 0.0f, 0.0f);
		missilePurchaseButtons.buttons[4].buttonEntity.textEntity.material.tint = glm::vec3(0.0f, 1.0f, 1.0f);

		pawnPromotionButtons = MainMenuButtons{
	vec3(400, 100, 0),
	vec3(0, -100, 0),
	{
	"  Rook",
	"Knight",
	"Bishop",
	" Queen",
	},
		};


		pawnPromotionButtons.buttons[0].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 1.0f, 1.0f);
		pawnPromotionButtons.buttons[1].buttonEntity.textEntity.material.tint = glm::vec3(0.0f, 0.85f, 0.0f);
		pawnPromotionButtons.buttons[2].buttonEntity.textEntity.material.tint = glm::vec3(1.0f, 0.75f, 0.0f);
		pawnPromotionButtons.buttons[3].buttonEntity.textEntity.material.tint = glm::vec3(0.85f, 0.0f, 0.0f);

		// Textures Init Here
		Title.setTexture(TextureIndexer::Title);
		Title.transform.localScale = vec3(768, 256, 1);
		Title.transform.localPosition = vec3(0, 335.5, 0);

		selectionSprite.setTexture(TextureIndexer::Selection);
		selectionSprite.transform.localScale = vec3(64, 64, 1);

		actionSprite.setTexture(TextureIndexer::Action);
		actionSprite.transform.localScale = vec3(64, 64, 1);

		boardSprite.setTexture(TextureIndexer::ChessBoard);
		boardSprite.transform.localScale = vec3(615, 610, 1);
		boardSprite.transform.localPosition = vec3(-186.75, 45.5, 0);

		Background.setTexture(TextureIndexer::MainMenu);
		Background.transform.localScale = vec3(1065, 800, 1);
		Background.transform.localPosition = vec3(0, 0, 0);

		CallingCard.setTexture(TextureIndexer::USSRFLAG);
		CallingCard.transform.localScale = vec3(256, 128, 1);
		CallingCard.transform.localPosition = vec3(405, 337, 0);

		// Audio Init Here
		playMusic();
		
		// chess piece types
		for (size_t i = 0; i < chessSprites.size(); i++)
		{
			auto& sprite = chessSprites[i];
			sprite.setTexture(static_cast<TextureIndexer>(static_cast<int>(TextureIndexer::BishopUS) + i));
			sprite.transform.localScale = vec3(64, 64, 1);
		}

		//------------------------------------------------//
		//-------------------UI Text----------------------//
		//------------------------------------------------//

		SovietCurrency.transform.localPosition = vec3(265, 255, 0);
		UnitedStatesCurrency.transform.localPosition = vec3(285, 235, 0);

		//
		// Player Turn Labels
		//
		currentPlayerLabel.transform.localPosition = vec3(335, 300, 0);
	}



#pragma endregion ~Scene::Builder
#pragma region Scene

	void Scene::deselect() {
		selectedCoords = std::nullopt;
		isChoosingRocketTarget = false;
		availableActions.clear();
	};



	void Scene::onCellClicked(ivec2 cellCoords)
	{
		globalPosition = cellCoords;
		size_t const thatLinearIndex = getLinearIndex(cellCoords);
		auto& thatPiece = boardPieces[thatLinearIndex];

		if (selectedCoords || isChoosingRocketTarget)
		{
			auto it = availableActions.find(thatLinearIndex);
			if (it == availableActions.end()) // if no available action there
			{
				deselect();
			}
			else // if available action there
			{
				CRESS_MOO_FINAL_ACT_BEGIN(fa);
				{
					// finish move
					deselect();

					// next players's turn
					if (isCurrentPlayerTwo = !isCurrentPlayerTwo)
					{
						CallingCard.setTexture(TextureIndexer::USFLAG);
					}
					else { CallingCard.setTexture(TextureIndexer::USSRFLAG); }
					DEBUG_LOG("Next player's turn");
				}
				CRESS_MOO_FINAL_ACT_END(fa);

				auto const regularMove = [&] {
					assert(selectedCoords);
					auto& selectedPiece = boardPieces[getLinearIndex(*selectedCoords)];
					if (thatPiece.type != ChessPiece::None)
					{
						playSoundEffect(g_soundCapture);
						assert(thatPiece.isPlayer2 != selectedPiece.isPlayer2);

						// gain money from capture for specific piece type
						int reward = [&] { // IIFC
							switch (thatPiece.type)
							{
							case ChessPiece::Bishop:
								return 2;
							case ChessPiece::Knight:
								return 3;
							case ChessPiece::Pawn:
								return 1;
							case ChessPiece::PawnDouble:
								return 1;
							case ChessPiece::Queen:
								return 4;
							case ChessPiece::Rook:
								return 2;
							case ChessPiece::King:
								return 0;
								break;
							default:
								assert(false);
								break;
							}
							return 0;
						}();

						getCurrentPlayerMoney() += reward;
					}
					if (thatPiece.type == ChessPiece::None)
					{
						playSoundEffect(g_soundMovePiece);
					}
					bool const wasKing = thatPiece.type == ChessPiece::King;

					thatPiece = selectedPiece;
					selectedPiece = { ChessPiece::None };

					if (wasKing) {
						winGame(false);
					}
				};

				auto const& action = it->second;

				switch (action.type)
				{
				case ChessActionType::RegularMove:
				{
					// Sounds Stored in
					// Regular Move func
					regularMove();
				}
				break;

				case ChessActionType::RocketAttack:
				{
					playSoundEffect(g_soundCapture);
					thatPiece = { ChessPiece::None };
				}
				break;

				case ChessActionType::PawnPromotion:
				{
					playSoundEffect(g_soundSelect);
					regularMove();
					navigation->gamePanel = FocusedPanel::PawnPromotion();
					
					
				}
				break;
				case ChessActionType::PawnDoubleMove:{
					
					regularMove();
					thatPiece.type = ChessPiece::Pawn;
				}
				break;
				default:
					throw std::runtime_error("ChessActionType functionality not implemented");
					break;
				}

				int& money = getCurrentPlayerMoney();
				money -= action.cost;
			}
		}
		
		// NOTE: no 'else'. If it was deselected, check for new selection.
		// if no cell is currently selected, select the allied piece

		// NOTE: no 'else'. If it was deselected, check for new selection.
		// if no cell is currently selected, select the allied piece
		{
			if (thatPiece.type != ChessPiece::None)
			{
				if (this->isCurrentPlayerTwo == thatPiece.isPlayer2)
				{
					selectedCoords = cellCoords;

					availableActions.clear();

					int const yForward = thatPiece.isPlayer2 ? -1 : 1;

					// returns true if destination is empty or invalid coords
					auto const doit = [&](ivec2 const& neighbourCoords) -> bool {
						ivec2 const actionCoords = cellCoords + neighbourCoords;
						if (isValidCoords(actionCoords))
						{
							size_t const actionLinearIndex = getLinearIndex(actionCoords);
							// if the destination space is empty or has enemy piece
							auto const& destPiece = boardPieces[actionLinearIndex];
							if (destPiece.type == ChessPiece::None
								|| destPiece.isPlayer2 != isCurrentPlayerTwo)
							{
									availableActions.insert(std::make_pair(
										actionLinearIndex,
										ChessAction{ ChessActionType::RegularMove, actionCoords }
									));
							}
							return destPiece.type == ChessPiece::None;
						}
						return false;
					};


					// TODO test if king is in 'Check' before providing actions.
					switch (thatPiece.type)
					{
					case ChessPiece::Pawn:
					{
						// action: move 1 space forward or diagonal (up for white, down for black)
						std::array<ivec2, 3> const relativeCoords{
							ivec2(-1, yForward), ivec2(0, yForward), ivec2(1, yForward)
						};
						for (ivec2 const& relativeCoord : relativeCoords)
						{
							ivec2 const actionCoords = cellCoords + relativeCoord;
							if (isValidCoords(actionCoords))
							{
								size_t const actionLinearIndex = getLinearIndex(actionCoords);
								auto const& destPiece = boardPieces[actionLinearIndex];

								ChessActionType const type = (actionCoords.y == 0 || actionCoords.y == boardSize - 1)
									? ChessActionType::PawnPromotion
									: ChessActionType::RegularMove;

								auto const add = [&] {
									availableActions.insert(std::make_pair(
										actionLinearIndex,
										ChessAction{ type, actionCoords }
									));
								};

								// if moving straight forward
								if (relativeCoord.x == 0)
								{
									// if the destination space is empty
									if (destPiece.type == ChessPiece::None)
									{
										// the action can be performed
										add();
									}
								}
								else // if moving diagonally forward
								{
									// if enemy on destination
									if (destPiece.type != ChessPiece::None
										&& destPiece.isPlayer2 != thatPiece.isPlayer2)
									{
										// the action can be performed
										add();
									}
								}
							}
						}
					}
						break;
					//used for when the pawn has yet to move
					case ChessPiece::PawnDouble: {

						// action: move up to 2 spaces forward (up for white, down for black)
						std::array<ivec2, 5> const move2Coords{
 							ivec2(-1, yForward), ivec2(0, yForward), ivec2(1, yForward), ivec2(0, yForward * 2)
						};
						for (ivec2 const& relativeCoord : move2Coords)
						{
							ivec2 const actionCoords = cellCoords + relativeCoord;  
							ivec2 const pieceCoords = cellCoords;
							if (isValidCoords(actionCoords))
							{
								size_t const actionLinearIndex = getLinearIndex(actionCoords);
								auto const& destPiece = boardPieces[actionLinearIndex];
								ChessActionType const type = (actionCoords.y == 0 || actionCoords.y == boardSize - 1)
									? ChessActionType::PawnDoubleMove
									: ChessActionType::PawnDoubleMove;

								auto const add = [&] {
									availableActions.insert(std::make_pair(
										actionLinearIndex,
										ChessAction{ type, actionCoords }
									));
								};

								// if moving straight forward
								if (relativeCoord.x == 0)
								{
									//checks to see if there is a piece infront of it
									if (destPiece.type != ChessPiece::None) {
										return;
									}
		
									// if the destination space is empty
									if (destPiece.type == ChessPiece::None)
									{											// the action can be performed
											add();
										}
									}
								else // if moving diagonally forward
								{
									// if enemy on destination
									if (destPiece.type != ChessPiece::None
										&& destPiece.isPlayer2 != thatPiece.isPlayer2)
									{
										// the action can be performed
										add();
									}
								}
							}
						}

					}

					break;

					case ChessPiece::King:
					{
						// action: move 1 space in any direction
						static std::array<ivec2, 8> const neighboursCoords{
							ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1),
							ivec2(-1, 0), ivec2(1, 0),
							ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1)
						};

						for (ivec2 const& neighbourCoords : neighboursCoords)
						{
							ivec2 const actionCoords = cellCoords + neighbourCoords;
							if (isValidCoords(actionCoords))
							{
								size_t const actionLinearIndex = getLinearIndex(actionCoords);
								// if the destination space is empty or has enemy piece
								auto const& destPiece = boardPieces[actionLinearIndex];
								if (destPiece.type == ChessPiece::None
									|| destPiece.isPlayer2 != isCurrentPlayerTwo)
								{
									// check if enemy king is next to destination
									bool const enemyKingNearDestination = std::any_of(neighboursCoords.begin(), neighboursCoords.end(),
										[&](ivec2 const& neighbourCoords)
									{
										ivec2 const kingCoords = actionCoords + neighbourCoords;
										if (!isValidCoords(kingCoords))
										{
											return false;
										}
										auto const& kingPiece = boardPieces[getLinearIndex(kingCoords)];
										return kingPiece.type == ChessPiece::King
											&& kingPiece.isPlayer2 != thatPiece.isPlayer2;
									});

									if (!enemyKingNearDestination)
									{
										availableActions.insert(std::make_pair(
											actionLinearIndex,
											ChessAction{ ChessActionType::RegularMove, actionCoords }
										));
									}
								}
							}
						}
					}
					break;

					case ChessPiece::Rook: {

						//horizontal
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, 0));
							if (!wasEmpty) { break; }
						}
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, 0));
							if (!wasEmpty) { break; }
						}
						//vertical
						for (int e = 1; e < 8; e++) {
							bool wasEmpty = doit(ivec2(0, e));
							if (!wasEmpty) { break; }
						}
						for (int e = 1; e < 8; e++) {
							bool wasEmpty = doit(ivec2(0, -e));
							if (!wasEmpty) { break; }
						}
					}
										   break;
					case ChessPiece::Bishop: {

						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, i));
							if (!wasEmpty) { break; }
						}

						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, -i));
							if (!wasEmpty) { break; }
						}
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, i));
							if (!wasEmpty) { break; }
						}

						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, -i));
							if (!wasEmpty) { break; }
						}
					}
											 break;

					case ChessPiece::Queen: {


						//horizontal
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, 0));
							if (!wasEmpty) { break; }
						}
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, 0));
							if (!wasEmpty) { break; }
						}
						//vertical
						for (int e = 1; e < 8; e++) {
							bool wasEmpty = doit(ivec2(0, e));
							if (!wasEmpty) { break; }
						}
						for (int e = 1; e < 8; e++) {
							bool wasEmpty = doit(ivec2(0, -e));
							if (!wasEmpty) { break; }
						}
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, i));
							if (!wasEmpty) { break; }
						}

						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(i, -i));
							if (!wasEmpty) { break; }
						}
						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, i));
							if (!wasEmpty) { break; }
						}

						for (int i = 1; i < 8; i++) {
							bool wasEmpty = doit(ivec2(-i, -i));
							if (!wasEmpty) { break; }
						}


					}
					break;

					case ChessPiece::Knight: {
						// action: move 1 space in any direction
						static std::array<ivec2, 8> const neighboursCoords{
							ivec2(1, 2), ivec2(-1, 2), ivec2(2, -1),
							ivec2(2, 1), ivec2(-2, -1),
							ivec2(-2, 1), ivec2(1, -2), ivec2(-1, -2)
						};

						for (ivec2 const& neighbourCoords : neighboursCoords)
						{
							ivec2 const actionCoords = cellCoords + neighbourCoords;
							if (isValidCoords(actionCoords))
							{
								size_t const actionLinearIndex = getLinearIndex(actionCoords);
								// if the destination space is empty or has enemy piece
								auto const& destPiece = boardPieces[actionLinearIndex];
								if (destPiece.type == ChessPiece::None
									|| destPiece.isPlayer2 != isCurrentPlayerTwo)
								{
										availableActions.insert(std::make_pair(
											actionLinearIndex,
											ChessAction{ ChessActionType::RegularMove, actionCoords }
										));
								}
							}
						}
					}
					break;

					case ChessPiece::None:
						throw std::runtime_error("should be unreachable code");
						break;

					default:
						selectedCoords = std::nullopt;
						throw std::runtime_error("invalid chess piece type");
						break;
					}

					// after adding actions
					// if no actions exist
					// deselect
					if (availableActions.empty())
					{
						selectedCoords = std::nullopt;
					}
				}
			}
		}
	}


	//Used for when a rocket is chosen to be shot
	//prevents the user from destroying anything that the missile cannot
	//destroy
	void Scene::onRocketClicked(int rocket)
	{
		int& money = getCurrentPlayerMoney();
		//based on what missile the user has chosen
		switch (rocket)
		{
			//RPG
		case 1:
		{
			int cost = 3;
			if (money >= cost) {
				deselect();
				isChoosingRocketTarget = true;
				for (int y = 0; y < boardSize; y++)
				{
					for (int x = 0; x < boardSize; x++)
					{
						ivec2 const coords = ivec2(x, y);
						auto const& piece = boardPieces[getLinearIndex(coords)];
						if (piece.type != ChessPiece::None
							&& piece.isPlayer2 != this->isCurrentPlayerTwo)
						{//checking to see what pieces can be taken
							if (piece.type == ChessPiece::Pawn || piece.type == ChessPiece::PawnDouble) {
								availableActions.insert(std::make_pair(
									getLinearIndex(coords),
									ChessAction{ ChessActionType::RocketAttack, coords, cost }
								));
							}
						}
					}
				}
			}
		}
			break;
		//Ballistic Missile
		case 2:
		{
			int cost = 8;
			if (money >= cost) {
				deselect();
				isChoosingRocketTarget = true;
				for (int y = 0; y < boardSize; y++)
				{
					for (int x = 0; x < boardSize; x++)
					{
						ivec2 const coords = ivec2(x, y);
						auto const& piece = boardPieces[getLinearIndex(coords)];
						if (piece.type != ChessPiece::None
							&& piece.isPlayer2 != this->isCurrentPlayerTwo)
						{//checking to see what pieces can be taken
							if (piece.type == ChessPiece::Pawn 
								|| piece.type == ChessPiece::Rook 
								|| piece.type == ChessPiece::Bishop 
								|| piece.type == ChessPiece::PawnDouble) {
								availableActions.insert(std::make_pair(
									getLinearIndex(coords),
									ChessAction{ ChessActionType::RocketAttack, coords, cost }
								));
							}
						}
					}
				}
			}
		}
		break;
		//ICBM
		case 3:
		{
			int cost = 13;
			if (money >= cost) {
				deselect();
				isChoosingRocketTarget = true;
				for (int y = 0; y < boardSize; y++)
				{
					for (int x = 0; x < boardSize; x++)
					{
						ivec2 const coords = ivec2(x, y);
						auto const& piece = boardPieces[getLinearIndex(coords)];
						if (piece.type != ChessPiece::None
							&& piece.isPlayer2 != this->isCurrentPlayerTwo)
						{//checking to see what pieces can be taken
							if (piece.type == ChessPiece::Pawn 
								|| piece.type == ChessPiece::Rook 
								|| piece.type == ChessPiece::Bishop 
								|| piece.type == ChessPiece::Queen 
								|| piece.type == ChessPiece::Knight 
								|| piece.type == ChessPiece::PawnDouble) {
								availableActions.insert(std::make_pair(
									getLinearIndex(coords),
									ChessAction{ ChessActionType::RocketAttack, coords, cost }
								));
							}
						}
					}
				}
			}
		}
		break;
		//Voyager 1
		case 4: {
			int cost = 22;
			if (money >= cost) {
				deselect();
				money -= cost;
				winGame(true);
			}
		}
		break;
		}
	}
	//determines what piece the promoted unit is to become
	void Scene::onPawnPromotion(int piece)
	{
		size_t const thatLinearIndex = getLinearIndex(globalPosition);
		auto& thatPiece = boardPieces[thatLinearIndex];

		switch (piece) {
		case 1: {
			thatPiece.type = ChessPiece::Rook;
		}
				break;
		case 2: {
			thatPiece.type = ChessPiece::Knight;
		}
				break;
		case 3: {
			thatPiece.type = ChessPiece::Bishop;
		}
				break;
		case 4: {
			thatPiece.type = ChessPiece::Queen;
		}
				break;
		
		}
	}

	bool Scene::isValidCoords(ivec2 cellCoords)
	{
		return cellCoords.x >= 0 && cellCoords.x < boardSize
			&& cellCoords.y >= 0 && cellCoords.y < boardSize;
	}

	size_t Scene::getLinearIndex(ivec2 cellCoords)
	{
		if (!isValidCoords(cellCoords))
		{
			throw std::runtime_error("cell coords out of range");
		}
		return static_cast<size_t>(cellCoords.x) + static_cast<size_t>(cellCoords.y) * boardSize;
	}




	void Scene::init()
	{
		initEntities();
		initBehaviour();

		navigation = std::make_unique<Navigation>(
			[this](ivec2 coords) { onCellClicked(coords); },
		[this](int rocket) { onRocketClicked(rocket); }
		);
		navigation->onPawnPromotion = [this](int i) { onPawnPromotion(i); };

		missile.scene = this;
	}

	void Scene::destroy() noexcept
	{
		stopMusic();
		stopMusicG();
		navigation.reset();
	}



	void Scene::update()
	{
		if (doGameOverEvent)
		{
			navigation->gamePanel.emplace<FocusedPanel::OutcomeScreen>();
			doGameOverEvent = false;
		}

		updateAudio();

		if (isRestarting())
		{
			if (m_restartDelay->updateClampedFinished(Time::getDeltaTime()))
			{
				postRestartGameMessage();
				return;
			}
		}

		navigation->update();
	}



	void Scene::render()
	{
		cameraHud.recalculate();
		cameraHud.useForRendering();

		// render Background
		try { Background.render(); }
		
		catch (...) { printException(); }

		if (navigation->isGameSceneVisible())
		{

			// render help tab
			try {
				TabHelp.render();
			}
			catch (...) { printException(); }

			// render help tab
			try {
				TabExit.render();
			}
			catch (...) { printException(); }

			// render board
			try {
				boardSprite.render();
			}
			catch (...) { printException(); }

			// render Calling card
			try {
				CallingCard.render();
			}
			catch (...) { printException(); }

			auto const coordsToHudPosition = [](ivec2 coords) -> vec3
			{
				return vec3(
					vec2(coords) * 70.0f + vec2(-430, -200),
					0);
			};

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// render chess board array
			for (int y = 0; y < boardSize; y++)
			{
				for (int x = 0; x < boardSize; x++)
				{
					auto& piece = boardPieces[getLinearIndex({ x, y })];
					if (piece.type != ChessPiece::None)
					{
						auto& sprite = chessSprites[GetPieceType(x, y)];
						sprite.transform.localPosition = coordsToHudPosition({ x, y });
						try {
							sprite.render();
						}
						catch (...) { printException(); }
					}
				}
			}


			// render selection outline
			if (selectedCoords)
			{
				selectionSprite.transform.localPosition = coordsToHudPosition(*selectedCoords);
				try {
					selectionSprite.render();
				}
				catch (...) { printException(); }
			}

			// render available actions
			for (auto const& pair : availableActions)
			{
				ChessAction const& action = pair.second;

				// TODO use different sprite with texture for action type.
				actionSprite.transform.localPosition = coordsToHudPosition(action.coords);
				try {
					actionSprite.render();
				}
				catch (...) { printException(); }
			}

			glDisable(GL_BLEND);



			// Needs To Be Set To A Button Similar To The Quit Button
			// Also Needs To Have A Defined State Of Play Where Players Can
			// Take Turns Moving Their Pieces In Accordance To The Rules Of Chess
			// Call UI Section for the Players move US & USSR
			auto& PlayerType = isCurrentPlayerTwo;
			if (isCurrentPlayerTwo == true) // Needs Switching
			{
				currentPlayerLabel.textRenderer.text = "US's Move";
				currentPlayerLabel.material.tint = vec3(0.2, 0.1, 0.85);//(0.65, 0.2, 0.85);
			}
			else //if (isCurrentPlayerTwo == false) // Default:
			{
				currentPlayerLabel.textRenderer.text = "USSR's Move";
				currentPlayerLabel.material.tint = vec3(0.65, 1, 0.65);
			}
			try {
				currentPlayerLabel.render();
			}
			catch (...) { printException(); }

			// render currency
			SovietCurrency.textRenderer.text = "Soviet Rubles $ " + toString(Rubles);
			UnitedStatesCurrency.textRenderer.text = "US Dollars $ " + toString(Dollars);

			try {
				SovietCurrency.render();
			}
			catch (...) { printException(); }

			try {
				UnitedStatesCurrency.render();
			}
			catch (...) { printException(); }
		}

		// render missile purchase buttons
		{
			this->navigation->visit(overload{
				[&](FocusedPanel::MainMenu const& panelData) {},
				[&](FocusedPanel::RocketPurchase const& panelData)
			{
				try {
					missilePurchaseButtons.highlight(static_cast<size_t>(panelData.focusedButton));
					missilePurchaseButtons.render();
				}
				catch (...) { printException(); }
			},
				[&](FocusedPanel::PawnPromotion const& panelData)
			{
				try {
					pawnPromotionButtons.highlight(static_cast<size_t>(panelData.focusedButton));
					pawnPromotionButtons.render();
				}
				catch (...) { printException(); }
			},
				[&](auto const& other) {},
				});
		}



		// render main menu buttons
		{
			this->navigation->visit(overload{
				[&](FocusedPanel::MainMenu const& panelData)
			{
				try {
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					Title.render();// HERE...
					glDisable(GL_BLEND);
					mainMenuButtons.highlight(static_cast<size_t>(panelData.focusedButton));
					mainMenuButtons.render();
				}
				catch (...) { printException(); }
			},
				[&](auto const& other)
			{
			},
				});
		}



		// render other screens
		{
			this->navigation->visit(overload{
				[&](FocusedPanel::InstructionsMenu const& panelData)
			{
				try {
					instructions.render();
				}
				catch (...) { printException(); }
			},
				[&](FocusedPanel::CreditsMenu const& panelData)
			{
				try {
					credits.render();
				}
				catch (...) { printException(); }
			},
				[&](FocusedPanel::OutcomeScreen const& panelData)
			{
				try {
					outcomeScreen.render();
				}
				catch (...) { printException(); }
			},
				[&](auto const& other) {}
				});
		}


		// render pause menu
		if (navigation->pauseMenu)
		{
			try {
				pauseMenuButtons.highlight(static_cast<size_t>(navigation->pauseMenu->focusedButton));
				pauseMenuButtons.render();
			}
			catch (...) { printException(); }
		}



		try { navigation->render(); }
		catch (...) { printException(); }
	}

	void Scene::initBehaviour()
	{
		// place chess pieces on board
		auto const initPlayer = [this](bool isPlayer2) {
			int y = isPlayer2 ? 7 : 0;
			auto& board = boardPieces;
			board[0 + y * 8] = board[7 + y * 8] = { ChessPiece::Rook, isPlayer2 };
			board[1 + y * 8] = board[6 + y * 8] = { ChessPiece::Knight, isPlayer2 };
			board[2 + y * 8] = board[5 + y * 8] = { ChessPiece::Bishop, isPlayer2 };
			board[3 + y * 8] = { ChessPiece::Queen, isPlayer2 };
			board[4 + y * 8] = { ChessPiece::King, isPlayer2 };
			y = isPlayer2 ? 6 : 1;
			for (int x = 0; x < 8; ++x)
			{
				board[x + y * 8] = { ChessPiece::PawnDouble, isPlayer2 };
			}
		};
		initPlayer(false);
		initPlayer(true);
	}

	int Scene::GetPieceType(int x, int y)
	{
		// return int in range 0..<12
		switch (this->boardPieces[x + y * 8].type)
		{
			case ChessPiece::Bishop:
				{
				return this->boardPieces[x + y * 8].isPlayer2
					? 0
					: 1;
				}
				
			case ChessPiece::King:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 2
					: 3;
			}
		
			case ChessPiece::Knight:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 4
					: 5;
			}
			case ChessPiece::Pawn:
			case ChessPiece::PawnDouble:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 6
					: 7;
			}

			case ChessPiece::Queen:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 8
					: 9;
			}
			case ChessPiece::Rook:
			{
				return this->boardPieces[x + y * 8].isPlayer2
					? 10
					: 11;
			}
			case ChessPiece::None:
			{
				return 0;
			}
			default:
			{	return 0; }
		}
	}

	void Scene::winGame(bool winByVoyager)
	{
		outcomeScreen.UpdateWinner(isCurrentPlayerTwo, winByVoyager);
		doGameOverEvent = true;
		//postRestartGameMessage(2.0f);
	}

	bool Scene::isRestarting() const
	{
		return m_restartDelay.has_value();
	}

	void Scene::postRestartGameMessage(float delay)
	{
		m_restartDelay.emplace(delay);
	}

	void Scene::postRestartGameMessage()
	{
		singleton::postRestartMessage();
	}

#pragma endregion ~Scene


#pragma region missileStuffs



	//chose where to launch the missile
	void Scene::MissilePosition() {

		//moves cursor back to the chessboard
		navigation->gamePanel = FocusedPanel::ChessBoard();

		/*TODO
		player choses where to launch the missile
		checks to see what piece is under it and passes that back to the
		missile manager and the missile manager can determine whether
		the piece is destroyed or not*/

	}


	//returns user to the chessboard after launching the missile
	void Scene::LaunchedMissile() {
		//resets the cursor back to the chessBoard
		navigation->gamePanel = FocusedPanel::ChessBoard();

		try { navigation->render(); }
		catch (...) { printException(); }
	}





#pragma endregion missileStuffs
