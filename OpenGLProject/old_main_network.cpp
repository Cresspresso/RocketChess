/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	main_network.cpp
**
**	Summary		:	Network entry point.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#ifdef COMMENT

#include "network.hpp"
#include "socket.hpp"
#include "client.hpp"
#include "server.hpp"
#include "console_tools.hpp"
#include "handle_exception.hpp"



void ReceiveThreadFunc(INetworkEntity& entity)
{
	while (entity.m_IsRunning)
	{
		try
		{
			entity.UpdateReceive();
		}
		catch (...)
		{
			HandleException(entity);
		}
	}
}



void KeepAliveThreadFunc(INetworkEntity& entity)
{
	while (entity.m_IsRunning)
	{
		try
		{
			entity.UpdateKeepAlive();
		}
		catch (...)
		{
			HandleException(entity);
		}
	}
}



void ProcessThreadFunc(INetworkEntity& entity)
{
	while (entity.m_IsRunning)
	{
		try
		{
			entity.UpdateProcess();
		}
		catch (...)
		{
			HandleException(entity);
		}
	}
}



void RunNetworkEntity(INetworkEntity& entity)
{
	entity.m_IsRunning = true;

	entity.Initialise();

	// Spawn Thread for Receiving Messages
	std::thread receiveThread{ ReceiveThreadFunc, std::ref(entity) };

	std::thread keepAliveThread{ KeepAliveThreadFunc, std::ref(entity) };

	// Start Processing Messages on this Thread
	ProcessThreadFunc(entity);

	keepAliveThread.join();
	receiveThread.join();
}



std::unique_ptr<INetworkEntity> g_NetworkEntity = nullptr;



template<class T>
void CreateAndRunNetworkEntity()
{
	g_NetworkEntity = std::make_unique<T>();
	RunNetworkEntity(*g_NetworkEntity);
}



int mainNetwork()
{
	try
	{
		UtilsSetTextColor(7);

		g_Network.Startup();

		bool const isServer = 'S' == QueryOption("Client or Server (C/S)? ", "CS");
		g_Socket.Open(QueryPortNumber(isServer ? DEFAULT_SERVER_PORT : DEFAULT_CLIENT_PORT));

		if (isServer)
		{
			CreateAndRunNetworkEntity<Server>();
		}
		else
		{
			CreateAndRunNetworkEntity<Client>();
		}

		UtilsSetTextColor(7);

		g_Socket.Close();
		g_Network.Cleanup();

		_getch();
		return EXIT_SUCCESS;
	}
	catch (...)
	{
		UtilsSetTextColor(12);
		//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 5 });
		//std::cerr << "Critical Error: ";
		std::cerr << GetStringFromException() << '\n';
	}

	UtilsSetTextColor(7);
	_getch();
	return EXIT_FAILURE;
}

#endif //~ COMMENT