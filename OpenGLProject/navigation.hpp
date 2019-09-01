/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	navigation.hpp
**
**	Summary		:	Handles player UI navigation with arrow keys.
**
**	Project		:	GD2S02 Software Engineering
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	23/08/2019
*/

#pragma once

#include <functional>

#include "focused_panel.hpp"
#include "sprite_entity.hpp"



/*
**	Can be either in the main menu or game scene.
**	Can have pause menu open at the same time as either scene.
**	Renders the cursor over the focused button.
**	Does not render the scenes.
*/
class Navigation
{
public:
	explicit Navigation(std::function<void(ivec2)> onChessBoardCellPressed);
	void update();
	void render();

	std::variant<
		FocusedPanel::MainMenu,
		FocusedPanel::ChessBoard,
		FocusedPanel::RocketPurchase,
		FocusedPanel::EndTurn
	> gamePanel;

	std::optional<FocusedPanel::PauseMenu> pauseMenu;

	SpriteEntity spriteEntity;

	std::function<void(ivec2)> onChessBoardCellPressed = nullptr;

	bool isGameSceneVisible() const;
	bool isPauseMenuVisible() const;

	template<class Visitor>
	auto visit(Visitor&& visitor)
	{
		return std::visit(
			std::forward<Visitor>(visitor),
			gamePanel
		);
	}

private:
	void invokeAction();
	void handleMoveInput();
};
