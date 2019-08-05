/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	pursuer.hpp
**
**	Summary		:	Pursues a target by seeking a point ahead of it.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "seeker.hpp"



struct Pursuer
{
	Seeker seeker;

	struct Target
	{
		vec3 position = vec3();
		vec3 velocity = vec3();

	} target;

	struct VelocityScale
	{
		float velocity = 0; // scaled by target.velocity
		float direction = 0; // scaled by normalized(target.velocity)
	};

	VelocityScale targetScales = { 0.1f, 0 }; // just scaled using target.velocity
	VelocityScale distanceScales = { 0.9f, 0 }; // also scaled by distance from self to target

	bool getEvade() const noexcept { return seeker.flee; }
	void setEvade(bool evade) noexcept { seeker.flee = evade; }



	ReturnCode update();
	ReturnCode render();
private:
	vec3 calculateTargetFuturePosition() const;
};
