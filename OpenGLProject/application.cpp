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
	DO_ANYALL(scene.init());
	return END_ANYALL();
}

void Application::destroy() noexcept
{
	scene.destroy();
	resources.destroy();
}

ReturnCode Application::update()
{
	return scene.update();
}

ReturnCode Application::render()
{
	return scene.render();
}
