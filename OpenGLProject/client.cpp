
#include "console.hpp"
#include "to_string.hpp"
#include "port_constants.hpp"
#include "network_message_type.hpp"
#include "network_serialization.hpp"
#include "final_act.hpp"
#include "wsa_error.hpp"
#include "errors.hpp"
#include "debug_log.hpp"

#include "client.hpp"



namespace
{
	using ConnectionMutex = std::shared_timed_mutex;
	using ReadLock = std::shared_lock<ConnectionMutex>;
	using WriteLock = std::unique_lock<ConnectionMutex>;
}



Client::~Client()
{
	console::log("Client Network Entity is being destroyed.");
	TRY_CATCH_PRINT(dropConnection());
}



Client::Client(std::string const& username)
	: NetworkEntity{ DEFAULT_CLIENT_PORT },
	username{ username }
{
	console::log("Client Network Entity is starting...");
}



void Client::connectTo(AddressInfo const& serverAddress)
{
	WriteLock lock{ connectionMutex };

	dropConnectionInner();

	this->serverAddress = serverAddress;
	connectionState = ConnectionState::Connecting;

	console::log(stringLink("Client> Sending Handshake Request to server at ", serverAddress.GetString()));
	sendToServer(serializeLink(NetworkMessageType::CtS_HandshakeRequest, username));
}



void Client::dropConnection()
{
	WriteLock lock{ connectionMutex };
	dropConnectionInner();
}



void Client::dropConnectionInner()
{
	if (!isConnected())
	{
		return;
	}

	FINAL_ACT_LAM(_, {
		connectionState = ConnectionState::None;
	DEBUG_LOG("Client> Closed.");
		});

	try
	{
		console::log("Client> Closing connection.");
		sendToServer(serialize(NetworkMessageType::CtS_DroppedConnection));
	}
	CATCH_PRINT();
}



void Client::updateReceive()
{
	ReadLock lock{ connectionMutex };
	if (!isConnected())
	{
		return;
	}

	FINAL_ACT_LAM(_, { DEBUG_LOG("Client> - Finished Update Receive."); });

	try
	{
		DEBUG_LOG("Client> + Update Receive...");

		AddressedPacket packet = socket.ReceiveFrom(MAX_PACKET_LENGTH);
		if (packet.first == serverAddress)
		{
			receivedPacketQueue.push(std::move(packet));
		}
	}
	catch (WsaError const& e)
	{
		if (e.code == WSAECONNRESET)
		{
			console::error("Client> Error: Could not receive data from server. The server has dropped.");

			WriteLock lock{ connectionMutex, std::adopt_lock };
			dropConnectionInner();
		}
		else
		{
			throw;
		}
	}
}



void Client::updateKeepAlive()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	ReadLock lock{ connectionMutex };
	if (isConnected())
	{
		sendToServer(serialize(NetworkMessageType::CtS_KeepAlive_Response));
	}
}



void Client::process(AddressedPacket&& packetReceived)
{
	ReadLock lock{ connectionMutex };
	if (!isConnected())
	{
		return;
	}

	auto& [fromAddress, packet] = packetReceived;

	NetworkMessageType const type = extractNmt(packet);

	if (fromAddress != serverAddress)
	{
		console::warning("Client> Warning: Message received from Unknown Server at " + fromAddress.GetString());
		return;
	}

	switch (type)
	{
	case NetworkMessageType::StC_HandshakeResponse_Failure:
	{
		std::string const reason = extractString(packet);
		console::error("Client> Error: Handshake Failed: " + reason);

		WriteLock lock{ connectionMutex, std::adopt_lock };
		dropConnectionInner();

		break;
	}
	case NetworkMessageType::StC_HandshakeResponse_Success:
	{
		console::log("Client> Handshake succeeded.");

		connectionState = ConnectionState::Connected;

		std::string const userList = extractString(packet);
		console::info("Client> Users on this server: " + userList);
		break;
	}
	case NetworkMessageType::StC_ChatMessage_ToClient:
	{
		std::string const message = extractString(packet);
		console::chat(message);
		break;
	}
	case NetworkMessageType::StC_ChatMessage_UserJoined:
	{
		std::string const otherUsername = extractString(packet);
		console::info("Client> User has joined the chat: " + otherUsername);
		break;
	}
	case NetworkMessageType::StC_KeepAlive_Query:
	{
		sendToServer(serialize(NetworkMessageType::CtS_KeepAlive_Response));
		break;
	}
	case NetworkMessageType::StC_ClientLeft:
	{
		std::string const otherUsername = extractString(packet);
		console::info("Client> User has left the chat: " + otherUsername);
		break;
	}
	case NetworkMessageType::StC_YourClientKicked:
	{
		std::string const reason = extractString(packet);
		console::warning("Client> You have been kicked from the chat. Reason: " + reason);

		WriteLock lock{ connectionMutex, std::adopt_lock };
		dropConnectionInner();

		break;
	}
	case NetworkMessageType::StC_BroadcastToDetectServer_Response:
	case NetworkMessageType::CtS_BroadcastToDetectServer:
	{
		console::log("Client> Ignored a broadcast message that was not meant for this client.");
		break;
	}
	default:
		throw std::runtime_error{ "Client> Invalid deserialized NetworkMessageType enum value" };
	}
}



void Client::sendToServer(Packet const& packet)
{
	assert(isConnected());
	socket.SendTo(serverAddress, packet);
}



void Client::broadcastToDetectServers()
{
	sockaddr_in address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.S_un.S_addr = INADDR_BROADCAST;

	// Just try a series of 10 ports to detect a running server.
	// This is needed since we are testing multiple servers on the same local machine.
	constexpr unsigned short maxPortRelative = 10;
	for (unsigned short i = 0; i < maxPortRelative; ++i)
	{
		address.sin_port = htons(DEFAULT_SERVER_PORT + i);
		socket.SendToRaw(address, serialize(NetworkMessageType::CtS_BroadcastToDetectServer));
	}
}



std::vector<AddressInfo> Client::receiveBroadcastResponses()
{
	// Broadcast to detect server
	socket.EnableBroadcast();
	FINAL_ACT_LAM(_, { socket.DisableBroadcast(); });

	broadcastToDetectServers();

	// Wait for a time.
	std::vector<AddressedPacket> const packets = socket.ReceiveMany(10, MAX_PACKET_LENGTH);

	// Deserialize messages to make sure they are the correct response type of message.
	std::vector<AddressInfo> validServers;
	for (auto const& [sourceAddress, immutablePacket] : packets)
	{
		auto packet = immutablePacket;
		NetworkMessageType const type = extractNmt(packet);
		// rest of packet is ignored

		bool const isValid = type == NetworkMessageType::StC_BroadcastToDetectServer_Response;
		if (isValid)
		{
			validServers.push_back(sourceAddress);
		}
	}
	
	return validServers;
}
