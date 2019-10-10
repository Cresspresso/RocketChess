
#pragma once

#include <vector>
#include <optional>

#include "main_menu_button_entity.hpp"



struct MainMenuButtons
{
	std::vector<MainMenuButtonEntity> buttons;

	MainMenuButtons() = default;
	explicit MainMenuButtons(vec3 const& position, vec3 const& offset, std::vector<std::string> const& texts);
	void highlight(std::optional<size_t> index);

	void render();
};
