/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	lobby_scene.cpp
**
**	Summary		:	Lobby for joining or hosting multiplayer servers.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "entity_utils.hpp"

#include "singleton.hpp"
#include "screen.hpp"
#include "input.hpp"

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "socket.hpp"

#include "lobby_scene.hpp"



namespace lobby_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initNames(TextMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initNames(materials.names);
	}



	void Scene::Materials::Builder::initNames(TextMaterial& material) const
	{
		material.tint = vec3(0.9f);
	}



#pragma endregion ~Scene::Materials::Builder
#pragma region Scene::Materials



	void Scene::Materials::init()
	{
		Builder{ *this }.build();
	}



#pragma endregion ~Scene::Materials
#pragma region Scene::Builder



	struct Scene::Builder : public BasicSceneBuilder
	{
		Scene& scene;
		Entities& entities;
		Materials& materials;

		Builder(Scene& scene);

		void build() const;

	private:
		// hud
		void initHostServerButton(MenuButtonEntity& entity) const;
		void initJoinOrLeaveToggle(MenuButtonEntity& entity) const;
		void initReadyToggle(MenuButtonEntity& entity) const;
		void initStartGameButton(MenuButtonEntity& entity) const;
		void initServerName(TextRenderer& entity) const;
		void initJoinedClients(std::array<JoinedClient, 4>& entities) const;
		void initJoinedClient(JoinedClient& entity) const;
		void initAvailableServerButtons(std::vector<MenuButtonEntity>& entities) const;
		void initAvailableServerButton(MenuButtonEntity& entity, size_t serverID) const;
	};



	Scene::Builder::Builder(Scene& scene) :
		BasicSceneBuilder{ scene.materials },
		scene{ scene },
		entities{ scene.entities },
		materials{ scene.materials }
	{}



	void Scene::Builder::build() const
	{
		BasicSceneBuilder::build(entities);

		initHostServerButton(entities.hostServerButton);
		initJoinOrLeaveToggle(entities.joinOrLeaveToggle);
		initReadyToggle(entities.readyToggle);
		initStartGameButton(entities.startGameButton);
		initServerName(entities.serverName);
		initJoinedClients(entities.joinedClients);
		initAvailableServerButtons(entities.availableServerButtons);
	}

	void Scene::Builder::initHostServerButton(MenuButtonEntity & entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('h');
		button.onClickLeft.hotkeys.push_back('H');
		button.onClickLeft.action = []
		{
			ASSERT0_RE(singleton::isNetworkOpen(), "MULTIPLAYER: You are already joined to a server.");
			singleton::openHost();
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Host (H)";
			tx.position.x += 20;
		}
	}

	void Scene::Builder::initJoinOrLeaveToggle(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('j');
		button.onClickLeft.hotkeys.push_back('J');
		auto& scene = this->scene;
		button.onClickLeft.action = [&button, &scene]
		{
			if (singleton::isNetworkOpen())
			{
				scene.ready = false;
				singleton::closeNetwork();
				return RC_SUCCESS;
			}
			else
			{
				scene.ready = false;
				singleton::openClient();
				return RC_SUCCESS;
			}
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.position.x += 20;
		}
	}

	void Scene::Builder::initReadyToggle(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('r');
		button.onClickLeft.hotkeys.push_back('R');
		auto& scene = this->scene;
		button.onClickLeft.action = [&scene]
		{
			ASSERT1_RE(singleton::isNetworkOpen(), "MULTIPLAYER: You are not joined to a server.");
			scene.ready = !scene.ready;
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.position.x += 20;
		}
	}

	void Scene::Builder::initStartGameButton(MenuButtonEntity& entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('b');
		button.onClickLeft.hotkeys.push_back('B');
		auto& scene = this->scene;
		button.onClickLeft.action = [&scene]
		{
			bool allReady = std::all_of(scene.entities.joinedClients.begin(), scene.entities.joinedClients.end(),
				[](auto const& client) { return client.ready; }
			);
			ASSERT1_RE(allReady, "MULTIPLAYER: Not all players are ready to start.");
			singleton::postLoadSceneEvent(SceneIndexer::Multiplayer);
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "Start Game";
			tx.position.x += 20;
		}
	}

	void Scene::Builder::initServerName(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.names));

		entity.text = "<Server Name>";
	}

	void Scene::Builder::initJoinedClients(std::array<JoinedClient, 4>& entities) const
	{
		for (auto& entity : entities)
		{
			initJoinedClient(entity);
		}
	}

	void Scene::Builder::initJoinedClient(JoinedClient& entity) const
	{
		auto& name = entity.name;
		initTextRenderer(name, &(materials.names));
		name.text = "<Player Name>";

		auto& rt = entity.readyText;
		initTextRenderer(rt, &(materials.names));
	}

	void Scene::Builder::initAvailableServerButtons(std::vector<MenuButtonEntity>& entities) const
	{
		entities.resize(3);
		for (size_t i = 0; i < entities.size(); i++)
		{
			initAvailableServerButton(entities[i], i);
		}
	}

	void Scene::Builder::initAvailableServerButton(MenuButtonEntity& entity, size_t serverID) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		auto& scene = this->scene;
		button.onClickLeft.action = [&scene, serverID]
		{
			ASSERT1_RE(singleton::isNetworkOpen(), "MULTIPLAYER: You are already joined to a server.");
			scene.selectedServerID = serverID;
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			tx.text = "<Available Server Name>";
			tx.position.x += 20;
		}
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	void Scene::init()
	{
		materials.init();
		Builder{ *this }.build();
	}

	

	ReturnCode Scene::update()
	{
		BEGIN_ANYALL();
		{
			auto& e = entities;

			e.cameraWorld.update();

			// hud

			// button horizontal layout at top-left corner of hud
			{
				vec2 const hudHalfSize = e.cameraHud.projection.calculateHalfSize();
				vec3 const buttonHalfSize = 0.5f
					* e.buttonMenu.button.backgroundTransform.localScale
					* e.buttonMenu.scaleHovered;
				vec2 const menuButtonPos = vec2(
					-hudHalfSize.x + buttonHalfSize.x,
					hudHalfSize.y - buttonHalfSize.y
				);

				// first column
				e.buttonMenu.button.transform.localPosition = vec3(menuButtonPos, 0);
				vec2 const offset = vec2(0, -buttonHalfSize.y * 2);
				size_t i = 1;
				e.hostServerButton.button.transform.localPosition = vec3(menuButtonPos + offset * (float)(i++), 0);
				e.joinOrLeaveToggle.button.transform.localPosition = vec3(menuButtonPos + offset * (float)(i++), 0);
				e.readyToggle.button.transform.localPosition = vec3(menuButtonPos + offset * (float)(i++), 0);
				e.startGameButton.button.transform.localPosition = vec3(menuButtonPos + offset * (float)(i++), 0);

				// second column
				vec2 const offsetHorz = vec2(buttonHalfSize.x * 2, 0);
				vec2 const start2 = menuButtonPos + offsetHorz;
				i = 0;
				for (auto& button : e.availableServerButtons)
				{
					button.button.transform.localPosition = vec3(start2 + offset * (float)(i++), 0);
				}

				// third column
				vec2 const start3 = menuButtonPos + offsetHorz * 2.f;
				e.serverName.position = start3;
				i = 1;
				for (auto& client : e.joinedClients)
				{
					client.name.position = (start3 + offset * (float)(i++));
					client.readyText.position = client.name.position + vec2(buttonHalfSize.x * 1.5f, 0);
				}
			}

			DO_ANYALL(e.buttonMenu.update());

			DO_ANYALL(e.hostServerButton.update());
			DO_ANYALL(e.joinOrLeaveToggle.update());
			DO_ANYALL(e.readyToggle.update());
			DO_ANYALL(e.startGameButton.update());

			{
				size_t i = 0;
				for (auto& button : e.availableServerButtons)
				{
					bool isSelected = NetworkState::Host != singleton::getNetworkState() && i == selectedServerID;
					button.scaleNormal = vec3(isSelected ? 1.2f : 1.0f);
					DO_ANYALL(button.update());
					++i;
				}
			}
		}
		return END_ANYALL();
	}



	ReturnCode Scene::render()
	{
		BEGIN_ANYALL();
		{
			DO_ANYALL(renderWorld());
			DO_ANYALL(renderHud());
		}
		return END_ANYALL();
	}



	ReturnCode Scene::renderWorld()
	{
		auto& e = entities;

		e.cameraWorld.recalculate();
		e.cameraWorld.useForRendering();

		BEGIN_ANYALL();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		{
			DO_ANYALL(e.skybox.render());
		}
		glDisable(GL_DEPTH_TEST);

		return END_ANYALL();
	}



	ReturnCode Scene::renderHud()
	{
		auto& e = entities;

		e.cameraHud.recalculate();
		e.cameraHud.useForRendering();

		BEGIN_ANYALL();
		{
			for (auto& button : e.availableServerButtons)
			{
				DO_ANYALL(button.render());
			}

			DO_ANYALL(e.hostServerButton.render());

			// joinOrLeaveToggle
			{
				e.joinOrLeaveToggle.button.text.text = singleton::isNetworkOpen()
					? "Leave (J)"
					: "Join (J)";
				DO_ANYALL(e.joinOrLeaveToggle.render());
			}

			// readyToggle
			{
				e.readyToggle.button.text.text = ready
					? "Ready: [Yes] No (R)"
					: "Ready: Yes [No] (R)";
				DO_ANYALL(e.readyToggle.render());
			}

			DO_ANYALL(e.startGameButton.render());

			DO_ANYALL(e.serverName.render());

			e.joinedClients[0].name.text = singleton::getUsername();
			e.joinedClients[0].ready = ready;

			for (auto& client : e.joinedClients)
			{
				DO_ANYALL(client.name.render());

				client.readyText.text = client.ready ? "Ready" : "Not Ready";
				DO_ANYALL(client.readyText.render());
			}

			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



#pragma endregion ~Scene
}
