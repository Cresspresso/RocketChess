/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	navigation.cpp
**
**	Summary		:	Handles player UI navigation with arrow keys.
**
**	Project		:	GD2S02 Software Engineering
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	23/08/2019
*/

#include "math_utils.hpp"
#include "input.hpp"
#include "console.hpp"
#include "singleton.hpp"


#include "audio.hpp"
#include "toggle_music.hpp"

#include "navigation.hpp"
#include "MissileManager.h"



Navigation::Navigation(std::function<void(ivec2)> onChessBoardCellPressed, std::function<void(int)> onRocketPressed)
	: onChessBoardCellPressed(onChessBoardCellPressed),
	onRocketPressed(onRocketPressed)
{
	spriteEntity.setTexture(TextureIndexer::Cursor);
	spriteEntity.transform.localScale = vec3(72, 72, 1);
}



bool Navigation::isGameSceneVisible() const
{
	return std::visit(overload{
		[](FocusedPanel::EndTurn const&) { return true; },
		[](FocusedPanel::ChessBoard const&) { return true; },
		[](FocusedPanel::RocketPurchase const&) { return true; },
		[](FocusedPanel::PawnPromotion const&) { return true; },
		[](auto const&) { return false; },
		}, gamePanel);
}



bool Navigation::isPauseMenuVisible() const
{
	return static_cast<bool>(pauseMenu);
}



void Navigation::render()
{
	using namespace FocusedPanel;

	bool doRender = true;

	// update transform position
	if (pauseMenu)
	{
		PauseMenu const& focusedPanelData = *pauseMenu;
		using ButtonID = PauseMenu::ButtonID;
		switch (focusedPanelData.focusedButton)
		{
		case ButtonID::Continue:
		{
			spriteEntity.transform.localPosition = vec3(-450, 100, 0);
		}
		break;

		case ButtonID::ExitToMainMenu:
		{
			spriteEntity.transform.localPosition = vec3(-450, 000, 0);
		}
		break;

		case ButtonID::ExitToDesktop:
		{
			spriteEntity.transform.localPosition = vec3(-450, -100, 0);
		}
		break;

		default:
		{
			doRender = false;
		}
		break;
		}
	}
	else
	{
		std::visit(overload{
			// if it is a MainMenuPanel
			[&](MainMenu& focusedPanelData)
		{
			using ButtonID = MainMenu::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::NewGame:
			{
				spriteEntity.transform.localPosition = vec3(-200, 200, 0);
			}
			break;

			case ButtonID::Instructions:
			{
				spriteEntity.transform.localPosition = vec3(-200, 100, 0);
			}
			break;

			case ButtonID::Credits:
			{
				spriteEntity.transform.localPosition = vec3(-200, 0, 0);
			}
			break;

			case ButtonID::ExitToDesktop:
			{
				spriteEntity.transform.localPosition = vec3(-200, -100, 0);
			}
			break;

			default:
			{
				doRender = false;
			}
			break;
			}
		},
			// else if it is a ChessBoardPanel
			[&](ChessBoard& focusedPanelData)
		{
			ivec2 const coords = focusedPanelData.getFocusedCellCoords();

			vec2 const boardOrigin = vec2(-430, -200);
			spriteEntity.transform.localPosition = vec3(boardOrigin + vec2(coords.x * 70, coords.y * 70), 0);
		},
			// else if
			[&](RocketPurchase& focusedPanelData)
		{
			using ButtonID = RocketPurchase::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::BackToBoard:
			{
				spriteEntity.transform.localPosition = vec3(200, 200, 0);
			}
			break;

			case ButtonID::RPG:
			{
				spriteEntity.transform.localPosition = vec3(200, 100, 0);
			}
			break;

			case ButtonID::ConventionalMissile:
			{
				spriteEntity.transform.localPosition = vec3(200, 000, 0);
			}
			break;

			case ButtonID::ICBM:
			{
				spriteEntity.transform.localPosition = vec3(200, -100, 0);
			}
			break;

			case ButtonID::Voyager1:
			{
				spriteEntity.transform.localPosition = vec3(200, -200, 0);
			}
			break;

			default:
			{
				doRender = false;
			}
			break;
			}
		},
			// else if
		[&](PawnPromotion& focusedPanelData)
	{
		using ButtonID = PawnPromotion::ButtonID;
		switch (focusedPanelData.focusedButton)
		{
		case ButtonID::Rook:
		{
			spriteEntity.transform.localPosition = vec3(200, 200, 0);
		}
		break;

		case ButtonID::Knight:
		{
			spriteEntity.transform.localPosition = vec3(200, 100, 0);
		}
		break;

		case ButtonID::Bishop:
		{
			spriteEntity.transform.localPosition = vec3(200, 000, 0);
		}
		break;

		case ButtonID::Queen:
		{
			spriteEntity.transform.localPosition = vec3(200, -100, 0);
		}
		break;

		default:
		{
			doRender = false;
		}
		break;
		}
	},
			// else if
		[&](InstructionsMenu& focusedPanelData)
	{
		spriteEntity.transform.localPosition = vec3(-200, -300, 0);
	},
		// else if
		[&](CreditsMenu& focusedPanelData)
	{
		spriteEntity.transform.localPosition = vec3(-200, -300, 0);
	},
		// else if
		[&](OutcomeScreen& focusedPanelData)
	{
		spriteEntity.transform.localPosition = vec3(-200, -300, 0);
	},
			// else if
			[&](EndTurn& focusedPanelData)
		{
			using ButtonID = EndTurn::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::EndTurn:
			{

				spriteEntity.transform.localPosition = vec3(300, -100, 0);
			}
			break;

			case ButtonID::Undo:
			{

				spriteEntity.transform.localPosition = vec3(300, -200, 0);
			}
			break;

			default:
			{
				doRender = false;
			}
			break;
			}
		},
			}, gamePanel);
	}
	//~ end visit

	// after updating transform
	if (doRender)
	{
		glEnable(GL_BLEND);
		CRESS_MOO_FINAL_ACT_SINGLE(fa, glDisable(GL_BLEND));
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		try {
			spriteEntity.render();
		}
		catch (...)
		{
			throw;
			//throw std::runtime_error("Failed to render Navigation");
		}
	}
}



