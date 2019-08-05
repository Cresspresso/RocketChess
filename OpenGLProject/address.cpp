/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	address.cpp
**
**	Summary		:	Network IP address wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <array>
#include <sstream>
#include <WS2tcpip.h>

#include "wsa_error.hpp"

#include "address.hpp"



AddressInfo::AddressInfo(sockaddr_in const& address) :
	m_SocketAddress{ address },
	m_String{ ToString(address) }
{}



AddressInfo::AddressInfo(unsigned short portNumber, std::string const& inet)
	: m_String{ inet + ':' + std::to_string(portNumber) }
{
	ZeroMemory(&m_SocketAddress, sizeof(m_SocketAddress));
	m_SocketAddress.sin_family = AF_INET;
	m_SocketAddress.sin_port = htons(portNumber);
	inet_pton(AF_INET, inet.c_str(), &m_SocketAddress.sin_addr);
}



std::string ToString(sockaddr_in const& address)
{
	//INET_ADDRSTRLEN - maximum length for IPv4 addresses
	std::array<char, INET_ADDRSTRLEN> AddressBuffer;
	if (nullptr == inet_ntop(AF_INET, &address.sin_addr, AddressBuffer.data(), INET_ADDRSTRLEN))
	{
		throw WsaError{};
	}

	std::ostringstream s;
	s << AddressBuffer.data()
		<< ':'
		<< std::to_string(ntohs(address.sin_port));

	return s.str();
}
