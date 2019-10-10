/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mover.hpp
**
**	Summary		:	Moves along a path without smooth animation, just simulating velocity.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <vector>

#include "common.hpp"
#include "math_utils.hpp"

#include "transform.hpp"
#include "renderer.hpp"



struct Mover
{
	Transform transform;
	Renderer renderer;

	std::vector<vec3> points;
	size_t currentIndex = 0;

	float speed = 1;



	void update();
	void render();

	vec3 const& getTargetPosition();
	vec3 getVelocity();
};
