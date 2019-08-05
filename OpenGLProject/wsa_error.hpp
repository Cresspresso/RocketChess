/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	wsa_error.hpp
**
**	Summary		:	Exception for WSA error codes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <WinSock2.h>

#include "error_code_exception.hpp"



class WsaError : public ErrorCodeException<int>
{
public:
	WsaError(int code = WSAGetLastError(), std::string const& title = "WsaError")
		: ErrorCodeException{ code, title }
	{}
};



inline void CheckWsaError(int error = WSAGetLastError(), std::string const& title = "WsaError")
{
	if (0 != error) { throw WsaError{ error, title }; }
}



char const* GetWSAErrorInfoMessage(int error);