namespace
{
	// Example return value is (0, 1) if player pressed UP arrow.
	ivec2 getInputDelta()
	{
		ivec2 dir{ 0, 0 };

		auto const isPressed = [](InputState state)
		{
			return state == InputState::DownFirst
				|| state == InputState::DownAgain;
		};

		if (isPressed(Input::getKeyboardState('w'))) { dir.y++; }
		if (isPressed(Input::getKeyboardState('W'))) { dir.y++; }
		if (isPressed(Input::getSpecialState(GLUT_KEY_UP))) { dir.y++; }

		if (isPressed(Input::getKeyboardState('s'))) { dir.y--; }
		if (isPressed(Input::getKeyboardState('S'))) { dir.y--; }
		if (isPressed(Input::getSpecialState(GLUT_KEY_DOWN))) { dir.y--; }

		if (isPressed(Input::getKeyboardState('d'))) { dir.x++; }
		if (isPressed(Input::getKeyboardState('D'))) { dir.x++; }
		if (isPressed(Input::getSpecialState(GLUT_KEY_RIGHT))) { dir.x++; }

		if (isPressed(Input::getKeyboardState('a'))) { dir.x--; }
		if (isPressed(Input::getKeyboardState('A'))) { dir.x--; }
		if (isPressed(Input::getSpecialState(GLUT_KEY_LEFT))) { dir.x--; }

		return dir;
	}



	template<class Enum>
	Enum cycleEnum(Enum value, int delta)
	{
		return static_cast<Enum>(cycle<int>(
			static_cast<int>(value) + delta,
			static_cast<int>(Enum::MAX) + 1
		));
	}

	template<class Enum>
	void cycleEnumInPlace(Enum& value, int delta)
	{
		value = cycleEnum(value, delta);
	}
}



void Navigation::update()
{
	if (Input::getKeyboardState(Input::KEY_ESCAPE) == InputState::DownFirst)
	{
		if (pauseMenu)
		{
			pauseMenu = std::nullopt;
		}
		else
		{
			pauseMenu = FocusedPanel::PauseMenu();
		}
	}
	else if (Input::getKeyboardState(Input::KEY_TAB) == InputState::DownFirst)
	{
		if (!pauseMenu)
		{
			using namespace FocusedPanel;
			visit(overload{
				[&](ChessBoard const&) {
					gamePanel = RocketPurchase();
				},
				[&](RocketPurchase const&) {
					gamePanel = ChessBoard();
				},
				[&](PawnPromotion const&) {
					gamePanel = ChessBoard();
				},
				[&](auto const&) {}
			});
		}
	}
	else if (Input::getKeyboardState(Input::KEY_SPACE) == InputState::DownFirst)
	{
#ifdef _DEBUG
		console::log("SPACE pressed.");
#endif
//		playSoundEffect(g_soundCapture);
		invokeAction();
	}
	else
	{

		handleMoveInput();
	}
}



