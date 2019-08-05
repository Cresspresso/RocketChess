/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	seeker_enemy.cpp
**
**	Summary		:	Enemy character that seeks the player.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "math_utils.hpp"
#include "seeker_enemy.hpp"

ReturnCode SeekerEnemy::update()
{
	ReturnCode const r = seeker.update();

	// clamp position
	vec3& pos = seeker.simple.transform.localPosition;
	pos = positionLimits.clamp(pos);

	return r;
}

ReturnCode SeekerEnemy::render()
{
	return seeker.render();
}
