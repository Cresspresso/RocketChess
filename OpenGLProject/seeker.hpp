/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	seeker.hpp
**
**	Summary		:	Seeks a point by applying a force to change its velocity in the direction of the point.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

#include "simple_entity.hpp"



struct Seeker
{
	SimpleEntity simple;

	vec3 targetPosition = vec3();
	float mass = 1;
	float maxForce = 1;
	float maxSpeed = 1;
	float forceScale = 1;
	bool flee = false;

	float arrivingRadius = 1;
	float arrivedRadius = 0.01f;
	float arrivedZeroSpeed = 0.01f;



	void update();
	void render();
private:
	vec3 calculateNewVelocity() const;
};
