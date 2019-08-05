/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	client.cpp
**
**	Summary		:	Client network entity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#include <stdexcept>
#include <iostream>
#include <WS2tcpip.h>

#include "socket.hpp"
#include "message.hpp"
#include "console_tools.hpp"
#include "wsa_error.hpp"
#include "handle_exception.hpp"
#include "final_act.hpp"

#include "client.hpp"



void Client::Initialise()
{
	bool serverChosen = false;
	while (!serverChosen)
	{
		switch (QueryOption("Do you want to broadcast for servers or manually connect (B/M)?", "BM"))
		{
		case 'B': // broadcast to detect servers and pick a server.
		{
			//Question 7: Broadcast to detect server
			g_Socket.EnableBroadcast();
			FinalAct _{ []()noexcept { g_Socket.DisableBroadcast(); } };

			BroadcastForServers();

			// Wait for a time.
			std::vector<Packet> const packets = g_Socket.ReceiveMany(10, MAX_PACKET_LENGTH);

			std::vector<std::pair<ServerToClientMessageType, std::string>>
				messages(packets.size());

			// Deserialize messages to make sure they are the correct response type of message.
			size_t numValid = 0;
			for (size_t i = 0; i < packets.size(); ++i)
			{
				auto const&[sourceAddr, data] = packets[i];
				messages[i] = Deserialize<ServerToClientMessageType>(data);
				auto const&[type, content] = messages[i];
				if (type != ServerToClientMessageType::BroadcastToDetectServer_Response)
				{
					continue;
				}

				++numValid;
				std::cout << "[" << i << "] Server found at : " << sourceAddr.GetString()
					<< " : \"" << content << "\"\n";
			}

			// Print servers found.
			if (0 == numValid)
			{
				std::cout << "No Servers Found.\n";
			}
			else
			{
				std::cout << "Choose a server number to connect to (-1 to cancel): ";
				int i;
				// While input failed, or input is: not -1, or not in range, or the server response was incorrect. 
				while (!(std::cin >> i)
					|| (i != -1 && (
						i < 0
						|| i >= packets.size()
						|| messages[i].first != ServerToClientMessageType::BroadcastToDetectServer_Response
						)
					))
				{
					std::cout << "Invalid choice.\n";
					std::cout << "Choose a server number to connect to (-1 to cancel): ";
				}
				if (i != -1)
				{
					m_ServerAddress = packets[i].first;
					serverChosen = true;
				}
			}

			break;
		}
		case 'M': // enter server address manually.
		{
			std::cout << "Enter server IP or empty for localhost: ";

			char serverIPAddress[128];
			ZeroMemory(serverIPAddress, sizeof(serverIPAddress));
			gets_s(serverIPAddress);
			if (serverIPAddress[0] == '\0')
			{
				strcpy_s(serverIPAddress, "127.0.0.1");
			}

			std::cout << "Enter server's port number or empty for default server port: ";
			char serverPort[128];
			ZeroMemory(serverPort, sizeof(serverPort));
			gets_s(serverPort);
			unsigned short port = (serverPort[0] == '\0')
				? DEFAULT_SERVER_PORT
				: static_cast<unsigned short>(std::atoi(serverPort));

			sockaddr_in addr;
			ZeroMemory(&addr, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			inet_pton(AF_INET, serverIPAddress, &addr.sin_addr);
			m_ServerAddress = { addr };

			serverChosen = true;
			break;
		}
		default:
			throw "invalid option";
			break;
		}
	}

	std::cout << "Chosen address: " << m_ServerAddress.GetString() << '\n';

	std::cout << "Enter a username: ";
	std::string username;
	std::cin >> username;

	m_Info.Username = username;

	this->AddMessage({ MessageType::Log, "Client> Client is up." });

	this->AddMessage({ MessageType::Log, "Client> Sending Handshake Request..." });
	g_Socket.SendTo(m_ServerAddress, Serialize(ClientToServerMessageType::HandshakeRequest, username));
}



void Client::BroadcastForServers()
{
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	
	// Just try a series of 10 ports to detect a running server.
	// This is needed since we are testing multiple servers on the same local machine.
	for (int i = 0; i < 10; ++i)
	{
		address.sin_port = htons(DEFAULT_SERVER_PORT + i);
		g_Socket.SendTo(
			AddressInfo{ address },
			Serialize(ClientToServerMessageType::BroadcastToDetectServer, "Broadcast to Detect Server")
		);
	}
}



void Client::UpdateReceive()
{
	try {
		Packet const packet = g_Socket.ReceiveFrom(MAX_PACKET_LENGTH);
		if (packet.first == m_ServerAddress)
		{
			m_WorkQueue.push(packet);
		}
	}
	catch (WsaError const& e)
	{
		if (e.code == WSAECONNRESET)
		{
			this->AddMessage({ MessageType::Error, "Client> Error: Could not receive data from server. The server has dropped." });

			g_Socket.SendTo(m_ServerAddress, Serialize(ClientToServerMessageType::ClientQuit, "server dropped"));
			// Stop the client
			m_IsRunning = false;
		}
		else
		{
			throw;
		}
	}
}



void Client::UpdateProcess()
{
	bool const entered = false;//UserInput();
	if (!entered)
	{
		auto optPacket = m_WorkQueue.tryPop();
		if (optPacket)
		{
			Process(*optPacket);
		}
	}

	PrintMessages();
}



void Client::UpdateKeepAlive()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	g_Socket.SendTo(m_ServerAddress, Serialize(ClientToServerMessageType::KeepAlive_Response, ""));
}



