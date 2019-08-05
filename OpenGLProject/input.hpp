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
ReturnCode getKeyboardState(InputState* out, unsigned char key) noexcept;
ReturnCode getSpecialState(InputState* out, int key) noexcept;
ReturnCode getMouseButtonState(InputState* out, int button) noexcept;
InputState getKeyboardState(unsigned char key) noexcept;
InputState getSpecialState(int key) noexcept;
InputState getMouseButtonState(int button) noexcept;

bool isKeyboardKeyDown(unsigned char key);
bool isSpecialKeyDown(int key);
bool isMouseButtonDown(int button);

void updateInput();
