/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	resource_warehouse.hpp
**
**	Summary 	:	Manages most of the resource managers.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include "program_manager.hpp"
#include "font_manager.hpp"
#include "texture_manager.hpp"
#include "mesh_manager.hpp"

struct ResourceWarehouse
{
	ProgramManager programs;
	FontManager fonts;
	TextureManager textures;
	MeshManager meshes;

	void init();
	void destroy() noexcept;
};
