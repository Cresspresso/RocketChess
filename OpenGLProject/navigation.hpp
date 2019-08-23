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
#include "renderer.hpp"
#include "transform.hpp"
#include "quad4_material.hpp"



class Navigation
{
public:
	Navigation();
	void update();
	void render();

	FocusedPanel focusedPanel;
private:
	Quad4Material material;
	Renderer renderer;
	Transform transform;

	void invokeAction();
	void handleMoveInput();
};
