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

void ResourceWarehouse::init()
{
	try {
		programs.init();
	}
	catch (...) { printException(); }

	try {
		fonts.init();
	}
	catch (...) { printException(); }

	try {
		textures.init();
	}
	catch (...) { printException(); }

	try {
		meshes.init();
	}
	catch (...) { printException(); }
}

void ResourceWarehouse::destroy() noexcept
{
	meshes.destroy();
	textures.destroy();
	fonts.destroy();
	programs.destroy();
}
