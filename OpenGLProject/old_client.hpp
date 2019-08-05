/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	client.hpp
**
**	Summary		:	Client network entity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include "work_queue.hpp"
#include "network_entity.hpp"
#include "packet.hpp"
#include "info.hpp"
//#include "input_buffer.hpp"



class Client : public INetworkEntity
{
public:

	virtual ~Client() = default;
	Client() = default;

	Client(Client const&) = delete;
	Client(Client&&) = delete;
	Client& operator=(Client const&) = delete;
	Client& operator=(Client&&) = delete;


	void Initialise() override;
	void UpdateReceive() override;
	void UpdateProcess() override;
	void UpdateKeepAlive() override;


private:

	//bool UserInput();
	void Process(Packet const& received);
	void BroadcastForServers();



	ClientBasicInfo m_Info;
	AddressInfo m_ServerAddress;
	//ServerBasicInfo m_ServerInfo;

	WorkQueue<Packet> m_WorkQueue;
	//InputBuffer m_InputBuffer;

	std::mutex m_MessagesMutex;
	std::vector<Message> m_MessagesQueue;
};

#endif //~ COMMENT