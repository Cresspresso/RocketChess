/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	program.hpp
**
**	Summary		:	Loads OpenGL Programs.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#pragma once

//#include <memory>

#include "common.hpp"
#include "shader.hpp"

struct Program
{
	GLuint program = 0; // program
	GLuint verts = 0; // vertex shader
	GLuint frags = 0; // fragment shader

	void destroy() noexcept;
};

ReturnCode loadProgram(Program& p, std::string const& name);
//ReturnCode loadProgram(std::unique_ptr<Program>& p, std::string const& name);

//inline GLuint programOf(std::unique_ptr<Program>& p) noexcept { return p ? p->program : 0; }
