/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	cubemap_material.hpp
**
**	Summary 	:	Material for skybox/cubemap.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "material.hpp"

// can be used for shaders: quad4
struct SkyboxMaterial : public IMaterial
{
	virtual ~SkyboxMaterial() = default;

	void prepare() override;
	void apply(GLuint program) override;
	void unapply(GLuint program) override;


private:
	mat4 mvp = {};
public:
	GLuint cubemap = 0; // texture ID
};
