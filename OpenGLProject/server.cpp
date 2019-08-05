
#include "console.hpp"
#include "port_constants.hpp"
#include "network_message_type.hpp"
#include "network_serialization.hpp"
#include "wsa_error.hpp"
#include "print_container.hpp"
#include "lambda.hpp"

#include "server.hpp"



Server::~Server()
{
	console::log("Server Network Entity is being destroyed.");
}



Server::Server(std::string const& servername)
	: NetworkEntity{ DEFAULT_SERVER_PORT },
	servername{ servername }
{
	console::log("Server Network Entity is starting...");
}



void Server::updateReceive()
{
	try
	{
		receivedPacketQueue.push(socket.ReceiveFrom(MAX_PACKET_LENGTH));
	}
	catch (WsaError const& e)
	{
		if (e.code == WSAECONNRESET)
		{
			console::error("Server> Error: Could not receive data from client. The client has dropped.");
		}
		else
		{
			throw;
		}
	}
}



void Server::updateKeepAlive()
{
	lock_guard lock{ clientsMutex };

	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	sendToAllClients(serialize(NetworkMessageType::StC_KeepAlive_Query));

	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	static constexpr std::chrono::milliseconds const timeout{ 100 };

	// Gather list of dead clients
	std::vector<std::pair<AddressInfo, std::string>> clientsToRemove;

	// For each client
	for (auto const&[address, client] : clients)
	{
		// If client has not responded in a long time
		if (std::chrono::system_clock::now() - client.lastAlive > timeout)
		{
			// Client is dead
			clientsToRemove.push_back(std::make_pair(address, client.username));
			socket.SendTo(address, serializeLink(NetworkMessageType::StC_YourClientKicked, "Your client was unresponsive. KeepAlive timed out."));
		}
	}

	// Delete dead clients
	for (auto const&[address, username] : clientsToRemove)
	{
		clients.erase(address);
	}

	// Tell other alive clients that the dead clients have left
	for (auto const&[address, username] : clientsToRemove)
	{
		console::info("Server> Client has timed out. Username: " + username);
		sendToAllClients(serializeLink(NetworkMessageType::StC_ClientLeft, username));
	}
}



void Server::process(AddressedPacket&& packetReceived)
{
	lock_guard lock{ clientsMutex };

	auto&[fromAddress, packet] = packetReceived;

	auto const clientIter = clients.find(fromAddress);
	bool const isClientConnected = clientIter != clients.end();

	NetworkMessageType const type = extractNmt(packet);

	// If the client is not in set of connected clients...
	if (!isClientConnected
		&& type != NetworkMessageType::CtS_HandshakeRequest
		&& type != NetworkMessageType::CtS_BroadcastToDetectServer)
	{
		console::warning("Server> Warning: Message received from Unknown Client at " + fromAddress.GetString());
		return;
	}

	switch (type)
	{
	case NetworkMessageType::CtS_BroadcastToDetectServer:
	{
		console::log("Server> Received broadcast for detecting server from client at " + fromAddress.GetString());

		socket.SendTo(fromAddress, serialize(NetworkMessageType::StC_BroadcastToDetectServer_Response));

		break;
	}
	case NetworkMessageType::CtS_HandshakeRequest:
	{
		std::string clientUsername = extractString(packet);
		console::log("Server> Received handshake request. Username: " + clientUsername);

		std::string usernameList;

		// if source address already exists in clients
		// and usernames do not match
		if (isClientConnected
			&& clientIter->second.username != clientUsername)
		{
			constexpr auto const reason = "username does not match previous session";

			console::log(stringLink("Server> Handshake failure: ", reason));

			// Respond that the handshake failed.
			socket.SendTo(fromAddress, serializeLink(
				NetworkMessageType::StC_HandshakeResponse_Failure,
				reason));

			return;
		}

		// else if username is already used by a different client
		auto const itUsr = std::find_if(clients.begin(), clients.end(), [&](auto const& pair) {
			return pair.second.username == clientUsername;
		});
		if (itUsr != clients.end())
		{
			constexpr auto const reason = "username already in use";

			console::log(stringLink("Server> Handshake failure: ", reason));

			// Respond that the handshake failed.
			socket.SendTo(fromAddress, serializeLink(
				NetworkMessageType::StC_HandshakeResponse_Failure,
				reason));

			return;
		}
		// else...

		// Add to list of connected clients
		clients.emplace(std::pair<AddressInfo, ClientInfo>{
			fromAddress,
				ClientInfo{ clientUsername }
		});

		console::log("Server> Handshake success. Client has joined. Username: " + clientUsername);

		// Accumulate a list of client usernames.
		std::ostringstream s;
		container::printAs(s, clients, LAM_PRINT(outs, pair, outs << pair.second.username), "", "");
		usernameList = s.str();

		// Send handshake response with status 'success'.
		socket.SendTo(fromAddress, serializeLink(NetworkMessageType::StC_HandshakeResponse_Success, usernameList));

		// Tell other clients that the user has joined.
		sendToAllClients(serializeLink(NetworkMessageType::StC_ChatMessage_UserJoined, clientUsername));

		break;
	}
	case NetworkMessageType::CtS_ChatMessage:
	{
		auto const& clientUsername = clientIter->second.username;
		auto const content = extractString(packet);
		auto const msg = clientUsername + "> " + content;
		console::chat(msg);
		sendToAllClients(serializeLink(NetworkMessageType::StC_ChatMessage, msg));
		break;
	}
	case NetworkMessageType::CtS_KeepAlive_Response:
	{
		clientIter->second.lastAlive = std::chrono::system_clock::now();
		break;
	}
	case NetworkMessageType::CtS_DroppedConnection:
	{
		// Copy username
		std::string const clientUsername = clientIter->second.username;

		// Erase from list of clients
		clients.erase(clientIter);

		// Tell other clients
		console::info("Server> Client has left the server: " + clientUsername);
		sendToAllClients(serializeLink(NetworkMessageType::StC_ClientLeft, clientUsername));

		break;
	}
	default:
		throw std::runtime_error{ "Server> Invalid deserialized NetworkMessageType enum value" };
	}
}



void Server::sendToAllClients(Packet const& packet)
{
	for (auto const& [address, client] : clients)
	{
		socket.SendTo(address, packet);
	}
}
