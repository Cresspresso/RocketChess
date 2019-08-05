/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	console_tools.hpp
**
**	Summary		:	Tools for console input and output.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <iostream>
#include <mutex>
#include <Windows.h>
#include <ctype.h>
#include <conio.h>

inline std::mutex g_ConsoleMutex;

char* GetLineFromConsole(char* pBuffer, int iNumChars);

template <size_t iNumChars>
inline char* GetLineFromConsole(char(&pBuffer)[iNumChars])
{
	return GetLineFromConsole(pBuffer, (int)iNumChars);
}

char QueryOption(const char* Question, const char* Accepted, bool bCaseSensitive = false);

char* CollapseBackspacesAndCleanInput(char* pBuffer);

unsigned short QueryPortNumber(unsigned short uDefault = 0);

void UtilsSetTextColor(WORD color = 12);

#endif //~ COMMENT