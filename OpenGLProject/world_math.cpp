/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	world_math.cpp
**
**	Summary		:	Math functions for geometry, vectors, space, rotations, etc.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

//#include <glm/gtx/orthonormalize.hpp>
//#include <glm/gtc/quaternion.hpp>

#include "world_math.hpp"



mat4 makePositionMatrix(vec3 const& pos)
{
	return glm::translate(mat4(), pos);
}

mat4 makeRotationMatrix(quat const& rot)
{
	return glm::toMat4(rot);
}

mat4 makeScaleMatrix(vec3 const& scale)
{
	return glm::scale(mat4(), scale);
}

mat4 makeTransformMatrix(vec3 const & pos, quat const & rot, vec3 const & scale)
{
	return makePositionMatrix(pos) * makeRotationMatrix(rot) * makeScaleMatrix(scale);
}



mat3 calculateFixNormalsMatrix(mat4 const& modelMatrix)
{
	return mat3(glm::transpose(glm::inverse(modelMatrix)));
}



vec3 transformPoint(mat4 const& transformation, vec3 const& point)
{
	return vec3(transformation * vec4(point, 1));
}

vec3 transformDirection(mat4 const& transformation, vec3 const& dir)
{
	return vec3(vec4(dir, 1) * transformation);
}



bool isInsideRange(float value, float min, float max)
{
	return value >= min && value <= max;
}

bool isInsideBox(vec3 const& point, vec3 const& min, vec3 const& max)
{
	return isInsideRange(point.x, min.x, max.x)
		&& isInsideRange(point.y, min.y, max.y)
		&& isInsideRange(point.z, min.z, max.z);
}



// makes a world-space ray direction from a view point in the rect range [-1..1].
vec3 makeRayDirection(vec2 const& viewPoint, mat4 const& vpMatrix)
{
	mat4 const invVP = glm::inverse(vpMatrix);
	vec4 const screenPos = vec4(viewPoint.x, viewPoint.y, 1, 1);
	vec4 const worldPos = invVP * screenPos;
	vec3 const dir = glm::normalize(vec3(worldPos));
	return dir;
}

// converts a screen point (origin bottom-left) to a view point in the rect range [-1..1] (origin centre).
vec2 screenPointToViewPoint(vec2 const& screenPoint, vec2 const& screenHalfSize)
{
	return screenPoint / screenHalfSize - vec2(1.0f);
}



vec3 project(vec3 const& point, vec3 const& linePoint, vec3 const& lineVector)
{
	vec3 const lineDir = glm::normalize(lineVector);
	vec3 const toPoint = point - linePoint;
	vec3 const closestPoint = glm::dot(toPoint, lineDir) * lineDir;
	return closestPoint;
}

// distance: max distance moving towards targetPosition.
vec3 moveTowards(vec3 const& position, vec3 const& targetPosition, float distance)
{
	return position + limit(targetPosition - position, distance);
}

vec3 limit(vec3 const& vec, float length)
{
	float const len = glm::length(vec);
	return (len < length || len <= 0)
		? vec
		: (vec * (length / len)); // normalize(vec) * length
}



bool toleranceEqual(float a, float b, float tolerance)
{
	return std::abs(a - b) <= tolerance;
}

bool toleranceZero(float a, float tolerance)
{
	return std::abs(a) <= tolerance;
}



// Source: https://github.com/g-truc/glm/blob/master/glm/gtc/quaternion.inl
// (newer version of glm)
namespace
{
	GLM_FUNC_QUALIFIER quat quatLookAtRH(vec3 const& direction, vec3 const& up)
	{
		mat3 Result;

		Result[2] = -direction;
		Result[0] = normalize(cross(up, Result[2]));
		Result[1] = cross(Result[2], Result[0]);

		return quat_cast(Result);
}

	GLM_FUNC_QUALIFIER quat quatLookAtLH(vec3 const& direction, vec3 const& up)
	{
		mat3 Result;

		Result[2] = direction;
		Result[0] = normalize(cross(up, Result[2]));
		Result[1] = cross(Result[2], Result[0]);

		return quat_cast(Result);
	}

	GLM_FUNC_QUALIFIER quat quatLookAt(vec3 const& direction, vec3 const& up)
	{
#		if GLM_CONFIG_CLIP_CONTROL & GLM_CLIP_CONTROL_LH_BIT
		return quatLookAtLH(direction, up);
#		else
		return quatLookAtRH(direction, up);
# 		endif
	}
}

quat lookDir(vec3 const& forward, vec3 const& up)
{
	return quatLookAt(glm::normalize(forward), glm::normalize(up));

	//// Source: https://www.gamedev.net/forums/topic/613595-quaternion-lookrotationlookat-up/
	//up = glm::normalize(up);
	//forward = glm::orthonormalize(forward, up);
	//vec3 const right = glm::cross(up, forward);

	//quat q;

	//q.w = sqrtf(1.0f + right.x + up.y + forward.z) * 0.5f;
	//if (toleranceZero(q.w))
	//{
	//	return quat();
	//}
	//float const w4_recip = 1.0f / (4.0f * q.w);

	//q.x = (forward.y - up.z) * w4_recip;
	//q.y = (right.z - forward.x) * w4_recip;
	//q.z = (up.x - right.y) * w4_recip;

	//// to RHS
	//q.y = -q.y;
	////q.x = -q.x;
	////q.z = -q.z;

	//return q;
}
