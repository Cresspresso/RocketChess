/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cts_message_type.hpp
**
**	Summary		:	Client to Server Message Type enum.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

enum class ClientToServerMessageType
{
	HandshakeRequest,
	ChatMessage_FromClient,
	BroadcastToDetectServer,
	KeepAlive_Response,
	ClientQuit,

	//ClientLeft,
	//KeepAliveRequest_IsServerAlive,
	//KeepAliveResponse_ClientIsAlive,
	//KeepAliveResponse_ClientHasLeft,
	//FindServersBroadcast,
};

#endif //~ COMMENT