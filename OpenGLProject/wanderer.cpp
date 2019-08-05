/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	wanderer.cpp
**
**	Summary		:	Seeks a random point in front of itself every few seconds.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include <glm/gtx/rotate_vector.hpp>

#include "world_math.hpp"
#include "time.hpp"
#include "random.hpp"

#include "wanderer.hpp"



ReturnCode Wanderer::update()
{
	/*timer -= Time::getDeltaTime();
	if (timer <= 0)
	{
		seeker.targetPosition = calculateTargetPosition();
		timer = cooldown;
	}*/

	vec3& pos = seeker.simple.transform.localPosition;

	bool regen = false;
	// if out of bounds...
	if (!positionLimits.contains(pos))
	{
		regen = true;
		// negate velocity.
		vec3& vel = seeker.simple.rigidbody.localVelocity;
		vel = -vel;
		// clamp position.
		pos = positionLimits.clamp(pos);
	}

	// if close enough to target position...
	if (toleranceEqual2(
		pos,
		seeker.targetPosition,
		threshold * threshold))
	{
		regen = true;
	}

	if (regen)
	{
		// generate new target position.
		seeker.targetPosition = calculateTargetPosition();
	}

	return seeker.update();
}

ReturnCode Wanderer::render()
{
	return seeker.render();
}

namespace
{
	static vec2 randomPointOnCircleEdge(float radius = 1)
	{
		float const angle = glm::radians(randomRealEx<float>(0, 360));
		return glm::rotate(vec2(radius, 0), angle);
	}

	static vec2 randomPointInsideCircle(float radius = 1)
	{
		return randomPointOnCircleEdge(radius) * randomRealEx<float>(0, 1);
	}
}

vec3 Wanderer::calculateTargetPosition() const
{
	// Wander algorithm.
	vec3 const& position = seeker.simple.transform.localPosition;
	vec3 const& velocity = seeker.simple.rigidbody.localVelocity;

	vec3 const vec = toleranceZero2(velocity)
		? vec3(0, 0, -distance)
		: glm::normalize(velocity) * distance;
	vec3 const centre = position + vec;
	vec2 const offset2D = randomPointInsideCircle(radius);
	vec3 const offset = vec3(offset2D.x, 0, offset2D.y);

	vec3 const targetPos = centre + offset;
	return targetPos;
}
