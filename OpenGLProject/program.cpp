/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	program.cpp
**
**	Summary		:	Loads OpenGL Programs.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/
#include "program.hpp"

void Program::destroy() noexcept
{
	if (program) { glDeleteProgram(program); program = 0; }
	if (frags) { glDeleteShader(frags); frags = 0; }
	if (verts) { glDeleteShader(verts); verts = 0; }
}

ReturnCode loadProgram(Program& p, std::string const& name)
{
	return loadProgram(&p.program, &p.verts, &p.frags, name);
}
