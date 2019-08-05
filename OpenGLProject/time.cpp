/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	time.cpp
**
**	Summary		:	Keeps track of delta time.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "common.hpp"

#include "time.hpp"

namespace Time
{
	static float deltaTime = 0;
	static float currentTime = 0;
	static float lastTime = 0;

	static float now() noexcept { return glutGet(GLUT_ELAPSED_TIME) * 0.001f; }

	float getDeltaTime() noexcept { return deltaTime; }
	float getCurrentTime() noexcept { return currentTime; }

	void update() noexcept
	{
		currentTime = now();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	void init() noexcept
	{
		lastTime = now();
		update();
	}
}
