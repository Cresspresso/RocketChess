/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	console_color.hpp
**
**	Summary		:	Utilities for colouring console text.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	18/06/2019
*/

#include <Windows.h>

enum class ConsoleForegroundColor : WORD
{
	Black = 0,
	DarkBlue = 1,
	DarkGreen = 2,
	DarkCyan = 3,
	DarkRed = 4,
	DarkMagenta = 5,
	DarkYellow = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 12,
	LightYellow = 14,
	White = 15,
};

void setConsoleTextColor(ConsoleForegroundColor color = ConsoleForegroundColor::LightGray);
void setConsoleTextColor(WORD color);
