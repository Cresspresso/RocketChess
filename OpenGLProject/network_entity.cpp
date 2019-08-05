
#include "console.hpp"
#include "to_string.hpp"

#include "network_message_type.hpp"

#include "network_entity.hpp"



NetworkEntity::~NetworkEntity()
{
	threads.stop();
}



NetworkEntity::NetworkEntity(unsigned short portNumber)
{
	socket.Open(portNumber);
}



void NetworkEntity::startThreads(std::atomic_bool& whileCondition)
{
	threads.start(whileCondition, *this);
}



void NetworkEntity::updateProcess()
{
	auto optPacket = receivedPacketQueue.tryPop();
	if (optPacket)
	{
		TRY_CATCH_PRINT(process(std::move(*optPacket)));
	}
}
