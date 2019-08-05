/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	handle_exception.cpp
**
**	Summary		:	Exception handling functions for the networking module.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#include <sstream>
#include <exception>
#include <Kia/include/cress_kia__print_exception.hpp>

#include "wsa_error.hpp"
#include "network_entity.hpp"

#include "handle_exception.hpp"

std::string GetStringFromException()
{
	std::ostringstream s;
	try
	{
		throw;
	}
	catch (WsaError const& e)
	{
		s << "WsaError: " << e.what() << ":\n";
		s << GetWSAErrorInfoMessage(e.code);
	}
	catch (...)
	{
		s << cress::kia::printExceptionF;
	}
	return s.str();
}



void HandleException(INetworkEntity& entity)
{
	entity.AddMessage({ MessageType::Error, GetStringFromException() });
}

#endif //~ COMMENT