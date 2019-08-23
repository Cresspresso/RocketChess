
#include "application.hpp"
#include "singleton.hpp"
#include "input.hpp"
#include "math_utils.hpp"
#include "console.hpp"

#include "navigation.hpp"



Navigation::Navigation()
{
	using MM = FocusedPanels::MainMenuPanel;
	focusedPanel.emplace<MM>();

	transform.localScale = vec3(100, 100, 1);

	auto& resources = singleton::getResources();

	material.tex1 = resources.textures[TextureIndexer::AwesomeFace];

	renderer.program = resources.programs.getProgram(ProgramIndexer::Quad4);
	renderer.material = &material;
	renderer.mesh = &(resources.meshes[MeshIndexer::Quad]);
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
	if (getKeyboardState(' ') == InputState::DownFirst)
	{
		invokeAction();
	}
	else
	{
		handleMoveInput();
	}
}



void Navigation::render()
{
	bool doRender = true;

	// update transform position

	// do a switch statement on focusedPanel.
	{
		using namespace FocusedPanels;
		focusedPanel.visit<void>(
			// if it is a MainMenuPanel
			[&](MainMenuPanel& focusedPanelData)
		{
			using ButtonID = MainMenuPanel::ButtonID;
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
			[&](ChessBoardPanel& focusedPanelData)
		{
			ivec2 const coords = focusedPanelData.getFocusedCellCoords();

			vec2 const boardOrigin = vec2(-200, -200);
			transform.localPosition = vec3(boardOrigin + vec2(coords.x * 100, coords.y * 100), 0);
		},
			// else if
			[&](RocketPurchasePanel& focusedPanelData)
		{
			using ButtonID = RocketPurchasePanel::ButtonID;
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
			[&](EndTurnPanel& focusedPanelData)
		{
			using ButtonID = EndTurnPanel::ButtonID;
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
			// else if
			[&](PauseMenuPanel& focusedPanelData)
		{
			using ButtonID = PauseMenuPanel::ButtonID;
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
		);
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



void Navigation::invokeAction()
{
	// invoke focused button...

	// do a switch statement on focusedPanel.
	{
		using namespace FocusedPanels;
		focusedPanel.visit<void>(
			// if it is a MainMenuPanel
			[&](MainMenuPanel& focusedPanelData)
		{
			using ButtonID = MainMenuPanel::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::NewGame:
			{
				// TODO
				console::error("NewGame button not implemented.");
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
			[&](ChessBoardPanel& focusedPanelData)
		{
			ivec2 const coords = focusedPanelData.getFocusedCellCoords();
			// TODO
			console::error("Action in Board not implemented.");
		},
			// else if
			[&](RocketPurchasePanel& focusedPanelData)
		{
			using ButtonID = RocketPurchasePanel::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::BackToBoard:
			{
				// TODO
				console::error("BackToBoard button not implemented.");
			}
			break;

			case ButtonID::RPG:
			{
				// TODO
				console::error("RPG button not implemented.");
			}
			break;

			case ButtonID::ConventionalMissile:
			{
				// TODO
				console::error("ConventionalMissile button not implemented.");
			}
			break;

			case ButtonID::ICBM:
			{
				// TODO
				console::error("ICBM button not implemented.");
			}
			break;

			case ButtonID::Voyager1:
			{
				// TODO
				console::error("Voyager1 button not implemented.");
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
			[&](EndTurnPanel& focusedPanelData)
		{
			using ButtonID = EndTurnPanel::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::EndTurn:
			{
				// TODO
				console::error("EndTurn button not implemented.");
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
			// else if
			[&](PauseMenuPanel& focusedPanelData)
		{
			using ButtonID = PauseMenuPanel::ButtonID;
			switch (focusedPanelData.focusedButton)
			{
			case ButtonID::Continue:
			{
				// TODO
			}
			break;

			case ButtonID::ExitToMainMenu:
			{
				// TODO
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
		);
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

	// do a switch statement on focusedPanel.
	{
		using namespace FocusedPanels;
		focusedPanel.visit<void>(
			// if it is a MainMenuPanel
			[&](MainMenuPanel& focusedPanelData)
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
			[&](ChessBoardPanel& focusedPanelData)
		{
			ivec2 current = focusedPanelData.getFocusedCellCoords();
			current.x = cycle<int>(current.x + dir.x, 8);
			current.y = cycle<int>(current.y + dir.y, 8);
			focusedPanelData.setFocusedCellCoords(current);
		},
			// else if
			[&](RocketPurchasePanel& focusedPanelData)
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
			[&](EndTurnPanel& focusedPanelData)
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
			[&](PauseMenuPanel& focusedPanelData)
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
		);
	}
	//~ end visit
}
