/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	rigidbody.cpp
**
**	Summary		:	Simulates velocity by updating the transform's position every frame.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "time.hpp"

#include "rigidbody.hpp"

ReturnCode Rigidbody::update()
{
	ASSERT1(transform);
	float const dt = Time::getDeltaTime();
	transform->localPosition += localVelocity * dt;
	transform->localRotation = quat(localAngularVelocity * dt) * transform->localRotation;

	return RC_SUCCESS;
}

//ReturnCode Rigidbody::addForce(vec3 const& force)
//{
//	// calculate acceleration vector.
//	ASSERT1(!toleranceZero(mass));
//	vec3 const acceleration = force / mass;
//
//	// apply acceleration and calculate new velocity.
//	vec3 newVelocity = localVelocity + acceleration * Time::getDeltaTime();
//	newVelocity = limit(newVelocity, maxSpeed);
//
//	localVelocity = newVelocity;
//
//	return RC_SUCCESS;
//}
