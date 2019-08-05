/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	renderer.hpp
**
**	Summary		:	Renders a mesh with a shader program and a material.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "material.hpp"
#include "drawable.hpp"

struct Renderer
{
	ReturnCode render();


	GLuint program = 0;
	IMaterial* material = nullptr;
	IDrawable* mesh = nullptr;
	mat4 modelMatrix = mat4();
};
