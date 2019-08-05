/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	seeker.cpp
**
**	Summary		:	Seeks a point by applying a force to change its velocity in the direction of the point.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "world_math.hpp"
#include "time.hpp"

#include "seeker.hpp"



ReturnCode Seeker::update()
{
	BEGIN_ANYALL();

	simple.rigidbody.localVelocity = calculateNewVelocity();
	DO_ANYALL(simple.update()); // Update position with velocity.

	// if velocity is non-zero
	if (!toleranceZero2(simple.rigidbody.localVelocity))
	{
		// rotate instantly to look towards the velocity direction.
		simple.transform.localRotation = lookDir(simple.rigidbody.localVelocity, constants::up);
	}

	return END_ANYALL();
}

ReturnCode Seeker::render()
{
	return simple.render();
}

// Seek/Flee algorithm, with Arrival.
vec3 Seeker::calculateNewVelocity() const
{
	vec3 const& position = simple.transform.localPosition;
	vec3 const& velocity = simple.rigidbody.localVelocity;

	vec3 toTarget = targetPosition - position;
	float const d = glm::length(toTarget);

	// calculate desired velocity.
	vec3 desiredVelocity;
	if (flee)
	{
		// move in opposite direction, away from target position.
		desiredVelocity = maxSpeed * normalizeOrElse(-toTarget, vec3(1));
	}
	else
	{
		// move towards target position.
		desiredVelocity = maxSpeed * normalizeOrElse(toTarget);
		//desiredVelocity = limit(toTarget, maxSpeed);

		if (d < arrivingRadius)
		{
			//desiredVelocity = vec3();
			// inverse scale by distance remaining.
			desiredVelocity *= (d / arrivingRadius);
		}
	}

	// calculate force to get lined up with desired velocity.
	vec3 force = desiredVelocity - velocity;
	force = limit(force * forceScale, maxForce);

	// calculate acceleration vector.
	assert(mass != 0);
	vec3 const acceleration = force / mass;

	// apply acceleration and calculate new velocity.
	vec3 newVelocity = velocity + acceleration * Time::getDeltaTime();
	newVelocity = limit(newVelocity, maxSpeed);

	// if we have arrived...
	if (!flee && d < arrivedRadius && glm::length2(newVelocity) < arrivedZeroSpeed * arrivedZeroSpeed)
	{
		// stop moving.
		newVelocity = vec3();
	}

	return newVelocity;
}
