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

#include "entity_utils.hpp"

#include "singleton.hpp"
#include "screen.hpp"

#include "audio.hpp"
#include "toggle_music.hpp"
#include "input.hpp"

#include "resource_warehouse.hpp"

#include "debug_log.hpp"

#include "menu_scene.hpp"



#pragma region Scene::Builder



	ReturnCode Scene::initEntities()
	{
		auto& resources = singleton::getResources();

		// hud camera
		{
			auto& proj = cameraHud.projection;
			proj.isOrthographic = true;
			proj.halfHeight = 400;

			cameraHud.transform.eye = vec3(0, 0, proj.nearClip + (0.5f * proj.farClip));

			cameraHud.recalculate();
		}

		// main menu buttons
		mainMenuButtons = MainMenuButtons{
			vec3(0, 200, 0),
			vec3(0, -100, 0),
			{
			"New Game",
			"Instructions",
			"Options",
			"Exit to Desktop",
			},
		};

		pauseMenuButtons = MainMenuButtons{
			vec3(-200, 100, 0),
			vec3(0, -100, 0),
			{
			"Continue",
			"Exit to Menu",
			"Exit to Desktop",
			},
		};

		missilePurchaseButtons = MainMenuButtons{
			vec3(300, 100, 0),
			vec3(0, -100, 0),
			{
			"Cancel",
			"RPG",
			"Conventional Missile",
			"ICBM",
			"Voyager 1",
			},
		};

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
		int Rubles = 0; // Values Can be Set to equal
		int Dollars = 0; // Functional values for currency...


		SovietCurrency.textRenderer.text = "Soviet Rubles $ " + toString(Rubles);
		UnitedStatesCurrency.textRenderer.text = "US Dollars $ " + toString(Dollars);
		SovietCurrency.transform.localPosition = vec3(315, 300, 0);
		UnitedStatesCurrency.transform.localPosition = vec3(335, 250, 0);


		//
		// Player Turn Labels
		//
		
		if (isCurrentPlayerTwo == true) // Needs Switching
		{
			currentPlayerLabel.transform.localPosition = vec3(365, 350, 0);
			
		}
		else if (isCurrentPlayerTwo == false)
		{
			currentPlayerLabel.transform.localPosition = vec3(365, 350, 0);

		}
		else
		{

		}


		return RC_SUCCESS;
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	void Scene::onCellClicked(ivec2 cellCoords)
	{
		size_t const thatLinearIndex = getLinearIndex(cellCoords);
		auto& thatPiece = boardPieces[thatLinearIndex];

		if (selectedCoords)
		{
			auto& selectedPiece = boardPieces[getLinearIndex(*selectedCoords)];
			assert(selectedPiece.isPlayer2 == this->isCurrentPlayerTwo);
			 
			auto it = availableActions.find(thatLinearIndex);
			if (it == availableActions.end()) // if no available action there
			{
				// deselect
				selectedCoords = std::nullopt;
				availableActions.clear();
			}
			else // if available action there
			{
				auto const& action = it->second;
				switch (action.type)
				{
				case ChessActionType::PawnMoveOne:
				{
					if (thatPiece.type == ChessPiece::None)
					{
						thatPiece = selectedPiece;
						selectedPiece = { ChessPiece::None };
					}
					// else TODO invalid move sound effect
				}
				break;

				case ChessActionType::KingMove:
				{
					if (thatPiece.type == ChessPiece::None)
					{
						thatPiece = selectedPiece;
						selectedPiece = { ChessPiece::None };
					}
					else if (thatPiece.isPlayer2 != selectedPiece.isPlayer2)
					{
						// TODO gain money from capture
						thatPiece = selectedPiece;
						selectedPiece = { ChessPiece::None };
					}
					// else TODO invalid move sound effect
				}
				break;

				default:
					throw std::runtime_error("ChessActionType functionality not implemented");
					break;
				}

				// finish move
				selectedCoords = std::nullopt;
				availableActions.clear();

				// next players's turn
				isCurrentPlayerTwo = !isCurrentPlayerTwo;
				DEBUG_LOG("Next player's turn");
			}
		}
		else // if no cell is currently selected, select the allied piece
		{
			if (thatPiece.type != ChessPiece::None)
			{
				if (this->isCurrentPlayerTwo == thatPiece.isPlayer2)
				{
					selectedCoords = cellCoords;

					availableActions.clear();

					int const yForward = thatPiece.isPlayer2 ? -1 : 1;
					
					// TODO test if king is in 'Check' before providing actions.
					switch (thatPiece.type)
					{
					case ChessPiece::Pawn:
					{
						// action: move 1 space forward (up for white, down for black)
						ivec2 const actionCoords = cellCoords + ivec2(0, yForward);
						if (isValidCoords(actionCoords))
						{
							size_t const actionLinearIndex = getLinearIndex(actionCoords);
							// if the destination space is empty
							auto const& destPiece = boardPieces[actionLinearIndex];
							if (destPiece.type == ChessPiece::None)
							{
								// the action can be performed
								ChessActionType const type = (actionCoords.y == 0 || actionCoords.y == boardSize - 1)
									? ChessActionType::PawnPromotion
									: ChessActionType::PawnMoveOne;

								availableActions.insert(std::make_pair(
									actionLinearIndex,
									ChessAction{ type, actionCoords }
								));
							}
						}
					}
					break;

					case ChessPiece::King:
					{
						// action: move 1 space in any direction
						static std::array<ivec2, 8> const neighboursCoords{
							ivec2(-1, 1), ivec2(0, 1), ivec2(1, 1),
							ivec2(-1, 0), ivec2(1, 1),
							ivec2(-1, -1), ivec2(0, -1), ivec2(1, -1)
						};

						for (ivec2 const& neighbourCoords : neighboursCoords)
						{
							ivec2 const actionCoords = cellCoords + neighbourCoords;
							if (isValidCoords(actionCoords))
							{
								size_t const actionLinearIndex = getLinearIndex(actionCoords);
								// if the destination space is empty
								auto const& destPiece = boardPieces[actionLinearIndex];
								if (destPiece.type == ChessPiece::None)
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
											ChessAction{ ChessActionType::KingMove, actionCoords }
										));
									}
								}
							}
						}
					}
					break;

					case ChessPiece::None:
						throw std::runtime_error("should be unreachable code");
						break;

					default:
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



	ReturnCode Scene::init()
	{
		BEGIN_ANYALL();
		{
			DO_ANYALL(initEntities());
			DO_ANYALL(initBehaviour());

			try {
				navigation = std::make_unique<Navigation>(
					[this](ivec2 coords) { onCellClicked(coords); }
				);
			} CATCH_PRINT();
		}
		return END_ANYALL();
	}



	ReturnCode Scene::update()
	{
		BEGIN_ANYALL();
		{
			try
			{
				navigation->update();
			}
			CATCH_PRINT();
		}
		return END_ANYALL();
	}



	ReturnCode Scene::render()
	{
		cameraHud.recalculate();
		cameraHud.useForRendering();

		BEGIN_ANYALL();
		{
			// render board
			{
				this->navigation->visit(overload{
					[&](FocusedPanel::MainMenu const& panelData)
				{
				},
					[&](auto const& other)
				{
					// render chess board
					for (int y = 0; y < boardSize; y++)
					{
						for (int x = 0; x < boardSize; x++)
						{
							auto& piece = boardPieces[getLinearIndex({ x, y })];
							if (piece.type != ChessPiece::None)
							{
								auto& sprite = chessSprites[GetPieceType(x, y)];
								sprite.transform.localPosition = vec3(x * 70 - 430, y * 70 - 200, 0);
								DO_ANYALL(sprite.render());
							}
						}
					}

					// render available actions
					for (auto const& pair : availableActions)
					{
						ChessAction const& action = pair.second;
						ivec2 const& coords = action.coords;
						vec2 const position = vec2(coords) * 70.0f + vec2(-430, -200);

						// TODO use different sprite with texture for action type.
						auto& sprite = chessSprites[0];
						sprite.transform.localPosition = vec3(position, 0);
						DO_ANYALL(sprite.render());
					}
				},
					});
			}

			// Needs To Be Set To A Button Similar To The Quit Button
			// Also Needs To Have A Defined State Of Play Where Players Can
			// Take Turns Moving Their Pieces In Accordance To The Rules Of Chess
			// Call UI Section for the Players move US & USSR
			auto& PlayerType = isCurrentPlayerTwo;
			if (isCurrentPlayerTwo == true) // Needs Switching
			{
				currentPlayerLabel.textRenderer.text = "US's Move";
				currentPlayerLabel.material.tint = vec3(0.2,0.1,0.85);//(0.65, 0.2, 0.85);
			}
			else //if (isCurrentPlayerTwo == false) // Default:
			{
				currentPlayerLabel.textRenderer.text = "USSR's Move";
				currentPlayerLabel.material.tint = vec3(0.65, 1, 0.65);
			}
			DO_ANYALL(currentPlayerLabel.render());
			DO_ANYALL(SovietCurrency.render());
			DO_ANYALL(UnitedStatesCurrency.render());



			// render missile purchase buttons
			{
				this->navigation->visit(overload{
					[&](FocusedPanel::MainMenu const& panelData)
				{
				},
					[&](FocusedPanel::RocketPurchase const& panelData)
				{
					missilePurchaseButtons.highlight(static_cast<size_t>(panelData.focusedButton));
					DO_ANYALL(missilePurchaseButtons.render());
				},
				[&](auto const& other)
				{
					missilePurchaseButtons.highlight(std::nullopt);
					DO_ANYALL(missilePurchaseButtons.render());
				},
					});
			}



			// render main menu buttons
			{
				this->navigation->visit(overload{
					[&](FocusedPanel::MainMenu const& panelData)
				{
					mainMenuButtons.highlight(static_cast<size_t>(panelData.focusedButton));
					DO_ANYALL(mainMenuButtons.render());
				},
				[&](auto const& other)
				{
				},
					});
			}



			// render pause menu buttons
			if (navigation->pauseMenu)
			{
				pauseMenuButtons.highlight(static_cast<size_t>(navigation->pauseMenu->focusedButton));
				DO_ANYALL(pauseMenuButtons.render());
			}



			try { navigation->render(); } CATCH_PRINT();
		}
		return END_ANYALL();
	}

	ReturnCode Scene::initBehaviour()
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
				board[x + y * 8] = { ChessPiece::Pawn, isPlayer2 };
			}
		};
		initPlayer(false);
		initPlayer(true);

		return RC_SUCCESS;
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

#pragma endregion ~Scene
