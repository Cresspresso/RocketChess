/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	player.cpp
**
**	Summary		:	Player controller and renderer.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "math_utils.hpp"
#include "input.hpp"

#include "player.hpp"



void Player::update()
{
	seeker.targetPosition = calculateTargetPosition();
	try {
		seeker.update();

		// clamp position
		vec3& pos = seeker.simple.transform.localPosition;
		pos = positionLimits.clamp(pos);
	}
	catch (...)
	{
		// clamp position
		vec3& pos = seeker.simple.transform.localPosition;
		pos = positionLimits.clamp(pos);

		throw;
	}
}



void Player::render()
{
	return seeker.render();
}



// calculates movement target position for seeker to seek.
vec3 Player::calculateTargetPosition() const
{
	bool left = isKeyboardKeyDown('a') || isKeyboardKeyDown('A') || isSpecialKeyDown(GLUT_KEY_LEFT);
	bool right = isKeyboardKeyDown('d') || isKeyboardKeyDown('D') || isSpecialKeyDown(GLUT_KEY_RIGHT);
	bool up = isKeyboardKeyDown('w') || isKeyboardKeyDown('W') || isSpecialKeyDown(GLUT_KEY_UP);
	bool down = isKeyboardKeyDown('s') || isKeyboardKeyDown('S') || isSpecialKeyDown(GLUT_KEY_DOWN);

	int horz = (left ? -1 : 0) + (right ? 1 : 0);
	int vert = (up ? 1 : 0) + (down ? -1 : 0);
	vec2 inputDir = vec2(horz, vert);
	inputDir = toleranceZero2(inputDir)
		? vec2()
		: glm::normalize(inputDir);

	vec3 dir = vec3(inputDir.x, 0, -inputDir.y);

	vec3 const& position = seeker.simple.transform.localPosition;
	vec3 point = position + dir * 3.f;

	return point;
}
