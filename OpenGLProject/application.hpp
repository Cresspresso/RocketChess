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
#include "robotron_scene.hpp"
#include "lobby_scene.hpp"
#include "seek_scene.hpp"
#include "pursue_scene.hpp"
#include "wander_scene.hpp"
#include "arrival_scene.hpp"
#include "leader_following_scene.hpp"
#include "multiplayer_scene.hpp"

#include "config.hpp"

#include "network_manager.hpp"

struct Application
{
	//NetworkManager networkManager;

	Config config;

	ResourceWarehouse resources;

	std::variant<
		menu_scene::Scene,
		robotron_scene::Scene,
		lobby_scene::Scene,
		multiplayer_scene::Scene,
		seek_scene::Scene,
		pursue_scene::Scene,
		wander_scene::Scene,
		arrival_scene::Scene,
		leader_following_scene::Scene
	> scene; // currently loaded scene.

	// if not nullopt, will load the scene in the next frame.
	std::optional<SceneIndexer> newSceneIndex = std::nullopt;

	void postLoadSceneEvent(SceneIndexer i) { newSceneIndex = i; }

	/*struct SceneVariant
	{
		menu_scene::Scene menu;
		seek_scene::Scene seek;
		pursue_scene::Scene pursue;
	private:
		SceneIndexer current = SceneIndexer::Menu;

	public:
		ReturnCode init();
		void destroy() noexcept;

		ReturnCode update();
		ReturnCode render();

		ReturnCode load(SceneIndexer i);

	} scene;*/



	ReturnCode init();
	void destroy() noexcept;

	ReturnCode update();
	ReturnCode render();

private:
	ReturnCode loadScene(SceneIndexer i);
};
