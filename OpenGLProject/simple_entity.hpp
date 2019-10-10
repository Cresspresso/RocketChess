/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	simple_entity.hpp
**
**	Summary		:	Entity with transform, rigidbody, and mesh renderer components.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "renderer.hpp"
#include "transform.hpp"
#include "rigidbody.hpp"

struct SimpleEntity
{
	SimpleEntity();
	void update();
	void render();

	Transform transform;
	Rigidbody rigidbody;
	Renderer renderer;
};
