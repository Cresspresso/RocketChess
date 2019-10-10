/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	fix_normals_material.hpp
**
**	Summary 	:	Basic material with Model, FixNormals, and MVP matrices.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "mvp_material.hpp"

// can be used for shaders: phong, blinn_phong, gouraud
struct FixNormalsMaterial : public MvpMaterial
{
	using Super = MvpMaterial;
	virtual ~FixNormalsMaterial() = default;

	void prepare() override;
	void apply(GLuint program) override;



	mat4 model;
	mat3 fixNormals;
};
