/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	arrival_scene.cpp
**
**	Summary		:	Demo Scene for Arrival algorithm.
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

#include "arrival_scene.hpp"



namespace arrival_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initCorner(RimLightingMaterial& material) const;
		void initMouseMover(PhongMaterial& material) const;
		void initSeeker(PhongMaterial& material) const;
		void initTextInfo(TextMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initCorner(materials.corner);
		initMouseMover(materials.mouseMover);
		initSeeker(materials.seeker);
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

	void Scene::Materials::Builder::initMouseMover(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 0, 0);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 128;
	}

	void Scene::Materials::Builder::initSeeker(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initTextInfo(TextMaterial& material) const
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
		// world
		void initCorners(std::array<SimpleEntity, 4>& corners) const;
		void initCorner(SimpleEntity& entity) const;
		void initMouseMover(MouseMover& entity) const;
		void initSeeker(Seeker& entity) const;
		// hud
		void initTextInfo(TextRenderer& entity) const;
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
		initMouseMover(entities.mouseMover);
		initSeeker(entities.seeker);

		initTextInfo(entities.textInfo);
	}



	void Scene::Builder::initCorners(std::array<SimpleEntity, 4>& corners) const
	{
		constexpr float a = 5;
		static std::array<vec3, 4> const points = {
			vec3(-a, 0, -a),
			vec3(a, 0, -a),
			vec3(a, 0, a),
			vec3(-a, 0, a),
		};

		for (size_t i = 0; i < corners.size(); ++i)
		{
			auto& e = corners[i];
			initCorner(e);
			e.transform.localPosition = points[i];
		}
	}

	void Scene::Builder::initCorner(SimpleEntity & entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::RimLighting].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.corner);
	}

	void Scene::Builder::initMouseMover(MouseMover & entity) const
	{
		auto& r = entity.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.mouseMover);

		auto& t = entity.simple.transform;
		t.localScale = vec3(0.1f, 0.8f, 0.1f);
	}

	void Scene::Builder::initSeeker(Seeker& entity) const
	{
		entity.forceScale = 4;
		entity.maxForce = 3.1f;
		entity.maxSpeed = 2;
		entity.arrivingRadius = 1.5f;

		auto& r = entity.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.seeker);

		auto& t = entity.simple.transform;
		t.localPosition = vec3(-4, 0, -2);
		t.localScale = vec3(0.3f, 0.3f, 0.7f);
	}

	void Scene::Builder::initTextInfo(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.textInfo));

		entity.text = "Left Click to set target position.";
		entity.scale = vec2(1.2f);
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
			e.cameraWorld.useForRendering();

			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.update());
			}

			DO_ANYALL(e.mouseMover.update());

			e.seeker.targetPosition = e.mouseMover.simple.transform.localPosition;
			DO_ANYALL(e.seeker.update());

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
				//vec2 const offset = vec2(buttonHalfSize.x * 2, 0);

				e.textInfo.position = vec2(-hudHalfSize.x + buttonHalfSize.x * 2 + 20, hudHalfSize.y - 60);
			}
			e.cameraHud.useForRendering();
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
			DO_ANYALL(e.mouseMover.render());
			DO_ANYALL(e.seeker.render());
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
			DO_ANYALL(e.textInfo.render());
			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



#pragma endregion ~Scene
}
