/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_button.hpp
**
**	Summary		:	Button that executes an action when it is clicked or a hotkey is pressed.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <functional>
#include <vector>

#include "transform.hpp"
#include "text_renderer.hpp"

struct Button
{
	Transform transform;

	Transform colliderTransform; // child

	Transform backgroundTransform; // child
	Renderer background;

	Transform textTransform; // child
	TextRenderer text;

	struct ClickEvent
	{
		std::vector<unsigned char> hotkeys; // can be empty
		std::function<ReturnCode()> action = nullptr;

		ReturnCode update(); // checks input for the hotkey
		ReturnCode operator()(); // invokes the action
	};

	ClickEvent onClickLeft;
	ClickEvent onClickRight;



	Button();

	// recalculates all transform matrices
	void recalculate();

	// must be called after recalculate.
	ReturnCode update();

	// must be called after recalculate.
	ReturnCode render();

	// must be called after recalculate.
	// returns true if mouse ray intersects button collider quad.
	bool isMouseInsideQuad();

	// must be called after recalculate.
	void updatePointerCheck();
};
