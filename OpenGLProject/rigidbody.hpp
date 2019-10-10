/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	rigidbody.hpp
**
**	Summary		:	Simulates velocity by updating the transform's position every frame.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "transform.hpp"

struct Rigidbody
{
	Transform* transform = nullptr;
	vec3 localVelocity = vec3();
	vec3 localAngularVelocity = vec3();
	float mass = 1;

	void update();
	//void addForce(vec3 const& force);
};
