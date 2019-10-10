/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	pursuer.cpp
**
**	Summary		:	Pursues a target by seeking a point ahead of it.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "world_math.hpp"
#include "time.hpp"

#include "pursuer.hpp"



void Pursuer::update()
{
	seeker.targetPosition = calculateTargetFuturePosition();
	return seeker.update();
}

void Pursuer::render()
{
	return seeker.render();
}

vec3 Pursuer::calculateTargetFuturePosition() const
{
	// Pursue/Evade algorithm.
	vec3 const& position = seeker.simple.transform.localPosition;

	float const targetSpeed = glm::length(target.velocity);
	if (targetSpeed < 0.001f)
	{
		return target.position;
	}
	// vec = v * ((Sv + ||a|| * Sav) + (1 / ||v||)*(Sd + ||a|| * Sad))

	// (1 / ||v||)
	float const targetSpeedRecip = 1.0f / targetSpeed;

	// ||a||
	float const distanceToTarget = glm::distance(target.position, position);

	// (Sv + ||a|| * Sav)
	float const velocityScale = this->targetScales.velocity + (distanceToTarget * this->distanceScales.velocity);
	// (Sd + ||a|| * Sad)
	float const directionScale = this->targetScales.direction + (distanceToTarget * this->distanceScales.direction);

	// ((Sv + ||a|| * Sav) + (1 / ||v||)*(Sd + ||a|| * Sad))
	float const finalScale = velocityScale + targetSpeedRecip * directionScale;

	vec3 const vec = target.velocity * finalScale;
	vec3 const futurePosition = target.position + vec;
	return futurePosition;
}
