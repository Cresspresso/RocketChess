
#pragma once

#include <string>

#include "packet.hpp"
#include "network_message_type.hpp"
#include "compatibility.hpp"



Packet serialize(std::string const& str);
std::string extractString(Packet& packet);



constexpr size_t netSerializedSizeNmt = 1;
Packet serialize(NetworkMessageType type);
NetworkMessageType extractNmt(Packet& packet);



template<class Arg>
void appendSerializeLink(Packet& packet, Arg&& arg)
{
	Packet const tail = serialize(std::forward<Arg>(arg)); // serialize the argument
	packet.insert(packet.end(), tail.begin(), tail.end()); // append to packet
}

template<class Arg, class...Args>
void appendSerializeLink(Packet& packet, Arg&& arg, Args&&...args)
{
	appendSerializeLink(packet, std::forward<Arg>(arg));
	appendSerializeLink(packet, std::forward<Args>(args)...);
}



template<class...Args>
Packet serializeLink(Args&&...args)
{
	Packet packet;
	appendSerializeLink(packet, std::forward<Args>(args)...);
	return packet;
}
