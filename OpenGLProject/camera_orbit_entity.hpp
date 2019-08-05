/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	camera_orbit_entity.hpp
**
**	Summary		:	Camera controller able to be rotated with certain keyboard keys.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "camera.hpp"

struct CameraOrbitControl
{
	float minRadius = 4;
	float maxRadius = 8;
	float fastMultiplier = 3;
	vec3 eulerAngles = vec3(); // in degrees
	vec3 eulerAngleSpeeds = vec3(100); // in degrees per second

	static bool isInputFast();
	static bool isInputLeft();
	static bool isInputRight();
	static bool isInputUp();
	static bool isInputDown();
	static bool isInputZoom();
	void updateEulerAngles(); // polls user input
	float calculateRadius() const;
	vec3 calculateEye() const;
};

struct CameraOrbitEntity
{
	Camera camera;
	CameraOrbitControl control;

	void update(); // polls user input
	void recalculate();
	void useForRendering() const;
};
