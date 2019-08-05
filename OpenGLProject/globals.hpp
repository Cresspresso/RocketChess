/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	globals.hpp
**
**	Summary 	:	Global variables (used when rendering).
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

// current camera
extern vec3 g_cameraEye;
extern mat4 g_cameraVPMatrix;

// current renderer
extern mat4 g_modelMatrix;
extern mat4 g_mvp;

// current light
extern vec3 g_lightPos;
