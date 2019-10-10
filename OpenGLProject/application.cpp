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

#include "console.hpp"
#include "to_string.hpp"

#include "application.hpp"



void Application::init()
{
	try
	{
		resources.init();
	}
	catch (...)
	{
		printException();
	}

	try
	{
		scene.emplace();
		scene->init();
	}
	catch (...)
	{
		throw;
		//printException();
	}
}

void Application::destroy() noexcept
{
	scene->destroy();
	scene.reset();
	resources.destroy();
}

void Application::update()
{
	if (m_restart)
	{
		m_restart = false;
		scene->destroy();

		try
		{
			scene.emplace();
			scene->init();
		}
		catch (...)
		{
			throw;
			//printException();
		}
	}
	else
	{
		return scene->update();
	}
}

void Application::render()
{
	return scene->render();
}
