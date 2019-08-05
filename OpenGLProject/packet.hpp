/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	packet.hpp
**
**	Summary		:	Aliases for packets sent and received across the network.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <cstddef>
#include <vector>
#include <tuple>

#include "address.hpp"



using RawByte = unsigned char;

using Packet = std::vector<std::byte>;

using AddressedPacket = std::pair<AddressInfo, Packet>;



constexpr int const MAX_PACKET_LENGTH = 256;
