/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	server.hpp
**
**	Summary		:	Server network entity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#pragma once

#include <map>

#include "work_queue.hpp"
#include "network_entity.hpp"
#include "packet.hpp"
#include "info.hpp"



class Server : public INetworkEntity
{
public:

	virtual ~Server() = default;
	Server() = default;

	Server(Server const&) = delete;
	Server(Server&&) = delete;
	Server& operator=(Server const&) = delete;
	Server& operator=(Server&&) = delete;


	void Initialise() override;
	void UpdateReceive() override;
	void UpdateProcess() override;
	void UpdateKeepAlive() override;

private:

	void SendToAll(PacketData const& packet);
	void Process(Packet const& received);



	AddressInfo m_Address;
	//ServerBasicInfo m_Info;
	std::map<AddressInfo, ClientBasicInfo> m_Clients;
};

#endif //~ COMMENT