/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	console_color.cpp
**
**	Summary		:	Utilities for colouring console text.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	18/06/2019
*/

#include "console_color.hpp"

void setConsoleTextColor(ConsoleForegroundColor color)
{
	setConsoleTextColor(static_cast<WORD>(color));
}

void setConsoleTextColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
