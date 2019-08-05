/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	fix_normals_material.cpp
**
**	Summary 	:	Basic material with Model, FixNormals, and MVP matrices.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "world_math.hpp"
#include "globals.hpp"

#include "nvToolsExt.h"

#include "fix_normals_material.hpp"

ReturnCode FixNormalsMaterial::prepare()
{
	ReturnCode const r = Super::prepare();

	model = g_modelMatrix;
	fixNormals = calculateFixNormalsMatrix(model);

	return r;
}

ReturnCode FixNormalsMaterial::apply(GLuint program)
{
	nvtxRangePush(__FUNCTIONW__);

	ReturnCode const r = Super::apply(program);

	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(glGetUniformLocation(program, "fixNormals"), 1, GL_FALSE, glm::value_ptr(fixNormals));

	nvtxRangePop();

	return r;
}
