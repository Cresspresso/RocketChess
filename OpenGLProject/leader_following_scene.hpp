/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	leader_following_scene.hpp
**
**	Summary		:	Demo Scene for Leader Following behaviour.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	12/06/2019
*/

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
#include "pursuer.hpp"
#include "mouse_mover.hpp"



namespace leader_following_scene
{
	struct Scene
	{
		struct Materials : public BasicMaterials
		{
			RimLightingMaterial corner;
			PhongMaterial mouseMover;
			PhongMaterial leader;
			PhongMaterial follower;
			PhongMaterial targetPos;

			TextMaterial textInfo;

			void init();
		private:
			struct Builder;

		} materials;



		struct Entities : public BasicSceneEntities
		{
			std::array<SimpleEntity, 4> corners{};
			MouseMover mouseMover;
			Seeker leader;
			std::array<Pursuer, 20> followers;
			SimpleEntity targetPos;

			TextRenderer textInfo;

		} entities;



		void init();
		void destroy() noexcept {}

		ReturnCode update();
		ReturnCode render();

	private:
		ReturnCode renderWorld();
		ReturnCode renderHud();

		ReturnCode updateFollowers();
		vec3 calculateLeaderFollowPos() const;

		struct Builder;
	};
}
