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

	void postRestartMessage()
	{
		instance->postRestartMessage();
	}
}
