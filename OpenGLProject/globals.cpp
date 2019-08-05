/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	globals.cpp
**
**	Summary 	:	Global variables (used when rendering).
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "globals.hpp"

// current camera
vec3 g_cameraEye = { 0, 0, 4 };
mat4 g_cameraVPMatrix = {};

// current renderer
mat4 g_modelMatrix = {};
mat4 g_mvp = {};

// current light
vec3 g_lightPos = { -2, 6, 3 };
