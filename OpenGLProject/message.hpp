/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	message.hpp
**
**	Summary		:	Serialization of messages.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <WinSock2.h>
#include <cassert>
#include <cstring>
#include <string>

#include "socket.hpp"
#include "packet.hpp"
#include "stc_message_type.hpp"
#include "cts_message_type.hpp"



inline PacketData
SerializeRaw(u_short type, std::string const& content)
{
	size_t n = content.length() + 1;
	PacketData packet(sizeof(u_short) + n, '\0');
	assert(packet.size() <= MAX_PACKET_LENGTH);

	Byte * const p = packet.data();

	// serialize type
	*(reinterpret_cast<u_short*>(p + 0)) = htons(type);

	// serialize content
	strncpy_s(p + sizeof(u_short), n, content.c_str(), content.length());
	assert('\0' == packet[packet.size() - 1]);

	return packet;
}

template<class T>
PacketData
Serialize(T type, std::string const& content)
{
	return SerializeRaw(static_cast<u_short>(type), content);
}



inline std::pair<u_short, std::string>
DeserializeRaw(PacketData const& packet)
{
	if (packet.size() < sizeof(u_short) + 1)
	{
		throw std::invalid_argument{ "packet size too small" };
	}

	Byte const* const p = packet.data();

	u_short const type = ntohs(*reinterpret_cast<u_short const*>(p + 0));

	// Find null terminator
	Byte const* const contentBegin = p + sizeof(u_short);
	Byte const* const contentEnd = p + packet.size();
	Byte const* const terminator = std::find(contentBegin, contentEnd, '\0');

	// Ensure string only contains characters in the packet (prohibit overflow).
	std::string const content = terminator == contentEnd
		? std::string{ contentBegin, packet.size() - 1 - sizeof(u_short) }
		: std::string{ contentBegin, terminator };

	return std::pair<u_short, std::string>{ type, content };
}

template<class T>
std::pair<T, std::string>
Deserialize(PacketData const& packet)
{
	auto[type, content] = DeserializeRaw(packet);
	return std::pair<T, std::string>{ static_cast<T>(type), std::move(content) };
}

#endif //~ COMMENT