/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	menu_scene.hpp
**
**	Summary		:	Main Menu scene. Jumpboard to other scenes.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#pragma once

#include <array>
#include <map>
#include <optional>

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "phong_material.hpp"
#include "quad4_material.hpp"
#include "reflection_material.hpp"
#include "rim_lighting_material.hpp"
#include "fog_material.hpp"

#include "camera_orbit_entity.hpp"
#include "skybox_entity.hpp"
#include "simple_entity.hpp"
#include "text_renderer.hpp"
#include "menu_button.hpp"

#include "Model.h"

#include "ChessPiece.hpp"
#include "ChessAction.hpp"

namespace menu_scene
{
	struct Scene
	{
		struct Materials : public BasicMaterials
		{
			PhongMaterial focusedObject;
			PhongMaterial outline;
			Quad4Material flyby;
			RimLightingMaterial rimLighting;
			ReflectionMaterial reflection;
			TextMaterial infoText;
			FogMaterial foggy;

			void init();
		private:
			struct Builder;

		} materials;



		struct Entities : public BasicSceneEntities
		{
			SimpleEntity focusedObject;
			SimpleEntity outline;
			SimpleEntity flyby;
			SimpleEntity rimLit;
			SimpleEntity reflective;

			//Model model;

			TextRenderer infoText;
			MenuButtonEntity buttonToggleMusic;
			static constexpr size_t N = 7;
			std::array<MenuButtonEntity, N> loadSceneButtons;

			std::array<MenuButtonEntity, 8*8> boardButtons;

			MenuButtonEntity& getButtonQuit() { return buttonMenu; }

		} entities;

		std::array<OwnedChessPiece, 8 * 8> boardPieces;
		std::map<ivec2, ChessAction> availableActions;
		std::optional<ivec2> selectedCoords;
		bool isCurrentPlayerTwo = false;

		ReturnCode onCellClicked(int x, int y);

		ReturnCode init();
		void destroy() noexcept {}

		ReturnCode update();
		ReturnCode render();

	private:
		ReturnCode renderWorld();
		ReturnCode renderHud();

		struct Builder;
	};
}
