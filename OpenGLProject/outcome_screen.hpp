
#pragma once

#include "main_menu_buttons.hpp"
#include "multiline_text_entity.hpp"

struct OutcomeScreen
{
	MainMenuButtons m_buttons;
	TextEntity m_title;
	TextEntity m_winnerNation;
	MultilineTextEntity m_methodOfVictory;
	SpriteEntity m_winnerFlag;

	OutcomeScreen();

	void UpdateWinner(bool winnerIsUS, bool victoryByVoyager);

	void render();
};
