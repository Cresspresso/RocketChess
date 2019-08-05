/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	wander_scene.cpp
**
**	Summary		:	Demo Scene for Wandering algorithm.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	12/06/2019
*/

#include "entity_utils.hpp"

#include "screen.hpp"
#include "input.hpp"

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "wander_scene.hpp"



namespace wander_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initCorner(RimLightingMaterial& material) const;
		void initWanderer(PhongMaterial& material) const;
		void initTargetPos(PhongMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initCorner(materials.corner);
		initWanderer(materials.wanderer);
		initTargetPos(materials.targetPos);
	}



	void Scene::Materials::Builder::initCorner(RimLightingMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::CapguyWalk];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
		material.rimColor = vec3(0, 0, 1);
		material.rimExponent = 2;
	}

	void Scene::Materials::Builder::initWanderer(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initTargetPos(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0, 0.9f, 0.1f);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
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
		// world
		void initCorners(std::array<SimpleEntity, 4>& corners) const;
		void initCorner(SimpleEntity& entity) const;
		void initWanderer(Wanderer& entity) const;
		void initTargetPos(SimpleEntity& entity) const;
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

		initCorners(entities.corners);
		initWanderer(entities.wanderer);
		initTargetPos(entities.targetPos);
	}



	void Scene::Builder::initCorners(std::array<SimpleEntity, 4>& corners) const
	{
		constexpr float a = 5.5f;
		/*static auto const points = std::array{
			vec3(-a, 0, -a),
			vec3(a, 0, -a),
			vec3(a, 0, a),
			vec3(-a, 0, a),
		};*/
		static vec3 const points[] = {
			vec3(-a, 0, 0),
			vec3(0, 0, -a),
			vec3(a, 0, 0),
			vec3(0, 0, a),
		};

		for (size_t i = 0; i < corners.size(); ++i)
		{
			auto& e = corners[i];
			initCorner(e);
			e.transform.localPosition = points[i];
			e.transform.localRotation = quat(glm::radians(vec3(0, i * 90, 0)));
		}
	}

	void Scene::Builder::initCorner(SimpleEntity & entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::RimLighting].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.corner);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f, 1, 5);
	}

	void Scene::Builder::initWanderer(Wanderer& entity) const
	{
		entity.distance = 0.5f;
		entity.radius = 0.3f;
		entity.threshold = 0.2f;

		auto& pl = entity.positionLimits;
		pl.min = vec3(-5,0,-5);
		pl.max = -pl.min;

		auto& s = entity.seeker;
		s.forceScale = 1.5f;
		s.maxForce = 2.0f;
		s.maxSpeed = 1;
		s.arrivedRadius = entity.threshold * 0.5f;

		auto& r = s.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.wanderer);

		auto& t = s.simple.transform;
		t.localPosition = vec3(0, 0, 0);
		t.localScale = 0.7f * vec3(0.3f, 0.3f, 0.7f);
	}

	void Scene::Builder::initTargetPos(SimpleEntity & entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.targetPos);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f, 0.3f, 0.1f);
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
			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.update());
			}
			DO_ANYALL(e.wanderer.update());

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
				e.buttonMenu.button.transform.localPosition = vec3(menuButtonPos, 0);
				//vec2 const offset = vec2(0, buttonHalfSize.y * 2);
			}
			DO_ANYALL(e.buttonMenu.update());
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

			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.render());
			}
			DO_ANYALL(e.wanderer.render());

			e.targetPos.transform.localPosition = e.wanderer.seeker.targetPosition;
			DO_ANYALL(e.targetPos.render());
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
			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



#pragma endregion ~Scene
}
