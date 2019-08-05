/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	world_math.hpp
**
**	Summary		:	Math functions for geometry, vectors, space, rotations, etc.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"



mat4 makePositionMatrix(vec3 const& pos);
mat4 makeRotationMatrix(quat const& rot);
mat4 makeScaleMatrix(vec3 const& scale);
mat4 makeTransformMatrix(vec3 const& pos, quat const& rot, vec3 const& scale);


mat3 calculateFixNormalsMatrix(mat4 const& modelMatrix);


vec3 transformPoint(mat4 const& transformation, vec3 const& point);
vec3 transformDirection(mat4 const& transformation, vec3 const& dir);


bool isInsideRange(float value, float min, float max);
bool isInsideBox(vec3 const& point, vec3 const& min, vec3 const& max);


// makes a world-space ray direction from a view point in the rect range [-1..1].
vec3 makeRayDirection(vec2 const& viewPoint, mat4 const& vpMatrix);

// converts a screen point to a view point in the rect range [-1..1].
// screenHalfSize = screenSize / 2.
vec2 screenPointToViewPoint(vec2 const& screenPoint, vec2 const& screenHalfSize);



vec3 project(vec3 const& point, vec3 const& linePoint, vec3 const& lineVector);
vec3 moveTowards(vec3 const& position, vec3 const& targetPosition, float distance);
vec3 limit(vec3 const& vec, float length);



bool toleranceEqual(float a, float b, float tolerance = 0.001f);
bool toleranceZero(float a, float tolerance = 0.001f);

// tolerance equality of two points.
// tolerance2: max distance squared
template<class T>
bool toleranceEqual2(T const& a, T const& b, float tolerance2 = 0.001f)
{
	return glm::distance2(a, b) <= tolerance2;
}

template<class T>
bool toleranceZero2(T const& a, float tolerance2 = 0.001f)
{
	return glm::length2(a) <= tolerance2;
}



quat lookDir(vec3 const& forward, vec3 const& up);


template<class T>
T normalizeOrElse(
	T const& v,
	T const& elseValue = glm::zero<T>(),
	decltype(glm::length(v)) tolerance = 0.001f)
{
	auto const d = glm::length(v);
	return d <= tolerance ? elseValue : (v / d);
}
