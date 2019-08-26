
#pragma once

#include "quad4_material.hpp"
#include "button_renderer.hpp"



struct MainMenuButtonRenderer
{
	ButtonRenderer br;
	Quad4Material matBG;

	MainMenuButtonRenderer();

	ReturnCode render();
};
