/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	seeker_enemy.hpp
**
**	Summary		:	Enemy character that seeks the player.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "seeker.hpp"
#include "box_range.hpp"



struct SeekerEnemy
{
	Seeker seeker;
	BoxRange positionLimits = { vec3(-10, 0, -10), vec3(10, 0, 10) };

	ReturnCode update();
	ReturnCode render();
};
