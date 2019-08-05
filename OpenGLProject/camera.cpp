/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	camera.cpp
**
**	Summary		:	Camera (perspective or orthographic) used for rendering.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "screen.hpp"
#include "globals.hpp"

#include "camera.hpp"



void LookAt::recalculate()
{
	viewMatrix = glm::lookAt(eye, target, up);
}



vec2 Projection::calculateHalfSize() const
{
	return vec2(aspect * halfHeight, halfHeight);
}

mat4 Projection::makeProjectionMatrix() const
{
	if (isOrthographic)
	{
		float const halfWidth = aspect * halfHeight;
		return glm::ortho<float>(-halfWidth, halfWidth, -halfHeight, halfHeight, nearClip, farClip);
	}
	else
	{
		return glm::perspective<float>(fovy, aspect, nearClip, farClip);
	}
}

void Projection::recalculateProjectionMatrix()
{
	projectionMatrix = makeProjectionMatrix();
}

void Projection::recalculateAspect()
{
	aspect = screen::getAspect();
}

void Projection::recalculate()
{
	recalculateAspect();
	recalculateProjectionMatrix();
}



void Camera::recalculateVPMatrix()
{
	vpMatrix = projection.projectionMatrix * transform.viewMatrix;
}

void Camera::recalculate()
{
	transform.recalculate();
	projection.recalculate();
	recalculateVPMatrix();
}

void Camera::useForRendering() const
{
	g_cameraVPMatrix = vpMatrix;
	g_cameraEye = transform.eye;
}
