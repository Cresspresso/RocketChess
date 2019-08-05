/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	mvp_material.hpp
**
**	Summary 	:	Basic material with a Model-View-Projection matrix.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "material.hpp"

// can be used for shaders: phong, blinn_phong, gouraud
struct MvpMaterial : public IMaterial
{
	virtual ~MvpMaterial() = default;

	ReturnCode prepare() override;
	ReturnCode apply(GLuint program) override;
	ReturnCode unapply(GLuint program) override;



	mat4 mvp;
};
