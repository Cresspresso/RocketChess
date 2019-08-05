/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	stc_message_type.hpp
**
**	Summary		:	Server to Client Message Type enum.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

enum class ServerToClientMessageType
{
	/* The client sent a message, but has not successfully joined the server. */
	UnrecognizedClient,
	HandshakeResponse_Failure,
	HandshakeResponse_Success,
	ChatMessage_ToClient,
	BroadcastToDetectServer_Response,
	ChatMessage_UserJoined,
	KeepAlive_Query,
	ClientLeft,
	YourClientKicked,

	//ServerClosed,
	//KeepAliveRequest_IsClientAlive,
	//KeepAliveResponse_ServerIsAlive,
	//KeepAliveResponse_ServerIsClosed,
	//OtherClientLeft,
	//ServerInfo,
};

#endif //~ COMMENT