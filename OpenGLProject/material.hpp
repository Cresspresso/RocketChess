/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	material.hpp
**
**	Summary 	:	A material sends uniform values to the shader program before rendering.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "common.hpp"

struct IMaterial
{
	virtual ~IMaterial() = default;

	virtual ReturnCode prepare(/* TODO render event args */) = 0;
	virtual ReturnCode apply(GLuint program) = 0;
	virtual ReturnCode unapply(GLuint program) = 0;
};
