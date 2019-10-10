/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	quad4_material.hpp
**
**	Summary 	:	Unlit material.
**					Works with programs:
**						quad4
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "material.hpp"

struct Quad4Material : public IMaterial
{
	virtual ~Quad4Material() = default;

	void prepare() override;
	void apply(GLuint program) override;
	void unapply(GLuint program) override;



	mat4 mvp = {};
	GLuint tex1 = 0;
};
