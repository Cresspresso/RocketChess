
#include <stdexcept>

#include "network_serialization.hpp"



Packet serialize(std::string const& str)
{
	Packet packet(str.size() + 1);

	str.copy(reinterpret_cast<char*>(packet.data()), str.size());
	packet[str.size()] = static_cast<std::byte>('\0');

	return packet;
}

std::string extractString(Packet& packet)
{
	char const* const p = reinterpret_cast<char const*>(packet.data());
	size_t const sz = std::strlen(p);
	if (sz >= packet.size())
	{
		throw std::out_of_range{ "could not extract string because C-String extends beyond packet size" };
	}
	std::string str{ p, sz };
	packet.erase(packet.begin(), packet.begin() + sz + 1);
	return str;
}



Packet serialize(NetworkMessageType type)
{
	return Packet{ static_cast<std::byte>(type) };
}

NetworkMessageType extractNmt(Packet& packet)
{
	if (packet.size() < netSerializedSizeNmt)
	{
		throw std::out_of_range{ "could not extract NetworkMessageType because packet size is too small" };
	}
	NetworkMessageType const type = static_cast<NetworkMessageType>(packet[0]);
	packet.erase(packet.begin());
	return type;
}
