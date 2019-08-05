/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	network_entity.cpp
**
**	Summary		:	Common functionality shared by Client and Server.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#ifdef COMMENT

#include "console_tools.hpp"

#include "network_entity.hpp"



void INetworkEntity::AddMessage(Message const& message)
{
	std::lock_guard lock{ m_MessagesMutex };
	m_MessagesQueue.push_back(message);
}



void INetworkEntity::PrintMessages()
{
	// Print all new messages in the queue.
	std::lock_guard lock{ m_MessagesMutex };
	for (Message const& message : m_MessagesQueue)
	{
		std::lock_guard lock2{ g_ConsoleMutex };
		switch (message.type)
		{
		case MessageType::Error:
		{
			constexpr WORD const red = 12;
			UtilsSetTextColor(red);
			std::cout << message.content << '\n';
			break;
		}
		case MessageType::Warning:
		{
			constexpr WORD const yellow = 14;
			UtilsSetTextColor(yellow);
			std::cout << message.content << '\n';
			break;
		}
		case MessageType::Info:
		{
			constexpr WORD const lightGray = 7;
			UtilsSetTextColor(lightGray);
			std::cout << message.content << '\n';
			break;
		}
		case MessageType::Chat:
		{
			constexpr WORD const green = 10;
			UtilsSetTextColor(green);
			std::cout << message.content << '\n';
			break;
		}
		case MessageType::Log:
		{
			constexpr WORD const darkYellow = 6;
			UtilsSetTextColor(darkYellow);
			std::cout << message.content << '\n';
			break;
		}
		default:
			throw std::runtime_error{ "invalid chat message type" };
		}
	}
	m_MessagesQueue.clear();
}

#endif //~ COMMENT