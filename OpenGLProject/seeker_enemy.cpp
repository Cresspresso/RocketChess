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

#include <cress/moo/final_act.hpp>

#include "math_utils.hpp"
#include "seeker_enemy.hpp"

void SeekerEnemy::update()
{
	CRESS_MOO_FINAL_ACT_BEGIN(fa);
	// clamp position
	vec3& pos = seeker.simple.transform.localPosition;
	pos = positionLimits.clamp(pos);
	CRESS_MOO_FINAL_ACT_END(fa);

	seeker.update();
}

void SeekerEnemy::render()
{
	return seeker.render();
}
