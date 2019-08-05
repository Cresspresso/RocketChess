/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	network.hpp
**
**	Summary		:	Network wrapper class.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <memory>



class Network
{
public:
	void Startup();
	void Cleanup();

	bool IsOnline() const { return m_IsOnline; }

	~Network();
	Network() = default;
	Network(Network const&) = delete;
	Network(Network&&) = delete;
	Network& operator=(Network const&) = delete;
	Network& operator=(Network&&) = delete;

private:
	bool m_IsOnline = false;
};



extern std::unique_ptr<Network> g_Network;
