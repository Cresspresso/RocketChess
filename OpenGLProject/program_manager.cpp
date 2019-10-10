/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	program_manager.cpp
**
**	Summary		:	Collection of programs to be loaded at runtime.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "program_manager.hpp"

void ProgramManager::load(Resource& out, size_t i)
{
	return loadProgram(out, names[i]);
}

void ProgramManager::destroyResource(Resource& resource) noexcept
{
	resource.destroy();
}
