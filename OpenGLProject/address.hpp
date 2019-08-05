/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	address.hpp
**
**	Summary		:	Network IP address wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <WinSock2.h>
#include <string>



class AddressInfo
{
public:
	AddressInfo() = default;
	AddressInfo(sockaddr_in const& address);
	AddressInfo(unsigned short portNumber, std::string const& inet = "127.0.0.1");

	bool operator==(AddressInfo const& that) const { return this->m_String == that.m_String; }
	bool operator!=(AddressInfo const& that) const { return !(*this == that); }
	bool operator<(AddressInfo const& that) const { return this->m_String < that.m_String; }

	sockaddr_in const& GetSocketAddress() const { return m_SocketAddress; }
	std::string const& GetString() const { return m_String; }

private:
	sockaddr_in m_SocketAddress{};
	std::string m_String{};
};



std::string ToString(sockaddr_in const& address);
