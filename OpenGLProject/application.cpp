/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	application.cpp
**
**	Summary 	:	Scene and resource manager.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "socket.hpp"

#include "console.hpp"
#include "to_string.hpp"

#include "application.hpp"



ReturnCode Application::init()
{
	BEGIN_ANYALL();
	DO_ANYALL(resources.init());
	scene.emplace();
	DO_ANYALL(scene->init());
	return END_ANYALL();
}

void Application::destroy() noexcept
{
	scene->destroy();
	scene.reset();
	resources.destroy();
}

ReturnCode Application::update()
{
	if (m_restart)
	{
		m_restart = false;
		scene->destroy();
		scene.emplace();
		BEGIN_ANYALL();
		DO_ANYALL(scene->init());
		DO_ANYALL(scene->update());
		return END_ANYALL();
	}
	else
	{
		return scene->update();
	}
}

ReturnCode Application::render()
{
	return scene->render();
}
