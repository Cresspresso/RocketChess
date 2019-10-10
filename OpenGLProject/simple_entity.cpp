/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	simple_entity.cpp
**
**	Summary		:	Entity with transform, rigidbody, and mesh renderer components.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "simple_entity.hpp"

SimpleEntity::SimpleEntity()
{
	rigidbody.transform = &transform;
}

void SimpleEntity::update()
{
	return rigidbody.update();
}

void SimpleEntity::render()
{
	transform.recalculate();
	renderer.modelMatrix = transform.modelMatrix;
	return renderer.render();
}
