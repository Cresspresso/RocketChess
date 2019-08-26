/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	application.hpp
**
**	Summary 	:	Scene and resource manager.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <variant>
#include <optional>

#include "scene_indexer.hpp"

#include "resource_warehouse.hpp"
#include "menu_scene.hpp"

#include "config.hpp"

#include "network_manager.hpp"

struct Application
{
	ResourceWarehouse resources;
	std::optional<Scene> scene;



	ReturnCode init();
	void destroy() noexcept;

	ReturnCode update();
	ReturnCode render();
};
