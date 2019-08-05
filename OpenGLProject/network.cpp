/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	network.cpp
**
**	Summary		:	Network wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <WinSock2.h>

#include "wsa_error.hpp"

#include "network.hpp"



std::unique_ptr<Network> g_Network = nullptr;



void Network::Startup()
{
	WSADATA wsaData;
	CheckWsaError(WSAStartup(0x0202, &wsaData));
	m_IsOnline = true;
}

void Network::Cleanup()
{
	CheckWsaError(WSACleanup());
	m_IsOnline = false;
}

Network::~Network()
{
	WSACleanup();
}
