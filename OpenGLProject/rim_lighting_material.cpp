/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	rim_lighting_material.cpp
**
**	Summary 	:	Blinn-Phong material with extra Rim Lighting properties.
**					Works with programs:
**						rim_lighting
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "rim_lighting_material.hpp"

ReturnCode RimLightingMaterial::apply(GLuint program)
{
	ReturnCode const r = Super::apply(program);

	glUniform1f(glGetUniformLocation(program, "rimExponent"), rimExponent);
	glUniform3f(glGetUniformLocation(program, "rimColor"), rimColor.x, rimColor.y, rimColor.z);

	return r;
}
