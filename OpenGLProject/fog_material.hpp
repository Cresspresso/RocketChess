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

#pragma once

#include "phong_material.hpp"

// can be used for shaders: phong, blinn_phong, gouraud
struct FogMaterial : public PhongMaterial
{
	using Super = PhongMaterial;
	virtual ~FogMaterial() = default;

	ReturnCode apply(GLuint program) override;

	float fogStart = 5.0f;
	float fogRange = 10.0f;
	vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
};