void Navigation::invokeAction()
{
	using namespace FocusedPanel;

	if (pauseMenu)
	{
		PauseMenu& focusedPanelData = *pauseMenu;
		using ButtonID = PauseMenu::ButtonID;
		switch (focusedPanelData.focusedButton)
		{
		case ButtonID::Continue:
		{

			pauseMenu = std::nullopt;
		}
		break;

		case ButtonID::ExitToMainMenu:
		{
			singleton::postRestartMessage();

			//stopMusicG();
			//playMusic();
			//gamePanel = MainMenu();
			//pauseMenu = std::nullopt;
		}
		break;

		case ButtonID::ExitToDesktop:
		{
			glutLeaveMainLoop();
		}
		break;

		default:
		{
#ifdef _DEBUG
			console::error("focusedPanelData has an invalid focusedButton value.");
#endif
			focusedPanelData.focusedButton = ButtonID::Continue;
		}
		break;
		}
	}
	else
	{
		std::visit(overload{
			// if it is a MainMenuPanel
			[&](MainMenu& focusedPanelData)
		{
			using ButtonID = MainMenu::ButtonID;
			switch (focusedPanelData.focusedButton)
			{

			case ButtonID::NewGame:
			{
				// Audio
				stopMusic();

				playMusicG();
				playSoundEffect(g_musicGameBackground);
				//playSoundEffect(g_musicGameBackground);
				FMOD::Channel* g_musicChannel = nullptr;
				
				//FMOD_RESULT nr = g_audio->playSound(g_musicGameBackground, nullptr, true, &g_musicChannel);
				playSoundEffect(g_soundCapture);
				gamePanel = ChessBoard();
			}
			break;

			case ButtonID::Instructions:
			{
				playSoundEffect(g_soundNavigate);
				gamePanel = InstructionsMenu();
			}
			break;

			case ButtonID::Credits:
			{
				playSoundEffect(g_soundSelect);
				gamePanel = CreditsMenu();

			}
			break;

			case ButtonID::ExitToDesktop:
			{
				playSoundEffect(g_soundMovePiece);
				glutLeaveMainLoop();
			}
			break;

			default:
			{
#ifdef _DEBUG
				console::error("focusedPanelData has an invalid focusedButton value.");
#endif
				focusedPanelData.focusedButton = ButtonID::NewGame;
			}
			break;
			}
		},
			// else if it is a ChessBoardPanel
			[&](ChessBoard& focusedPanelData)
		{
			ivec2 const coords = focusedPanelData.getFocusedCellCoords();
			if (onChessBoardCellPressed != nullptr)
			{

				onChessBoardCellPressed(coords);
			}
			else
			{
				
				console::error("Action is not provided for when board cell pressed.");
			}
		},
			// else if
			[&](RocketPurchase& focusedPanelData)
		{
			using ButtonID = RocketPurchase::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::BackToBoard:
			{
				
				gamePanel = ChessBoard();
			}
			break;

			case ButtonID::RPG:
			{
				onRocketPressed(1);
				//playSoundEffect(g_sound);
				gamePanel = ChessBoard();
				//purchase rpg
			}
			break;

			case ButtonID::ConventionalMissile:
			{
				onRocketPressed(2);
				gamePanel = ChessBoard();
				//purchase Ballistic missile
			}
			break;

			case ButtonID::ICBM:
			{
				onRocketPressed(3);
				gamePanel = ChessBoard();
				//purchase ICBM
			}
			break;

			case ButtonID::Voyager1:
			{
				onRocketPressed(4);
				gamePanel = ChessBoard();
				//purchase Voyager
			}
			break;

			default:
			{
#ifdef _DEBUG
				console::error("focusedPanelData has an invalid focusedButton value.");
#endif
				focusedPanelData.focusedButton = ButtonID::BackToBoard;
			}
			break;
			}
		},
			// else if
			[&](PawnPromotion& focusedPanelData)
		{
			using ButtonID = PawnPromotion::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::Rook:
			{
				onPawnPromotion(1);
				gamePanel = ChessBoard();
			}
			break;

			case ButtonID::Knight:
			{
				onPawnPromotion(2);
				playSoundEffect(g_soundSelect);
				gamePanel = ChessBoard();
				//purchase rpg
			}
			break;

			case ButtonID::Bishop:
			{
				onPawnPromotion(3);
				gamePanel = ChessBoard();
				//purchase Ballistic missile
			}
			break;

			case ButtonID::Queen:
			{
				onPawnPromotion(4);
				gamePanel = ChessBoard();
				//purchase ICBM
			}
			break;

			default:
			{
#ifdef _DEBUG
				console::error("focusedPanelData has an invalid focusedButton value.");
#endif
				focusedPanelData.focusedButton = ButtonID::Queen;
			}
			break;
			}
		},
			// else if
			[&](InstructionsMenu& focusedPanelData)
		{
			gamePanel = MainMenu();
		},
			// else if
			[&](CreditsMenu& focusedPanelData)
		{
			gamePanel = MainMenu();
		},
			// else if
			[&](OutcomeScreen& focusedPanelData)
		{
			singleton::postRestartMessage();
			//gamePanel = MainMenu();
		},
			// else if
			[&](EndTurn& focusedPanelData)
		{
			using ButtonID = EndTurn::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::EndTurn:
			{
				// DEBUG

				gamePanel = MainMenu();
				// TODO
			}
			break;

			case ButtonID::Undo:
			{
				// TODO
				
				console::error("Undo button not implemented.");

			}
			break;

			default:
			{
#ifdef _DEBUG
				console::error("focusedPanelData has an invalid focusedButton value.");
#endif

				focusedPanelData.focusedButton = ButtonID::EndTurn;
			}
			break;
			}
		},
			}, gamePanel);
	}
	//~ end visit
}



