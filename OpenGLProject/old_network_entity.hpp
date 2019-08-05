/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	network_entity.hpp
**
**	Summary		:	Common functionality shared by Client and Server.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <vector>
#include <atomic>

#include "packet.hpp"
#include "work_queue.hpp"



enum MessageType
{
	Error,
	Warning,
	Info,
	Chat,
	Log,
};

struct Message
{
	MessageType type;
	std::string content;
};



class INetworkEntity
{
public:
	virtual ~INetworkEntity() = default;
	INetworkEntity() = default;

	INetworkEntity(INetworkEntity const&) = delete;
	INetworkEntity(INetworkEntity&&) = delete;
	INetworkEntity& operator=(INetworkEntity const&) = delete;
	INetworkEntity& operator=(INetworkEntity&&) = delete;


	virtual void Initialise() = 0;
	virtual void UpdateReceive() = 0;
	virtual void UpdateProcess() = 0;
	virtual void UpdateKeepAlive() = 0;
	void AddMessage(Message const& message);

	std::atomic_bool m_IsRunning;
protected:
	void PrintMessages();

	WorkQueue<Packet> m_WorkQueue;
	std::mutex m_MessagesMutex;
	std::vector<Message> m_MessagesQueue;
};

constexpr unsigned short DEFAULT_SERVER_PORT = 50012;
constexpr unsigned short DEFAULT_CLIENT_PORT = 60013;

#endif //~ COMMENT