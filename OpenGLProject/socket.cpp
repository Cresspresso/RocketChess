/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	socket.cpp
**
**	Summary		:	Network Socket wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

// Library Inludes
#include <array>
#include <iostream>
#include <WS2tcpip.h>

// Local Includes
#include "wsa_error.hpp"
#include "final_act.hpp"

#include "console.hpp"
#include "to_string.hpp"

// This Include
#include "socket.hpp"



namespace
{
	static inline void CheckIsOpen(Socket& socket)
	{
		if (!socket.IsOpen())
		{
			throw std::logic_error{ "socket is not open" };
		}
	}
}



//std::unique_ptr<Socket> g_Socket = nullptr; // TODO initialise



// Implicit Assumption: We are creating only UDP sockets.....
void Socket::Open(unsigned short portNumber)
{
	if (IsOpen())
	{
		throw std::logic_error{ "socket is already open" };
	}

	ZeroMemory(&m_SocketAddress, sizeof(m_SocketAddress));

	// Create a UDP socket.
	m_Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == m_Socket)
	{
		throw WsaError{};
	}

	// now bind the socket to the address and port number supplied.
	m_SocketAddress.sin_family = AF_INET;
	m_SocketAddress.sin_port = htons(portNumber);
	m_SocketAddress.sin_addr.S_un.S_addr = INADDR_ANY;

	while (0 != bind(m_Socket, reinterpret_cast<sockaddr*>(&m_SocketAddress), sizeof(m_SocketAddress)))
	{
		// Cannot bind to the specified IP and port;
		// Try with an incremented port number.
		++portNumber; // WARNING unchecked overflow - loop might not terminate

		m_SocketAddress.sin_port = htons(portNumber);
	}

	m_PortNumber = portNumber;
	m_LocalAddress = GatherLocalAddress();

	console::info(stringLink("Opened socket on ", m_LocalAddress, ':', m_PortNumber));
}

void Socket::Close()
{
	CheckIsOpen(*this);
	CheckWsaError(closesocket(m_Socket));
	m_Socket = NULL;
}

std::pair<sockaddr_in, Packet> Socket::ReceiveFromRaw(int MaxBytesToReceive)
{
	CheckIsOpen(*this);

	// Make a thread local buffer to receive data into.
	Packet packet(MaxBytesToReceive);

	// pull off the packet(s) using recvfrom()
	sockaddr_in address;
	int SizeOfAddress = sizeof(address);
	int const BytesReceived = recvfrom(				// pulls a packet from a single source...
		m_Socket,									// client-end socket being used to read from
		reinterpret_cast<char*>(packet.data()),		// incoming packet to be filled
		MaxBytesToReceive,							// length of incoming packet to be filled
		0,											// flags
		reinterpret_cast<sockaddr*>(&address),		// address to be filled with packet source
		&SizeOfAddress								// size of the above address struct.
	);
	if (BytesReceived <= 0)
	{
		throw WsaError{};
	}
	packet.erase(packet.begin() + BytesReceived, packet.end());

	return std::pair<sockaddr_in, Packet>{ address, packet };
}

AddressedPacket Socket::ReceiveFrom(int MaxBytesToReceive)
{
	auto const [socketAddr, data] = ReceiveFromRaw(MaxBytesToReceive);
	return { AddressInfo{ socketAddr }, data };
}

std::vector<AddressedPacket> Socket::ReceiveMany(int seconds, int MaxBytesToReceive)
{
	CheckIsOpen(*this);

	SOCKET const socket = m_Socket;

	// Enable timeout
	timeval timeValue;
	timeValue.tv_sec = seconds;
	timeValue.tv_usec = 0;
	setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char const*)&timeValue, sizeof(timeValue));

	// At end of function, disable timeout
	FINAL_ACT(_, [socket]() noexcept {
		timeval timeValue;
		timeValue.tv_sec = 0;
		timeValue.tv_usec = 0;
		setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char const*)&timeValue, sizeof(timeValue));
	});

	std::vector<AddressedPacket> packets;
	for (;;)
	{
		try
		{
			packets.push_back(ReceiveFrom(MaxBytesToReceive));
		}
		catch (WsaError const& e)
		{
			if (e.code == WSAETIMEDOUT)
			{
				break;
			}
			else
			{
				throw;
			}
		}
	}
	return packets;
}

void Socket::SendToRaw(sockaddr_in const& address, Packet const& packet)
{
	CheckIsOpen(*this);

	if (packet.size() > static_cast<size_t>(INT_MAX))
	{
		throw std::invalid_argument{ "packet data size is too large" };
	}

	int const BytesToSend = static_cast<int>(packet.size());
	int const BytesSent = sendto(
		m_Socket,										// socket to send through.
		reinterpret_cast<char const*>(packet.data()),	// data to send
		BytesToSend,									// number of bytes to send
		0,												// flags
		reinterpret_cast<sockaddr const*>(&address),	// address to be filled with packet target
		sizeof(address)									// size of the above address struct.
	);
	if (BytesSent != BytesToSend)
	{
		throw WsaError{};
	}
}

void Socket::SendTo(AddressInfo const& address, Packet const& data)
{
	SendToRaw(address.GetSocketAddress(), data);
}

void Socket::SendTo(AddressedPacket const& packet)
{
	auto const& [address, data] = packet;
	SendTo(address, data);
}

void Socket::EnableBroadcast()
{
	int const _iBroadCastOption = 1;
	int const _iResult = setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST,
		reinterpret_cast<char const*>(&_iBroadCastOption), sizeof(_iBroadCastOption));
	if (_iResult == SOCKET_ERROR)
	{
		throw WsaError{};
	}
}

void Socket::DisableBroadcast()
{
	int const _iBroadCastOption = 0;
	int const _iResult = setsockopt(m_Socket, SOL_SOCKET, SO_BROADCAST,
		reinterpret_cast<char const*>(&_iBroadCastOption), sizeof(_iBroadCastOption));
	if (_iResult == SOCKET_ERROR)
	{
		throw WsaError{};
	}
}

Socket::~Socket()
{
	if (m_Socket != NULL)
	{
		closesocket(m_Socket);
	}
}



std::string Socket::GatherLocalAddress()
{
	CheckIsOpen(*this);

	static constexpr int const len = 256;
	std::array<char, len> LocalHostName;
	CheckWsaError(gethostname(LocalHostName.data(), len));

	addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	addrinfo* result;
	CheckWsaError(getaddrinfo(LocalHostName.data(), nullptr, &hints, &result));

	/* loop over all returned results to get the address */
	addrinfo* p = result;
	while (p->ai_next != nullptr)
	{
		p = p->ai_next;
	}
	sockaddr_in* LocalIP = reinterpret_cast<sockaddr_in*>(p->ai_addr);

	std::array<char, 256> LocalAddress;
	if (nullptr == inet_ntop(AF_INET, &(LocalIP->sin_addr), LocalAddress.data(), LocalAddress.size()))
	{
		throw WsaError{};
	}

	return std::string{ LocalAddress.data() };
}
