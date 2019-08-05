/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	wanderer.hpp
**
**	Summary		:	Seeks a random point in front of itself every few seconds.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "box_range.hpp"
#include "seeker.hpp"



struct Wanderer
{
	Seeker seeker;

	float distance = 2; // distance from self to centre of circle.
	float radius = 1; // radius around the centre.
	float threshold = 0.3f; // max distance from self to target position before generating a new target position.
	//float cooldown = 0.4f; // seconds before generating new target position.
	//float timer = 0; // current time remaining.

	BoxRange positionLimits = { vec3(-10,0,-10), vec3(10,0,10) };

	ReturnCode update();
	ReturnCode render();
private:
	vec3 calculateTargetPosition() const;
};
