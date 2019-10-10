/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	phong_material.hpp
**
**	Summary 	:	Material for Full Phong lighting model.
**					Works with programs:
**						phong
**						blinn_phong
**						gouraud
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "fix_normals_material.hpp"

// can be used for shaders: phong, blinn_phong, gouraud
struct PhongMaterial : public FixNormalsMaterial
{
	using Super = FixNormalsMaterial;
	virtual ~PhongMaterial() = default;

	void prepare() override;
	void apply(GLuint program) override;
	void unapply(GLuint program) override;



	vec3 eye = { 0, 0, 4 };

	vec3 ambientColor = vec3(0.05f);
	vec3 lightColor = vec3(1);
	vec3 lightPos = { -2,6,3 };
	float lightSpecularStrength = 1;

	GLuint tex0 = 0;
	float shininess = 32;
};
