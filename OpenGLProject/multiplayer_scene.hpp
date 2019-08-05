
#pragma once

#include <array>

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "quad4_material.hpp"
#include "phong_material.hpp"
#include "font.hpp"
#include "rim_lighting_material.hpp"

#include "camera_orbit_entity.hpp"
#include "skybox_entity.hpp"
#include "simple_entity.hpp"
#include "menu_button.hpp"
#include "player.hpp"
#include "seeker_enemy.hpp"



namespace multiplayer_scene
{
	struct Scene
	{
		struct Materials : public BasicMaterials
		{
			RimLightingMaterial corner;
			PhongMaterial ground;
			PhongMaterial player;
			PhongMaterial enemy;
			TextMaterial textLabel;
			TextMaterial textGameOver;

			void init();
		private:
			struct Builder;

		} materials;



		struct Entities : public BasicSceneEntities
		{
			std::array<SimpleEntity, 4> corners{};
			SimpleEntity ground;
			Player player;
			SeekerEnemy enemy;
			std::vector<SimpleEntity> liveBullets;

			TextRenderer textScore;
			TextRenderer textLives;
			TextRenderer textGameOver;

		} entities;

		BoxRange bulletLimits = { vec3(-10, -2, -10), vec3(10, 2, 10) };



		ReturnCode init();
		void destroy() noexcept {}

		ReturnCode update();
		ReturnCode render();

	private:
		void initBullet(SimpleEntity& entity);

		ReturnCode updateEnemy();
		ReturnCode updatePlayerBullets();
		void updateHud();

		ReturnCode renderWorld();
		ReturnCode renderHud();

		struct Builder;
	};
}
