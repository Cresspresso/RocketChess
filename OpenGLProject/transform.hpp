/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	transform.hpp
**
**	Summary		:	Component that represents position, rotation, and scale relative to a parent Transform.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"
#include "world_math.hpp"

struct Transform
{
	vec3 localPosition = vec3();
	quat localRotation = quat();
	vec3 localScale = vec3(1);

	mat4 localMatrix = mat4();
	mat4 modelMatrix = mat4();
	mat4 viewMatrix = mat4();

	std::weak_ptr<Transform> parent{};

	// transforms from local space to parent space
	mat4 makeLocalMatrix() const { return makeTransformMatrix(localPosition, localRotation, localScale); }

	void recalculateLocalMatrix(); // uses localPosition, localRotation, localScale
	void recalculateModelMatrix(); // uses localToParentMatrix
	void recalculateViewMatrix(); // uses localToWorldMatrix
	void recalculate(); // recalculates all properties
};
