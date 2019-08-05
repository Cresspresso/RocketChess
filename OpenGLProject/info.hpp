/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	info.hpp
**
**	Summary		:	Information about a client.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <string>
#include <chrono>

#include "address.hpp"


struct ClientBasicInfo
{
	std::string Username;
	typename std::chrono::system_clock::time_point LastAlive = std::chrono::system_clock::now();
};

//struct ServerBasicInfo
//{
//
//};

#endif //~ COMMENT