void Navigation::handleMoveInput()
{
	// handle left/right/up/down movement...

	ivec2 const dir = getInputDelta();
	bool const moveUp = dir.y > 0;
	bool const moveDown = dir.y < 0;
	bool const moveRight = dir.x > 0;
	bool const moveLeft = dir.x < 0;

	if (dir == ivec2(0, 0)) { return; }



	using namespace FocusedPanel;

	if (pauseMenu)
	{
		PauseMenu& focusedPanelData = *pauseMenu;
		auto const cycleFocusedButton = [&](int delta)
		{
			cycleEnumInPlace(focusedPanelData.focusedButton, delta);
		};

		if (moveUp)
		{
			cycleFocusedButton(-1);
		}
		if (moveDown)
		{
			cycleFocusedButton(1);
		}
	}
	else
	{
		std::visit(overload{
			// if it is a MainMenuPanel
			[&](MainMenu& focusedPanelData)
		{
			auto const cycleFocusedButton = [&](int delta)
			{
				cycleEnumInPlace(focusedPanelData.focusedButton, delta);
			};

			if (moveUp)
			{

				cycleFocusedButton(-1);
			}
			if (moveDown)
			{

				cycleFocusedButton(1);
			}
		},
			// else if it is a ChessBoardPanel
			[&](ChessBoard& focusedPanelData)
		{
			ivec2 current = focusedPanelData.getFocusedCellCoords();
			current.x = cycle<int>(current.x + dir.x, 8);
			current.y = cycle<int>(current.y + dir.y, 8);
			focusedPanelData.setFocusedCellCoords(current);
		},
			// else if
			[&](RocketPurchase& focusedPanelData)
		{
			auto const cycleFocusedButton = [&](int delta)
			{	
				cycleEnumInPlace(focusedPanelData.focusedButton, delta);
			};

			if (moveUp)
			{
				playSoundEffect(g_soundNavigate);
				cycleFocusedButton(-1);
			}
			if (moveDown)
			{
				// Menu Sounds (Testing)
				playSoundEffect(g_soundNavigate);
				cycleFocusedButton(1);
			}
		},
			// else if
			[&](PawnPromotion& focusedPanelData)
		{
			auto const cycleFocusedButton = [&](int delta)
			{
				cycleEnumInPlace(focusedPanelData.focusedButton, delta);
			};

			if (moveUp)
			{
				playSoundEffect(g_soundNavigate);
				cycleFocusedButton(-1);
			}
			if (moveDown)
			{
				// Menu Sounds (Testing)
				playSoundEffect(g_soundNavigate);
				cycleFocusedButton(1);
			}
		},
			// else if
			[&](EndTurn& focusedPanelData)
		{
			auto const cycleFocusedButton = [&](int delta)
			{
				cycleEnumInPlace(focusedPanelData.focusedButton, delta);
			};

			if (moveUp)
			{
				cycleFocusedButton(-1);
			}
			if (moveDown)
			{
				cycleFocusedButton(1);
			}
		},
			// else if
			[&](InstructionsMenu& focusedPanelData)
		{
		},
			// else if
			[&](CreditsMenu& focusedPanelData)
		{
		},
			// else if
			[&](OutcomeScreen& focusedPanelData)
		{
		},
			// else if
			[&](PauseMenu& focusedPanelData)
		{
			auto const cycleFocusedButton = [&](int delta)
			{
				cycleEnumInPlace(focusedPanelData.focusedButton, delta);
			};

			if (moveUp)
			{
				cycleFocusedButton(-1);
			}
			if (moveDown)
			{
				cycleFocusedButton(1);
			}
		}		
			}, gamePanel);
	}
	//~ end visit
}