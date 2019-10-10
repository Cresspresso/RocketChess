/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	reflection_material.hpp
**
**	Summary 	:	Fully reflecive material.
**					Works with programs:
**						reflection
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "fix_normals_material.hpp"

struct ReflectionMaterial : public FixNormalsMaterial
{
	using Super = FixNormalsMaterial;
	virtual ~ReflectionMaterial() = default;

	void prepare() override;
	void apply(GLuint program) override;
	void unapply(GLuint program) override;



	vec3 eye = { 0, 0, 4 };
	GLuint skybox = 0;
};
