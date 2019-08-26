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
#include "application.hpp"
#include "singleton.hpp"

#include "navigation.hpp"
#include "MissileManager.h"


missileManager missile;


Navigation::Navigation()
{
	transform.localScale = vec3(100, 100, 1);

	auto& resources = singleton::getResources();

	material.tex1 = resources.textures[TextureIndexer::AwesomeFace];

	renderer.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	renderer.material = &material;
	renderer.mesh = &(resources.meshes[MeshIndexer::Quad]);
}



bool Navigation::isGameSceneVisible() const
{
	return std::visit(overload{
		[](FocusedPanel::MainMenu const&) { return false; },
		[](auto const&) { return true; },
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
			transform.localPosition = vec3(200, 200, 0);
		}
		break;

		case ButtonID::ExitToMainMenu:
		{
			transform.localPosition = vec3(200, 100, 0);
		}
		break;

		case ButtonID::ExitToDesktop:
		{
			transform.localPosition = vec3(200, 000, 0);
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
				transform.localPosition = vec3(-300, 200, 0);
			}
			break;

			case ButtonID::Instructions:
			{
				transform.localPosition = vec3(-300, 100, 0);
			}
			break;

			case ButtonID::Options:
			{
				transform.localPosition = vec3(-300, 0, 0);
			}
			break;

			case ButtonID::ExitToDesktop:
			{
				transform.localPosition = vec3(-300, -100, 0);
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

			vec2 const boardOrigin = vec2(-200, -200);
			transform.localPosition = vec3(boardOrigin + vec2(coords.x * 100, coords.y * 100), 0);
		},
			// else if
			[&](RocketPurchase& focusedPanelData)
		{
			using ButtonID = RocketPurchase::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::BackToBoard:
			{
				transform.localPosition = vec3(200, 200, 0);
			}
			break;

			case ButtonID::RPG:
			{
				transform.localPosition = vec3(200, 100, 0);
			}
			break;

			case ButtonID::ConventionalMissile:
			{
				transform.localPosition = vec3(200, 000, 0);
			}
			break;

			case ButtonID::ICBM:
			{
				transform.localPosition = vec3(200, -100, 0);
			}
			break;

			case ButtonID::Voyager1:
			{
				transform.localPosition = vec3(200, -200, 0);
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
			[&](EndTurn& focusedPanelData)
		{
			using ButtonID = EndTurn::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::EndTurn:
			{
				transform.localPosition = vec3(300, -100, 0);
			}
			break;

			case ButtonID::Undo:
			{
				transform.localPosition = vec3(300, -200, 0);
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
		transform.recalculate();
		renderer.modelMatrix = transform.modelMatrix;
		if (RC_SUCCESS != renderer.render())
		{
			throw std::runtime_error("Failed to render Navigation");
		}
	}
}



namespace
{
	// Example return value is (0, 1) if player pressed UP arrow.
	ivec2 getInputDelta()
	{
		ivec2 dir{ 0, 0 };

		if (getKeyboardState('w') == InputState::DownFirst) { dir.y++; }
		if (getKeyboardState('W') == InputState::DownFirst) { dir.y++; }
		if (getSpecialState(GLUT_KEY_UP) == InputState::DownFirst) { dir.y++; }

		if (getKeyboardState('s') == InputState::DownFirst) { dir.y--; }
		if (getKeyboardState('S') == InputState::DownFirst) { dir.y--; }
		if (getSpecialState(GLUT_KEY_DOWN) == InputState::DownFirst) { dir.y--; }

		if (getKeyboardState('d') == InputState::DownFirst) { dir.x++; }
		if (getKeyboardState('D') == InputState::DownFirst) { dir.x++; }
		if (getSpecialState(GLUT_KEY_RIGHT) == InputState::DownFirst) { dir.x++; }

		if (getKeyboardState('a') == InputState::DownFirst) { dir.x--; }
		if (getKeyboardState('A') == InputState::DownFirst) { dir.x--; }
		if (getSpecialState(GLUT_KEY_LEFT) == InputState::DownFirst) { dir.x--; }

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
	if (getKeyboardState(KEY_ESCAPE) == InputState::DownFirst)
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
	else if (getKeyboardState(KEY_SPACE) == InputState::DownFirst)
	{
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
			gamePanel = MainMenu();
			pauseMenu = std::nullopt;
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
				gamePanel = ChessBoard();
			}
			break;

			case ButtonID::Instructions:
			{
				// TODO
				console::error("Instructions button not implemented.");
			}
			break;

			case ButtonID::Options:
			{
				// TODO
				console::error("Options button not implemented.");
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
				focusedPanelData.focusedButton = ButtonID::NewGame;
			}
			break;
			}
		},
			// else if it is a ChessBoardPanel
			[&](ChessBoard& focusedPanelData)
		{
			ivec2 const coords = focusedPanelData.getFocusedCellCoords();

			// TODO
			// DEBUG
			if (coords == ivec2(0, 1))
			{
				gamePanel = RocketPurchase();
			}
			else
			{
				console::error("Action in Board not implemented.");
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
				//purchase rpg
				missile.purchaseMissile(1);
			}
			break;

			case ButtonID::ConventionalMissile:
			{
				//purchase Ballistic missile
				missile.purchaseMissile(2);
			}
			break;

			case ButtonID::ICBM:
			{
				//purchase ICBM
				missile.purchaseMissile(3);
			}
			break;

			case ButtonID::Voyager1:
			{
				//purchase Voyager
				missile.purchaseMissile(4);
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
				cycleFocusedButton(-1);
			}
			if (moveDown)
			{
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
