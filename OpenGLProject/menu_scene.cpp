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



#pragma region Scene::Materials::Builder



	ReturnCode Scene::initMaterials()
	{
		auto& resources = singleton::getResources();

		matButtonBackground.tex1 = resources.textures[TextureIndexer::Button];

		matButtonText.tint = vec3(0);

		for (size_t i = 0; i < chessMaterials.size(); i++)
		{
			chessMaterials[i].tex1 = resources.textures[static_cast<int>(TextureIndexer::BishopUS) + i];
		}

		return RC_SUCCESS;
	}



#pragma endregion ~Scene::Materials::Builder
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

		// quit button
		{
			auto& button = quitButton.button;
			initButton(button);

			// set callback
			button.onClickLeft.hotkeys.push_back(KEY_ESCAPE);
			button.onClickLeft.action = []
			{
				glutLeaveMainLoop();
				return RC_SUCCESS;
			};

			// init text renderer
			{
				auto& tx = button.text;
				tx.text = "Quit (ESC)";
				tx.position.x += 20;
			}

			button.transform.localPosition = vec3(500, 0, 0);
		}

		// board
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				auto& button = boardButtons[x + y * 8].button;

				// init collider transform
				{
					auto& ct = button.colliderTransform;
					ct.localScale = vec3(40, 40, 1);
				}

				// init background transform
				{
					auto& bgt = button.backgroundTransform;
					bgt.localScale = vec3(2.f * vec2(button.colliderTransform.localScale), 1);
				}

				// init background
				{
					auto& bg = button.background;
					bg.program = resources.programs[ProgramIndexer::Quad4].program;
					bg.mesh = &(resources.meshes[MeshIndexer::Quad]);
					bg.material = &matButtonBackground;// &(materials.buttonMenuBackground);
				}

				// init text renderer
				{
					auto& tx = button.text;
					initTextRenderer(tx); // &(materials.buttonMenuText);
					tx.scale = vec2(1);

					vec3 const buttonHalfSize = 0.5f * button.backgroundTransform.localScale;
					tx.position = vec2(-buttonHalfSize.x, -4);
				}

				button.transform.localPosition = glm::vec3(x * 100 - 440, y * 100 - 350, 0);

				// init text renderer
				{
					auto& tx = button.text;
					tx.text = std::to_string(x) + "," + std::to_string(y);
				}

				// set click action
				button.onClickLeft.action = [this, x, y]
				{
					return onCellClicked(x, y);
				};
			}
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
			DO_ANYALL(initMaterials());
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

			// button horizontal layout at top-left corner of hud
			/*{
				vec2 const hudHalfSize = cameraHud.projection.calculateHalfSize();
				vec3 const buttonHalfSize = 0.5f
					* quitButton.button.backgroundTransform.localScale
					* quitButton.scaleHovered;
				vec2 const menuButtonPos = vec2(
					-hudHalfSize.x + buttonHalfSize.x,
					hudHalfSize.y - buttonHalfSize.y
				);
				quitButton.button.transform.localPosition = vec3(menuButtonPos, 0);
			}*/

			for (auto& b : boardButtons)
			{
				DO_ANYALL(b.update());
			}

			DO_ANYALL(quitButton.update());
		}
		return END_ANYALL();
	}



	void Scene::initTextRenderer(TextRenderer & entity)
	{
		auto& resources = singleton::getResources();

		entity.font = &(resources.fonts[FontIndexer::Arial]);

		// Note: Mesh vertex buffer is modified at runtime.
		// DO NOT RENDER IN MULTITHREAD.
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::Text].program;
		r.mesh = &(resources.meshes[MeshIndexer::Text]);
		r.material = &matButtonText;
	}

	void Scene::initButton(Button& button)
	{
		auto& resources = singleton::getResources();

		// init collider transform
		{
			auto& ct = button.colliderTransform;
			ct.localScale = vec3(120, 30, 1);
		}

		// init background transform
		{
			auto& bgt = button.backgroundTransform;
			bgt.localScale = vec3(2.f * vec2(button.colliderTransform.localScale), 1);
		}

		// init background
		{
			auto& bg = button.background;
			bg.program = resources.programs[ProgramIndexer::Quad4].program;
			bg.mesh = &(resources.meshes[MeshIndexer::Quad]);
			bg.material = &matButtonBackground;
		}

		// init text renderer
		{
			auto& tx = button.text;
			initTextRenderer(tx); // &(materials.buttonMenuText);
			tx.scale = vec2(1);

			vec3 const buttonHalfSize = 0.5f * button.backgroundTransform.localScale;
			tx.position = vec2(-buttonHalfSize.x, -4);
		}
	}



	ReturnCode Scene::render()
	{
		cameraHud.recalculate();
		cameraHud.useForRendering();

		BEGIN_ANYALL();
		{
			DO_ANYALL(quitButton.render());

			for (int y = 0; y < 8; y++)
			{
				for (int x = 0; x < 8; x++)
				{
					auto& piece = boardPieces[x + y * 8];
					auto& button = boardButtons[x + y * 8];
					if (piece.type != ChessPiece::None)
					{
						button.button.text.text = stringLink(symbol(piece.type), " ", (piece.isPlayer2 ? "2" : "1"));
						button.button.background.material = &(chessMaterials[GetPieceType(x, y)]);
						DO_ANYALL(button.render());
					}
				}
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
