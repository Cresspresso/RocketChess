/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	singleton.cpp
**
**	Summary 	:	The only static lifetime object with non-trivial destructor in the application.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "application.hpp"

#include "singleton.hpp"



namespace singleton
{
	UniqueInitPtr<Application> instance{};



	ResourceWarehouse& getResources()
	{
		return instance->resources;
	}

	void postLoadSceneEvent(SceneIndexer i)
	{
		instance->postLoadSceneEvent(i);
	}

	bool isNetworkOpen()
	{
		return false;// instance->networkManager.isOpen();
	}

	ReturnCode openClient()
	{
		return RC_ERROR;// instance->networkManager.openClient(getUsername());
	}

	ReturnCode openHost()
	{
		return RC_ERROR;//instance->networkManager.openHost(getServername(), getUsername());
	}

	void closeNetwork()
	{
		//instance->networkManager.close();
	}

	NetworkState getNetworkState()
	{
		return NetworkState::None;//instance->networkManager.getState();
	}

	std::string getUsername()
	{
		return instance->config.username;
	}

	std::string getServername()
	{
		return instance->config.servername;
	}
}
