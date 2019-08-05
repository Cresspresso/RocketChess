/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	seek_scene.hpp
**
**	Summary		:	Demo Scene for Seek/Flee algorithm.
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

#include "camera_orbit_entity.hpp"
#include "skybox_entity.hpp"
#include "simple_entity.hpp"
#include "menu_button.hpp"
#include "mover.hpp"
#include "seeker.hpp"



namespace seek_scene
{
	struct Scene
	{
		struct Materials : public BasicMaterials
		{
			RimLightingMaterial corner;
			PhongMaterial mover;
			PhongMaterial seeker;

			void init();
		private:
			struct Builder;

		} materials;



		struct Entities : public BasicSceneEntities
		{
			std::array<SimpleEntity, 4> corners{};
			Mover mover;
			Seeker seeker;

			MenuButtonEntity buttonMode;

		} entities;



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
