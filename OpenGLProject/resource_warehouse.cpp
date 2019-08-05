/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	resource_warehouse.cpp
**
**	Summary 	:	Manages most of the resource managers.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "resource_warehouse.hpp"

ReturnCode ResourceWarehouse::init()
{
	BEGIN_ANYALL();

	DO_ANYALL(programs.init());
	DO_ANYALL(fonts.init());
	DO_ANYALL(textures.init());
	DO_ANYALL(cubemaps.init());
	DO_ANYALL(meshes.init());

	return END_ANYALL();
}

void ResourceWarehouse::destroy() noexcept
{
	cubemaps.destroy();
	textures.destroy();
	fonts.destroy();
	programs.destroy();
}
