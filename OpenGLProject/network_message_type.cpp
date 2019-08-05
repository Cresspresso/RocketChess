
#include "network_message_type.hpp"


#ifdef COMMENT
Packet prefixNmtHeader(NetworkMessageType type, Packet const& tail)
{
	Packet packet(1 + tail.size());

	static_assert(sizeof(type) == sizeof(std::byte));
	packet[0] = static_cast<std::byte>(type);
	std::memcpy(&packet[1], tail.data(), tail.size());

	return packet;
}



std::pair<NetworkMessageType, Packet> splitNmtHeader(Packet const& packet)
{
	if (packet.size() < 1) { throw std::out_of_range{ "packet size too small" }; }

	NetworkMessageType const type = static_cast<NetworkMessageType>(packet[0]);
	Packet tail(packet.size() - 1);
	std::copy(packet.begin() + 1, packet.end(), tail.begin());

	return std::pair<NetworkMessageType, Packet>{ type, std::move(tail) };
}
#endif //~ COMMENT
