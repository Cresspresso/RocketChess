/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_button.cpp
**
**	Summary		:	Button that scales when hovered.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "menu_button.hpp"

void MenuButtonEntity::update()
{
	button.recalculate();
	bool const hovered = button.isMouseInsideQuad();
	button.transform.localScale = hovered ? scaleHovered : scaleNormal;

	button.recalculate();
	return button.update();
}

void MenuButtonEntity::render()
{
	button.recalculate();
	return button.render();
}
