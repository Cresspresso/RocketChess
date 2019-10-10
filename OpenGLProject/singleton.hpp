/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	singleton.hpp
**
**	Summary 	:	The only static lifetime object with non-trivial destructor in the application.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "unique_init_ptr.hpp"



struct Application;
struct ResourceWarehouse;



namespace singleton
{
	extern UniqueInitPtr<Application> instance;



	ResourceWarehouse& getResources();

	void postRestartMessage();
}
