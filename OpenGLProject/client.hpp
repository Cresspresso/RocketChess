
#pragma once

#include <shared_mutex>

#include "network_entity.hpp"



struct ServerBroadcastResponse
{
	AddressInfo sourceAddress;
	Packet tail;
};

enum class ConnectionState
{
	None,
	Connecting,
	Connected,
};



class Client : public NetworkEntity
{
public:
	virtual ~Client();
	Client(std::string const& username);

	void connectTo(AddressInfo const& serverAddress);
	void dropConnection();

	void updateReceive() override;
	void updateKeepAlive() override;

	std::vector<AddressInfo> receiveBroadcastResponses();

protected:
	void process(AddressedPacket&& packetReceived) override;
private:
	// functions that require connectionMutex write lock
	void dropConnectionInner();
	// functions that require connectionMutex read lock
	bool isConnected() const { return ConnectionState::None != connectionState; }
	void sendToServer(Packet const& packet);
	void broadcastToDetectServers();



	std::string const username;

	std::shared_timed_mutex connectionMutex;
	// variables that require connectionMutex lock
	ConnectionState connectionState = ConnectionState::None;
	AddressInfo serverAddress;
};
