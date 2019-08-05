/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	server.cpp
**
**	Summary		:	Server network entity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#include <stdexcept>
#include <iostream>
#include <sstream>

#include "socket.hpp"
#include "message.hpp"
#include "handle_exception.hpp"
#include "print_container.hpp"
#include "lambda.hpp"
#include "wsa_error.hpp"

#include "server.hpp"



void Server::SendToAll(PacketData const& packet)
{
	for (auto const& [clientAddr, clientInfo] : m_Clients)
	{
		g_Socket.SendTo(clientAddr.GetSocketAddress(), packet);
	}
}



void Server::Initialise()
{
	this->AddMessage({ MessageType::Log, "Server> Server is up." });
}

void Server::UpdateReceive()
{
	try {
		auto const[socketAddr, packet] = g_Socket.ReceiveFrom(MAX_PACKET_LENGTH);
		m_WorkQueue.push({ AddressInfo{ socketAddr }, packet });
	}
	catch (WsaError const& e)
	{
		if (e.code == WSAECONNRESET)
		{
			this->AddMessage({ MessageType::Error, "Server> Error: Could not receive data from client. The client has dropped." });
		}
		else
		{
			throw;
		}
	}
}



void Server::UpdateProcess()
{
	auto optPacket = m_WorkQueue.tryPop();
	if (optPacket)
	{
		Process(*optPacket);
	}

	PrintMessages();
}


void Server::UpdateKeepAlive()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	SendToAll(Serialize(ServerToClientMessageType::KeepAlive_Query, ""));

	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	static constexpr std::chrono::milliseconds const timeout{ 100 };

	// Gather list of dead clients
	std::vector<AddressInfo> clientsToRemove;
	std::vector<std::string> usernames;

	// For each client
	for (auto const& [addr, info] : m_Clients)
	{
		// If client has not responded in a long time
		if (std::chrono::system_clock::now() - info.LastAlive > timeout)
		{
			// Client is dead
			clientsToRemove.push_back(addr);
			usernames.push_back(info.Username);
			g_Socket.SendTo(addr, Serialize(ServerToClientMessageType::YourClientKicked, "KeepAlive timed out"));
		}
	}

	// Delete dead clients
	for (AddressInfo const& key : clientsToRemove)
	{
		m_Clients.erase(key);
	}

	// Tell other alive clients that the dead clients have left
	for (std::string const& username : usernames)
	{
		AddMessage({ MessageType::Info, "Server> Client keep alive has timed out. Client has dropped: " + username });
		SendToAll(Serialize(ServerToClientMessageType::ClientLeft, username));
	}
}



void Server::Process(Packet const& packet)
{
	auto const& [sourceAddr, data] = packet;

	/* Returns true if the client has joined to this server. */
	auto const isClientJoined = [this](AddressInfo const& addr)
	{
		/*auto const it = std::find_if(m_Clients.begin(), m_Clients.end(),
			[&](auto const& pair)
		{
			return pair.first == addr;
		});*/
		return m_Clients.find(addr) != m_Clients.end();
	};

	try
	{
		auto const [type, content] = Deserialize<ClientToServerMessageType>(data);

		auto const clientIter = m_Clients.find(sourceAddr);

		// If the client is not in set of connected clients...
		if (clientIter == m_Clients.end()
			&& type != ClientToServerMessageType::HandshakeRequest
			&& type != ClientToServerMessageType::BroadcastToDetectServer)
		{
			this->AddMessage({ MessageType::Warning, "Server> Warning: Message from Unknown Client: " + content });
			return;
		}

		switch (type)
		{
		case ClientToServerMessageType::BroadcastToDetectServer:
		{
			this->AddMessage({ MessageType::Log, "Server> Received broadcast for detecting server: " + content });

			g_Socket.SendTo(
				sourceAddr,
				Serialize(
					ServerToClientMessageType::BroadcastToDetectServer_Response,
					g_Socket.GetLocalAddress()
				)
			);

			break;
		}
		case ClientToServerMessageType::HandshakeRequest:
		{
			this->AddMessage({ MessageType::Log, "Server> Received handshake request: " + content });

			// immediately invoked lambda
			[&] {
				std::string const& username = content;

				// if source address already exists in clients
				// and usernames do not match
				if (clientIter != m_Clients.end()
					&& clientIter->second.Username != username)
				{
					this->AddMessage({
						MessageType::Log,
						"Server> Handshake failure: username does not match previous session."
						});

					// Respond that the handshake failed.
					g_Socket.SendTo(sourceAddr, Serialize(
						ServerToClientMessageType::HandshakeResponse_Failure,
						"username does not match previous session")
					);

					return;
				}

				// else if username is already used by a different client
				auto const itUsr = std::find_if(m_Clients.begin(), m_Clients.end(), [&](auto const& pair) {
					return pair.second.Username == username;
				});
				if (itUsr != m_Clients.end())
				{
					this->AddMessage({
						MessageType::Log,
						"Server> Handshake failure: username already in use."
						});

					// Respond that the handshake failed.
					g_Socket.SendTo(sourceAddr, Serialize(
						ServerToClientMessageType::HandshakeResponse_Failure,
						"username already in use")
					);
					return;
				}
				// else...

				// Add to list of connected clients
				this->m_Clients.emplace(std::pair<AddressInfo, ClientBasicInfo>{
					sourceAddr,
					ClientBasicInfo{ username }
				});

				this->AddMessage({
					MessageType::Log,
					"Server> Handshake success: " + username
					});

				// Accumulate a list of client usernames.
				std::ostringstream s;
				container::printAs(s, m_Clients, LAM_PRINT(outs, pair, outs << pair.second.Username), "", "");

				// Send handshake response with status 'success'.
				g_Socket.SendTo(
					sourceAddr,
					Serialize(ServerToClientMessageType::HandshakeResponse_Success, s.str())
				);

				// Tell other clients that the user has joined.
				this->SendToAll(Serialize(ServerToClientMessageType::ChatMessage_UserJoined, username));
			}();

			break;
		}
		case ClientToServerMessageType::ChatMessage_FromClient:
		{
			auto const& username = clientIter->second.Username;
			auto const msg = username + "> " + content;
			this->AddMessage({ MessageType::Chat, msg });
			this->SendToAll(Serialize(ServerToClientMessageType::ChatMessage_ToClient, msg));
			break;
		}
		case ClientToServerMessageType::KeepAlive_Response:
		{
			clientIter->second.LastAlive = std::chrono::system_clock::now();
			break;
		}
		case ClientToServerMessageType::ClientQuit:
		{
			// Copy username
			std::string const username = clientIter->second.Username;

			// Erase from list of clients
			m_Clients.erase(clientIter);

			// Tell other clients
			AddMessage({ MessageType::Info, "Server> Client has quit the server: " + username });
			SendToAll(Serialize(ServerToClientMessageType::ClientLeft, username));

			break;
		}
		default:
			throw std::runtime_error{ "invalid deserialized ClientToServerMessageType" };
		}
	}
	catch (...)
	{
		HandleException(*this);
	}
}

#endif //~ COMMENT