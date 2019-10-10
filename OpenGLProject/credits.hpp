
#pragma once

#include "main_menu_buttons.hpp"
#include "multiline_text_entity.hpp"

struct Credits
{
	MainMenuButtons m_buttons;
	TextEntity m_title;
	MultilineTextEntity m_credits;

	Credits();

	void render();
};
