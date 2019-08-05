/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	camera.hpp
**
**	Summary		:	Camera (perspective or orthographic) used for rendering.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"
#include "transform.hpp"

struct LookAt
{
	vec3 eye = vec3(0, 0, 4);
	vec3 target = vec3();
	vec3 up = constants::up;

	mat4 viewMatrix = mat4();

	void recalculate();
};

struct Projection
{
	float nearClip = 0.1f;
	float farClip = 5000.0f;
	bool isOrthographic = false;
	float fovy = glm::radians(60.f); // perspective vertical field of view
	float halfHeight = 1; // orthographic view height / 2
	float aspect = 1; // screen aspect

	mat4 projectionMatrix = mat4();

	void recalculateAspect(); // gets global variable g_aspect
	vec2 calculateHalfSize() const; // orthographic view size / 2
	mat4 makeProjectionMatrix() const;
	void recalculateProjectionMatrix();
	void recalculate();
};

struct Camera
{
	Projection projection;
	LookAt transform;

	mat4 vpMatrix = mat4();

	void recalculateVPMatrix(); // uses projectionMatrix, viewMatrix
	void recalculate(); // recalculates all properties
	void useForRendering() const; // uses vpMatrix, transform; sets global variables g_cameraVPMatrix, g_cameraEye
};
