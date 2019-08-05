/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mouse_mover.cpp
**
**	Summary		:	When the player clicks the LMB, teleports to that click location in world space.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	12/06/2019
*/

#include <glm/gtx/intersect.hpp>

#include "input.hpp"
#include "globals.hpp"

#include "mouse_mover.hpp"

ReturnCode MouseMover::update()
{
	if (InputState::DownFirst == getMouseButtonState(MOUSE_LEFT))
	{
		simple.transform.localPosition = calculatePosition();
	}

	return simple.update();
}

ReturnCode MouseMover::render()
{
	return simple.render();
}

vec3 MouseMover::calculatePosition() const
{
	// get camera properties
	mat4 const& vpMatrix = g_cameraVPMatrix;
	vec3 const& eye = g_cameraEye;

	// get mouse ray in world space
	vec3 const rayDir = makeRayDirection(getMouseViewPos(), vpMatrix);
	vec3 const rayVector = 10'000.f * rayDir;

	vec3 const planeOrigin = vec3();

	// check if mouse ray intersects the ground.
	float distance;
	if (glm::intersectRayPlane(eye, rayVector, planeOrigin, vec3(0, 1, 0), distance))
	{
	}
	else if (glm::intersectRayPlane(eye, rayVector, planeOrigin, vec3(0, -1, 0), distance))
	{
	}
	else
	{
		distance = 5.0f;
	}

	return eye + rayVector * distance;
}
