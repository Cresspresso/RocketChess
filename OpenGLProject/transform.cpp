/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	transform.cpp
**
**	Summary		:	Component that represents position, rotation, and scale relative to a parent Transform.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "transform.hpp"

void Transform::recalculateLocalMatrix()
{
	localMatrix = makeLocalMatrix();
}

void Transform::recalculateModelMatrix()
{
	if (auto parent = this->parent.lock())
	{
		modelMatrix = parent->modelMatrix * localMatrix;
	}
	else
	{
		modelMatrix = localMatrix;
	}
}

void Transform::recalculateViewMatrix()
{
	viewMatrix = glm::inverse(viewMatrix);
}

void Transform::recalculate()
{
	recalculateLocalMatrix();
	recalculateModelMatrix();
	recalculateViewMatrix();
}
