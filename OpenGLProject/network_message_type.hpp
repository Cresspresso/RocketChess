
#pragma once

#include "packet.hpp"

enum class NetworkMessageType : RawByte
{
	// Client to Server
	CtS_BroadcastToDetectServer = 20,
	CtS_HandshakeRequest,
	CtS_DroppedConnection,
	CtS_KeepAlive_Response,
	CtS_ChatMessage,

	// Server to Client
	StC_BroadcastToDetectServer_Response = 70,
	StC_HandshakeResponse_Failure,
	StC_HandshakeResponse_Success,
	StC_ChatMessage_ToClient,
	StC_ChatMessage_UserJoined,
	StC_KeepAlive_Query,
	StC_ClientLeft,
	StC_YourClientKicked,
	StC_ChatMessage,
};

#ifdef COMMENT
Packet prefixNmtHeader(NetworkMessageType type, Packet const& tail);
std::pair<NetworkMessageType, Packet> splitNmtHeader(Packet const& packet);
#endif ~COMMENT
