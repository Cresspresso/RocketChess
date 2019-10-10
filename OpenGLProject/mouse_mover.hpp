/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mouse_mover.hpp
**
**	Summary		:	When the player clicks the LMB, teleports to that click location in world space.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	12/06/2019
*/

#pragma once

#include "camera.hpp"
#include "simple_entity.hpp"



struct MouseMover
{
	SimpleEntity simple;

	void update(); // requires g_cameraVPMatrix
	void render();
private:
	vec3 calculatePosition() const;
};
