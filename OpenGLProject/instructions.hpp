
#pragma once

#include "main_menu_buttons.hpp"
#include "multiline_text_entity.hpp"

struct Instructions
{
	MainMenuButtons m_buttons;
	TextEntity m_title;
	MultilineTextEntity m_instructions;
	MultilineTextEntity m_controls;

	Instructions();

	ReturnCode render();
};
