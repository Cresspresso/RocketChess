/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	player.hpp
**
**	Summary		:	Player controller and renderer.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <vector>
#include <functional>

#include "seeker.hpp"
#include "box_range.hpp"



struct Player
{
	Seeker seeker;
	BoxRange positionLimits = { vec3(-10, 0, -10), vec3(10, 0, 10) };

	long score = 0;
	int lives = 3;



	void update();
	void render();
private:
	vec3 calculateTargetPosition() const;
};
