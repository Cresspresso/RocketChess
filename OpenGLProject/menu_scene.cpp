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

		return RC_SUCCESS;
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	ReturnCode Scene::onCellClicked(int x, int y)
	{
		if (selectedCoords)
		{
			if (*selectedCoords == ivec2(x, y))
			{
				selectedCoords.reset();
				return RC_SUCCESS;
			}

			else
			{
				isCurrentPlayerTwo = !isCurrentPlayerTwo;
				DEBUG_LOG("Next player's turn");
				return RC_SUCCESS;
			}
		}
		else
		{
			auto& piece = boardPieces[x + y * 8];
			bool const isPlayer2 = piece.isPlayer2;
			int const yForward = isPlayer2 ? -1 : 1;
			switch (piece.type) {
			case ChessPiece::Pawn:
			{
				auto& other = boardPieces[(x)+(y + yForward) * 8];
				if (other.type == ChessPiece::None)
				{
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
				return RC_ERROR;
			}
			case ChessPiece::King:
			{
				auto& other = boardPieces[(x)+(y + yForward) * 8];
				if (other.type == ChessPiece::None)
				{
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
				else if (other.isPlayer2 != piece.isPlayer2)
				{
					// TODO gain money from capture
					other = piece;
					piece = { ChessPiece::None };
					return RC_SUCCESS;
				}
			}
			case ChessPiece::None:
			default:
				break;
			}
			return RC_ERROR;
		}
	}

	ReturnCode Scene::init()
	{
		BEGIN_ANYALL();
		{
			DO_ANYALL(initEntities());
			DO_ANYALL(initBehaviour());
			try { navigation = std::make_unique<Navigation>(); } CATCH_PRINT();
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
			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					auto& piece = boardPieces[x + y * 8];
					if (piece.type != ChessPiece::None)
					{
						auto& sprite = chessSprites[GetPieceType(x, y)];
						sprite.transform.localPosition = vec3(x * 70 - 430, y * 70 - 200, 0);
						DO_ANYALL(sprite.render());
					}
				}
			}



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
