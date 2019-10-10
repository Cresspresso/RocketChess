/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	input.hpp
**
**	Summary		:	Mouse and keyboard input.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

enum class InputState
{
	Up = 0,
	Down,
	UpFirst,
	DownFirst,
	UpAgain,
	DownAgain,
};

namespace Input
{
	bool isInputStateDown(InputState state) noexcept;

	enum InputMouseButton : int
	{
		MOUSE_LEFT = GLUT_LEFT_BUTTON,
		MOUSE_MIDDLE = GLUT_MIDDLE_BUTTON,
		MOUSE_RIGHT = GLUT_RIGHT_BUTTON,
	};

	enum InputKeyboardKey : unsigned char
	{
		KEY_SPACE = ' ',
		KEY_ESCAPE = 27,
		KEY_ENTER = 13,
		KEY_TAB = 9,
	};

	void onKeyboardDown(unsigned char key, int mouseX, int mouseY) noexcept;
	void onKeyboardUp(unsigned char key, int mouseX, int mouseY) noexcept;
	void onKeyboardSpecialDown(int key, int mouseX, int mouseY) noexcept;
	void onKeyboardSpecialUp(int key, int mouseX, int mouseY) noexcept;
	void onMouse(int button, int state, int mouseX, int mouseY) noexcept;
	void onMouseMove(int mouseX, int mouseY) noexcept;
	void onMouseDrag(int mouseX, int mouseY) noexcept;

	ivec2 const& getMousePos() noexcept; // gets mouse position in screen space (origin top-left)
	ivec2 const& getMouseDeltaPos() noexcept;
	vec2 const& getMouseViewPos() noexcept;

	InputState getKeyboardState(unsigned char key);
	InputState getSpecialState(int key);
	InputState getMouseButtonState(int button);

	bool isKeyboardKeyDown(unsigned char key);
	bool isSpecialKeyDown(int key);
	bool isMouseButtonDown(int button);

	void update();
}
