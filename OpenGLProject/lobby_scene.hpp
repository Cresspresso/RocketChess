/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	lobby_scene.hpp
**
**	Summary		:	Lobby for joining or hosting multiplayer servers.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <array>

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "quad4_material.hpp"
#include "phong_material.hpp"
#include "font.hpp"
#include "rim_lighting_material.hpp"

#include "simple_entity.hpp"
#include "menu_button.hpp"



namespace lobby_scene
{
	struct Scene
	{
		struct Materials : public BasicMaterials
		{
			TextMaterial names;

			void init();
		private:
			struct Builder;

		} materials;


		struct JoinedClient
		{
			TextRenderer name;
			bool ready = false;
			TextRenderer readyText;
		};

		struct Entities : public BasicSceneEntities
		{
			MenuButtonEntity hostServerButton;
			MenuButtonEntity joinOrLeaveToggle;
			MenuButtonEntity readyToggle;
			MenuButtonEntity startGameButton;
			TextRenderer serverName;
			std::array<JoinedClient, 4> joinedClients;
			std::vector<MenuButtonEntity> availableServerButtons;

		} entities;

		bool ready = false;
		size_t selectedServerID = 0;



		void init();
		void destroy() noexcept {}

		ReturnCode update();
		ReturnCode render();

	private:
		ReturnCode renderWorld();
		ReturnCode renderHud();

		struct Builder;
	};
}
