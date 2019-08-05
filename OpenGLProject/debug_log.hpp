/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	debug_print.hpp
**
**	Summary		:	Functions for printing messages when manually debugging.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#ifdef _DEBUG

//#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "to_string.hpp"
#include "console.hpp"



namespace console
{
	inline void debugLog(std::string const& message)
	{
		enqueMessage(ConsoleMessageType::DebugLog, message);
	}
}



#define DEBUG_LOG(...) ::console::debugLog(__VA_ARGS__)
#define DEBUG_LOG_LINK(...) DEBUG_LOG(stringLink(__VA_ARGS__))
//#define DEBUG_PRINT_LN(...) std::clog << stringLink(__VA_ARGS__)

#else //! _DEBUG

#define DEBUG_LOG(...)
#define DEBUG_LOG_LINK(...)
//#define DEBUG_PRINT_LN(...)

#endif //~ _DEBUG
