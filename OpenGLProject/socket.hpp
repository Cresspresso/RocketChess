/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	socket.hpp
**
**	Summary		:	Network Socket wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

// Library Includes
#include <WinSock2.h>
#include <string>
#include <vector>
#include <optional>
#include <memory>

// Local Includes
#include "packet.hpp"



// This Class
class Socket
{
public:

	// Lifetime Methods

	/*
	**	This function call does the act of creating a socket and binding it to the supplied port number on the local machine.
	*/
	void Open(unsigned short portNumber);

	void Close();

	std::pair<sockaddr_in, Packet> ReceiveFromRaw(int MaxBytesToReceive = MAX_PACKET_LENGTH);
	AddressedPacket ReceiveFrom(int MaxBytesToReceive = MAX_PACKET_LENGTH);

	// Receives packets until a timeout.
	std::vector<AddressedPacket> ReceiveMany(int seconds = 10, int MaxBytesToReceive = MAX_PACKET_LENGTH);


	void SendToRaw(sockaddr_in const& address, Packet const& data);
	void SendTo(AddressInfo const& address, Packet const& data);
	void SendTo(AddressedPacket const& packet);


	void EnableBroadcast();
	void DisableBroadcast();


	~Socket();
	Socket() { ZeroMemory(&m_SocketAddress, sizeof(m_SocketAddress)); }
	Socket(Socket const&) = delete;
	Socket(Socket&&) = delete;
	Socket& operator=(Socket const&) = delete;
	Socket& operator=(Socket&&) = delete;



	// Accessor Methods

	bool IsOpen() const { return m_Socket != NULL; }
	SOCKET GetSocketHandle() { return m_Socket; }

	AddressInfo GetAddress() const { return AddressInfo{ m_PortNumber }; }
	sockaddr_in const& GetSocketAddress() const { return m_SocketAddress; }
	unsigned short GetPortNumber() const { return m_PortNumber; }
	std::string const& GetLocalAddress() const { return m_LocalAddress; }



private:

	// Methods

	/*
	**	Get the local IP Address to which the socket is bound to.
	**	Used for printing the local address in the console window.
	*/
	std::string GatherLocalAddress();



	// Fields

	/*
	**	A handle to as of yet unbound socket.
	*/
	SOCKET m_Socket = NULL;

	/*
	**	A sockaddr structure containing the local address and port to bind the socket to.
	*/
	sockaddr_in m_SocketAddress;



	unsigned short m_PortNumber = 0;
	std::string m_LocalAddress;



};

//extern std::unique_ptr<Socket> g_Socket;
