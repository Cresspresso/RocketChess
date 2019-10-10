/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mover.cpp
**
**	Summary		:	Moves along a path without smooth animation, just simulating velocity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "time.hpp"

#include "mover.hpp"



void Mover::update()
{
	transform.recalculate();
	vec3 const& targetPosition = getTargetPosition();
	vec3& position = transform.localPosition;

	float const distance = speed * Time::getDeltaTime();
	position = moveTowards(position, targetPosition, distance);

	// if reached target position
	if (toleranceEqual2(targetPosition, position))
	{
		// move to next target position
		currentIndex = cycle<size_t>(currentIndex + 1, points.size());
	}
}



void Mover::render()
{
	transform.recalculate();
	renderer.modelMatrix = transform.modelMatrix;
	return renderer.render();
}



vec3 const& Mover::getTargetPosition()
{
	// ensure at least one target position
	if (points.empty()) { points.push_back(vec3()); }

	// ensure currentIndex is in range
	currentIndex = cycle<size_t>(currentIndex, points.size());

	return points[currentIndex];
}



vec3 Mover::getVelocity()
{
	transform.recalculate();
	vec3 const& targetPosition = getTargetPosition();
	vec3 const& position = transform.localPosition;

	vec3 const dir = glm::normalize(targetPosition - position);
	vec3 const velocity = dir * speed;
	return velocity;
}
