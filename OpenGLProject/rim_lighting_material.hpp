/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	rim_lighting_material.hpp
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

#pragma once

#include "phong_material.hpp"

// can be used for shaders: rim_lighting
struct RimLightingMaterial : public PhongMaterial
{
	using Super = PhongMaterial;
	~RimLightingMaterial() = default;

	ReturnCode apply(GLuint program) override;



	float rimExponent = 1.0f;
	vec3 rimColor = { 1, 1, 1 };
};
