/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	camera_orbit_entity.cpp
**
**	Summary		:	Camera controller able to be rotated with certain keyboard keys.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

//#include "globals.hpp"
#include "math_utils.hpp"
#include "time.hpp"

#include "input.hpp"

#include "camera_orbit_entity.hpp"

bool CameraOrbitControl::isInputFast()
{
	return isKeyboardKeyDown(' ');
}

bool CameraOrbitControl::isInputLeft()
{
	return isKeyboardKeyDown('j') || isKeyboardKeyDown('J');
}

bool CameraOrbitControl::isInputRight()
{
	return isKeyboardKeyDown('l') || isKeyboardKeyDown('L');
}

bool CameraOrbitControl::isInputUp()
{
	return isKeyboardKeyDown('i') || isKeyboardKeyDown('I');
}

bool CameraOrbitControl::isInputDown()
{
	return isKeyboardKeyDown('k') || isKeyboardKeyDown('K');
}

bool CameraOrbitControl::isInputZoom()
{
	return isMouseButtonDown(MOUSE_MIDDLE);
}

float CameraOrbitControl::calculateRadius() const
{
	return isInputZoom() ? maxRadius : minRadius;
}

vec3 CameraOrbitControl::calculateEye() const
{
	return quat(glm::radians(eulerAngles))
		* vec3(0, 0, calculateRadius());
}

void CameraOrbitControl::updateEulerAngles()
{
	// apply user input
	// orbit the camera around the target

	vec3 timedSpeeds = eulerAngleSpeeds * Time::getDeltaTime();

	if (isInputFast())
	{
		timedSpeeds *= fastMultiplier;
	}

	// vertical (euler X axis)
	if (isInputDown())
	{
		eulerAngles.x -= timedSpeeds.x;
	}
	if (isInputUp())
	{
		eulerAngles.x += timedSpeeds.x;
	}
	// horizontal (euler Y axis)
	if (isInputLeft())
	{
		eulerAngles.y -= timedSpeeds.y;
	}
	if (isInputRight()) //|| isSpecialKeyDown(GLUT_KEY_RIGHT)
	{
		eulerAngles.y += timedSpeeds.y;
	}
	//// roll (euler Z axis)
	//if (isKeyboardKeyDown('u') || isKeyboardKeyDown('U'))
	//{
	//	eulerAngles.z -= timedSpeeds.z;
	//}
	//if (isKeyboardKeyDown('o') || isKeyboardKeyDown('O'))
	//{
	//	eulerAngles.z += timedSpeeds.z;
	//}

	// Ensure angles are valid.
	eulerAngles.x = clamp(eulerAngles.x, -89.9f, 89.9f); // vertical
	eulerAngles.y = cycle(eulerAngles.y, 360.f); // horizontal
	//eulerAngles.z = cycle(eulerAngles.z, 360.f); // roll
}



void CameraOrbitEntity::update()
{
	control.updateEulerAngles();
}

void CameraOrbitEntity::recalculate()
{
	camera.transform.eye = control.calculateEye();
	camera.recalculate();
}

void CameraOrbitEntity::useForRendering() const
{
	return camera.useForRendering();
}
