/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	application.cpp
**
**	Summary 	:	Scene and resource manager.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "socket.hpp"

#include "console.hpp"
#include "to_string.hpp"

#include "application.hpp"



struct SceneInitter
{
	Application& app;

	ReturnCode operator()(menu_scene::Scene& scene)
	{
		// stop multiplayer if it was active
		//app.networkManager.close();

		return scene.init();
	}
	ReturnCode operator()(robotron_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(lobby_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(multiplayer_scene::Scene& scene) { return scene.init(); }
	ReturnCode operator()(seek_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(pursue_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(wander_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(arrival_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
	ReturnCode operator()(leader_following_scene::Scene& scene) { scene.init(); return RC_SUCCESS; }
};

static auto const destroyer = [](auto& scene) { return scene.destroy(); };

ReturnCode Application::init()
{
	BEGIN_ANYALL();

	//DO_ANYALL(networkManager.init());


	console::log("Loading config file (config.txt)...");
	ReturnCode const c = Config::load(config);
	if (c) { printError(c); }
	DO_ANYALL(c);
	console::log(stringLink("Your username is: '", config.username, "'."));
	console::log(stringLink("Your servername is: '", config.servername, "'."));


	DO_ANYALL(resources.init());

	DO_ANYALL(std::visit(SceneInitter{ *this }, scene));

	return END_ANYALL();
}

void Application::destroy() noexcept
{
	std::visit(destroyer, scene);

	resources.destroy();

	//networkManager.destroy();
}

ReturnCode Application::update()
{
	BEGIN_ANYALL();

	if (newSceneIndex)
	{
		DO_ANYALL(loadScene(*newSceneIndex));
		newSceneIndex = std::nullopt;
	}

	DO_ANYALL(std::visit([](auto& scene) { return scene.update(); }, scene));

	//DO_ANYALL(networkManager.update());

	return END_ANYALL();
}

ReturnCode Application::render()
{
	return std::visit([](auto& scene) { return scene.render(); }, scene);
}

ReturnCode Application::loadScene(SceneIndexer i)
{
	std::visit(destroyer, scene);

	switch (i)
	{
	case SceneIndexer::Menu:		scene.emplace< menu_scene			::Scene>(); break;
	case SceneIndexer::Robotron:	scene.emplace< robotron_scene		::Scene>(); break;
	case SceneIndexer::Lobby:		scene.emplace< lobby_scene			::Scene>(); break;
	case SceneIndexer::Multiplayer:	scene.emplace< multiplayer_scene	::Scene>(); break;
	case SceneIndexer::Seek:		scene.emplace< seek_scene			::Scene>(); break;
	case SceneIndexer::Pursue:		scene.emplace< pursue_scene			::Scene>(); break;
	case SceneIndexer::Wandering:	scene.emplace< wander_scene			::Scene>(); break;
	case SceneIndexer::Arrival:		scene.emplace< arrival_scene		::Scene>(); break;
	case SceneIndexer::LeaderFollowing:	scene.emplace<leader_following_scene::Scene>(); break;
	default: FAIL_RE("invalid SceneIndexer enum value"); break;
	}

	return std::visit(SceneInitter{ *this }, scene);
}