//bool Client::UserInput()
//{
//	// If user finished typing a message and enters a newline...
//	bool entered = m_InputBuffer.Update();
//	if (entered)
//	{
//		// Get the chat message from the buffer.
//		std::string content = m_InputBuffer.GetString();
//
//		// If the message is not empty whitespace...
//		if (std::any_of(content.begin(), content.end(), [](char c) { return !isspace(c); }))
//		{
//			// Handle Commands
//			bool absorbed = false;
//			if (content.size() > 0 && content[0] == '!')
//			{
//				absorbed = true;
//				if (content.size() < 2)
//				{
//					AddMessage({ MessageType::Error, "Client> Error: Command not recognised: " + content
//						+ ". Enter '!!' for a chat message \"!\"." });
//				}
//				else
//				{
//					switch (content[1])
//					{
//					case 'q':
//					{
//						AddMessage({ MessageType::Info, "Client> Closing the client." });
//						g_Socket.SendTo(m_ServerAddress, Serialize(ClientToServerMessageType::ClientQuit, "Quit Command"));
//						// Stop the client.
//						g_isRunning = false;
//						break;
//					}
//
//					case '!':
//						absorbed = false;
//						content = content.substr(1);
//						break;
//
//					default:
//					{
//						AddMessage({ MessageType::Error, "Client> Error: Command not recognised: " + content });
//						break;
//					}
//					}
//				}
//			}
//
//			// If not a command (just a regular chat message)...
//			if (!absorbed)
//			{
//				//this->AddMessage({ MessageType::Log, "Client> Sent chat message: " + content });
//
//				// Send the chat message to the server.
//				PacketData const data = Serialize(
//					ClientToServerMessageType::ChatMessage_FromClient,
//					content
//				);
//				// Send it to the server.
//				g_Socket.SendTo(m_ServerAddress, data);
//			}
//		}
//
//		m_InputBuffer.Clear();
//	}
//
//	m_InputBuffer.PrintToScreenTop();
//
//	return entered;
//}



void Client::Process(Packet const& packet)
{
	auto const& [sourceAddr, data] = packet;

	try
	{
		auto const [type, content] = Deserialize<ServerToClientMessageType>(data);

		if (sourceAddr != m_ServerAddress)
		{
			this->AddMessage({ MessageType::Warning, "Client> Warning: Message from Unknown Server: " + content });
			return;
		}

		switch (type)
		{
		case ServerToClientMessageType::HandshakeResponse_Failure:
		{
			this->AddMessage({ MessageType::Error, "Client> Error: Handshake Failed: " + content });
			// Stop the client
			m_IsRunning = false;
			break;
		}
		case ServerToClientMessageType::HandshakeResponse_Success:
		{
			this->AddMessage({ MessageType::Log, "Client> Handshake succeeded." });
			this->AddMessage({ MessageType::Info, "Client> Users on this server: " + content });
			break;
		}
		case ServerToClientMessageType::ChatMessage_ToClient:
		{
			this->AddMessage({ MessageType::Chat, content });
			break;
		}
		case ServerToClientMessageType::ChatMessage_UserJoined:
		{
			this->AddMessage({ MessageType::Info, "Client> User has joined the chat: " + content });
			break;
		}
		case ServerToClientMessageType::KeepAlive_Query:
		{
			g_Socket.SendTo(m_ServerAddress, Serialize(ClientToServerMessageType::KeepAlive_Response, ""));
			break;
		}
		case ServerToClientMessageType::ClientLeft:
		{
			this->AddMessage({ MessageType::Info, "Client> User has left the chat: " + content });
			break;
		}
		case ServerToClientMessageType::YourClientKicked:
		{
			this->AddMessage({ MessageType::Error, "Client> You have been kicked from the chat. Reason: " + content });
			// Stop the client
			m_IsRunning = false;
			break;
		}
		case ServerToClientMessageType::BroadcastToDetectServer_Response:
		default:
			throw std::runtime_error{ "invalid deserialized ServerToClientMessageType" };
		}
	}
	catch (...)
	{
		HandleException(*this);
	}
}

#endif //~ COMMENT