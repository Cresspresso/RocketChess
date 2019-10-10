
#pragma once

#include "button_entity.hpp"



struct MainMenuButtonEntity
{
	ButtonEntity buttonEntity;

	MainMenuButtonEntity();

	void recalculate() { buttonEntity.recalculate(); }
	void render();
};
