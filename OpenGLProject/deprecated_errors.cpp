/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	errors.cpp
**
**	Summary 	:	Error propagating and handling functions.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#if 0
#include <cassert>
#include <iostream>
#include <sstream>

#include "console.hpp"
#include "to_string.hpp"

#include "deprecated_errors.hpp"



void failFast()
{
	assert(0); // for debugging

	std::terminate();
}

std::unique_ptr<std::string> g_reason = nullptr;

std::string stringError(int code)
{
	return stringLink("Error (code ", code, "): ", (g_reason ? *g_reason : "unknown"));
}

std::string stringException()
{
	return stringLink(cress::kia::printExceptionF);
}

void printError(int code)
{
	console::error(stringError(code));
	g_reason->clear();
}

void printException()
{
	console::error(stringException());
}


#endif