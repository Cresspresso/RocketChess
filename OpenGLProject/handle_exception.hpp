/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	handle_exception.hpp
**
**	Summary		:	Exception handling functions for the networking module.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <string>

std::string GetStringFromException();


class INetworkEntity;
void HandleException(INetworkEntity& entity);

#endif //~ COMMENT