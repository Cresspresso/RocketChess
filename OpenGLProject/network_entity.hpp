
#pragma once

#include <atomic>

#include "work_queue.hpp"
#include "address.hpp"

#include "socket.hpp"

#include "network_entity_threads.hpp"



class NetworkEntity
{
public:
	virtual ~NetworkEntity();
	NetworkEntity(unsigned short portNumber);
	void startThreads(std::atomic_bool& whileCondition);

	virtual void updateReceive() = 0;
	virtual void updateProcess();
	virtual void updateKeepAlive() = 0;

	AddressInfo getAddress() { return socket.GetAddress(); }

protected:
	virtual void process(AddressedPacket&& receivedPacket) = 0;

	Socket socket;

	WorkQueue<AddressedPacket> receivedPacketQueue;
	NetworkEntityThreads threads;
};
