/*
**	Bachelor of Software Engineering
**	Media Design School
**	Auckland
**	New Zealand
**
**	(c) 2019 Media Design School
**
**	File		:	pursue_scene.cpp
**
**	Summary		:	Demo Scene for Pursue/Evade algorithm.
**
**	Project		:	Robotron Summative
**	Author		:	Elijah Shadbolt
**	Email		:	elijah.sha7979@mediadesign.school.nz
**	Date Edited	:	10/06/2019
*/

#include "entity_utils.hpp"

#include "screen.hpp"
#include "input.hpp"

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "pursue_scene.hpp"



namespace pursue_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initCorner(RimLightingMaterial& material) const;
		void initMover(PhongMaterial& material) const;
		void initPursuer(PhongMaterial& material) const;
		void initTargetPos(PhongMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initCorner(materials.corner);
		initMover(materials.mover);
		initPursuer(materials.pursuer);
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

	void Scene::Materials::Builder::initMover(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 0, 0);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 128;
	}

	void Scene::Materials::Builder::initPursuer(PhongMaterial& material) const
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
		void initMover(Mover& entity) const;
		void initPursuer(Pursuer& entity) const;
		void initTargetPos(SimpleEntity& entity) const;

		// hud
		void initButtonMode(MenuButtonEntity& entity) const;
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
		initMover(entities.mover);
		initPursuer(entities.pursuer);
		initTargetPos(entities.targetPos);

		initButtonMode(entities.buttonMode);
	}



	void Scene::Builder::initCorners(std::array<SimpleEntity, 4>& corners) const
	{
		constexpr float a = 5;
		static vec3 const points[] = {
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

	void Scene::Builder::initMover(Mover & entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.mover);

		auto& points = entity.points;
		points.push_back(vec3(-3, 0, -3));
		points.push_back(vec3(3, 0, -3));
		points.push_back(vec3(3, 0, 3));
		points.push_back(vec3(-3, 0, 3));

		auto & t = entity.transform;
		t.localPosition = points[0];
		t.localScale = vec3(0.3f, 1, 0.3f);
	}

	void Scene::Builder::initPursuer(Pursuer& entity) const
	{
		entity.targetScales.velocity = 0;
		entity.targetScales.direction = 0;
		entity.distanceScales.velocity = 0.9f;
		entity.distanceScales.direction = 0;

		auto& s = entity.seeker;
		s.maxSpeed = 1.1f * entities.mover.speed;
		s.forceScale = 2;
		s.maxForce = 3;

		auto& r = s.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.pursuer);

		auto& t = s.simple.transform;
		t.localPosition = vec3(0, 0, 0);
		t.localScale = vec3(0.3f, 0.3f, 0.7f);
	}

	void Scene::Builder::initTargetPos(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.targetPos);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f);
	}



	void Scene::Builder::initButtonMode(MenuButtonEntity & entity) const
	{
		auto& button = entity.button;
		initButton(button);

		// set callback
		button.onClickLeft.hotkeys.push_back('e');
		button.onClickLeft.hotkeys.push_back('E');
		Pursuer& pursuer = entities.pursuer;
		button.onClickLeft.action = [&pursuer]
		{
			pursuer.setEvade(!pursuer.getEvade()); // toggle mode
			return RC_SUCCESS;
		};

		// init text renderer
		{
			auto& tx = button.text;
			//tx.text = "Seek Mode";
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
			for (auto& corner : e.corners)
			{
				DO_ANYALL(corner.update());
			}
			DO_ANYALL(e.mover.update());

			e.pursuer.target.position = e.mover.transform.localPosition;
			e.pursuer.target.velocity = e.mover.getVelocity();
			DO_ANYALL(e.pursuer.update());

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
				vec2 const offset = vec2(buttonHalfSize.x * 2, 0);
				e.buttonMode.button.transform.localPosition = vec3(menuButtonPos + offset, 0);
			}
			DO_ANYALL(e.buttonMode.update());
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
			DO_ANYALL(e.mover.render());
			DO_ANYALL(e.pursuer.render());

			e.targetPos.transform.localPosition = e.pursuer.seeker.targetPosition;
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
			e.buttonMode.button.text.text = e.pursuer.getEvade() ? "Pursue [Evade] (E)" : "[Pursue] Evade (E)";
			DO_ANYALL(e.buttonMode.render());

			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



#pragma endregion ~Scene
}
