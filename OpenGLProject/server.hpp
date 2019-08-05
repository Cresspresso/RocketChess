
#pragma once

#include <chrono>
#include <map>

#include "network_entity.hpp"



struct ClientInfo
{
	std::string username;
	typename std::chrono::system_clock::time_point lastAlive = std::chrono::system_clock::now();
};



class Server : public NetworkEntity
{
public:
	virtual ~Server();
	Server(std::string const& servername);

	void updateReceive() override;
	void updateKeepAlive() override;

protected:
	void process(AddressedPacket&& packetReceived) override;
private:
	// requires clientsMutex lock
	void sendToAllClients(Packet const& packet);



	std::string const servername;

	std::mutex clientsMutex;
	std::map<AddressInfo, ClientInfo> clients;
};
