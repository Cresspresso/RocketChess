/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	fog_material.cpp
**
**	Summary 	:	Material for linear forg program.
**					Works with programs:
**						linear_fog
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	23/07/2019
*/

#include "globals.hpp"

#include "nvToolsExt.h"

#include "fog_material.hpp"

void FogMaterial::apply(GLuint program)
{
	nvtxRangePush(__FUNCTIONW__);

	Super::apply(program);

	glUniform1f(glGetUniformLocation(program, "fogStart"), fogStart);
	glUniform1f(glGetUniformLocation(program, "fogRange"), fogRange);
	glUniform4f(glGetUniformLocation(program, "fogColor"), fogColor.x, fogColor.y, fogColor.z, fogColor.w);

	nvtxRangePop();
}
