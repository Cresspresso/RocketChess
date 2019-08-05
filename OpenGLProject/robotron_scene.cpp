
#include "entity_utils.hpp"

#include "singleton.hpp"
#include "screen.hpp"
#include "input.hpp"

#include "basic_materials_builder.hpp"
#include "basic_scene_builder.hpp"

#include "robotron_scene.hpp"



namespace robotron_scene
{
#pragma region Scene::Materials::Builder



	struct Scene::Materials::Builder : BasicMaterialsBuilder
	{
		Materials& materials;

		Builder(Materials& materials);

		void build() const;
	private:
		void initCorner(RimLightingMaterial& material) const;
		void initGround(PhongMaterial& material) const;
		void initPlayer(PhongMaterial& material) const;
		void initEnemy(PhongMaterial& material) const;
		void initTextLabel(TextMaterial& material) const;
		void initTextGameOver(TextMaterial& material) const;
	};



	Scene::Materials::Builder::Builder(Materials& materials) :
		materials{ materials }
	{}



	void Scene::Materials::Builder::build() const
	{
		BasicMaterialsBuilder::build(materials);

		initCorner(materials.corner);
		initGround(materials.ground);
		initPlayer(materials.player);
		initEnemy(materials.enemy);
		initTextLabel(materials.textLabel);
		initTextGameOver(materials.textGameOver);
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

	void Scene::Materials::Builder::initGround(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::AwesomeFace];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 0.95f, 1);
		material.lightSpecularStrength = 1;
		material.shininess = 256;
	}

	void Scene::Materials::Builder::initPlayer(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::Rayman];
		material.ambientColor = vec3(0.05f);
		material.lightColor = vec3(0.9f, 1, 1);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initEnemy(PhongMaterial& material) const
	{
		material.tex0 = resources.textures[TextureIndexer::CapguyWalk];
		material.ambientColor = vec3(0.15f, 0.05f, 0.05f);
		material.lightColor = vec3(1, 0.5f, 0.5f);
		material.lightSpecularStrength = 0.5f;
		material.shininess = 64;
	}

	void Scene::Materials::Builder::initTextLabel(TextMaterial& material) const
	{
		material.tint = vec3(0.6f, 0.9f, 0.6f);
	}

	void Scene::Materials::Builder::initTextGameOver(TextMaterial& material) const
	{
		material.tint = vec3(1.0f, 0.3f, 0.3f);
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
		void initGround(SimpleEntity& entity) const;
		void initPlayer(Player& entity) const;
		void initEnemy(SeekerEnemy& entity) const;

		// hud
		void initTextScore(TextRenderer& entity) const;
		void initTextLives(TextRenderer& entity) const;
		void initTextGameOver(TextRenderer& entity) const;
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
		initGround(entities.ground);
		initPlayer(entities.player);
		initEnemy(entities.enemy);

		initTextScore(entities.textScore);
		initTextLives(entities.textLives);
		initTextGameOver(entities.textGameOver);
	}



	void Scene::Builder::initCorners(std::array<SimpleEntity, 4>& corners) const
	{
		constexpr float a = 5.5f;
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

	void Scene::Builder::initGround(SimpleEntity& entity) const
	{
		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Quad]);
		r.material = &(materials.ground);

		auto & t = entity.transform;
		t.localScale = vec3(10, 10, 1);
		t.localRotation = quat(glm::radians(vec3(-90, 0, 0)));
		t.localPosition = vec3(0, -0.5f, 0);
	}

	namespace
	{
		static vec3 const groundHalfSize = vec3(5, 0, 5);
	}

	void Scene::Builder::initPlayer(Player& entity) const
	{
		auto& pl = entity.positionLimits;
		pl.min = -groundHalfSize;
		pl.max = groundHalfSize;

		auto& s = entity.seeker;
		s.maxSpeed = 3;
		s.maxForce = 20;
		s.forceScale = 20;

		auto& r = s.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.player);

		auto& t = s.simple.transform;
		t.localPosition = vec3(0, 0, 0);
		t.localScale = vec3(0.5f, 0.5f, 0.6f);
	}

	void Scene::Builder::initEnemy(SeekerEnemy& entity) const
	{
		auto& pl = entity.positionLimits;
		pl.min = -groundHalfSize;
		pl.max = groundHalfSize;

		auto& s = entity.seeker;
		s.maxSpeed = 1;
		s.maxForce = 2;
		s.forceScale = 2;

		auto& r = s.simple.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.enemy);

		auto& t = s.simple.transform;
		t.localPosition = vec3(4, 0, 4);
		t.localScale = vec3(0.4f, 0.4f, 0.6f);
	}



	void Scene::Builder::initTextScore(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.textLabel), &(resources.fonts[FontIndexer::WaltographUI]));
	}

	void Scene::Builder::initTextLives(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.textLabel), &(resources.fonts[FontIndexer::WaltographUI]));
	}

	void Scene::Builder::initTextGameOver(TextRenderer& entity) const
	{
		initTextRenderer(entity, &(materials.textGameOver), &(resources.fonts[FontIndexer::WaltographUI]));
	}



#pragma endregion ~Scene::Builder
#pragma region Scene



	void Scene::init()
	{
		materials.init();
		Builder{ *this }.build();
	}



	void Scene::initBullet(SimpleEntity& entity)
	{
		auto& resources = singleton::getResources();

		auto& r = entity.renderer;
		r.program = resources.programs[ProgramIndexer::BlinnPhong].program;
		r.mesh = &(resources.meshes[MeshIndexer::Sphere]);
		r.material = &(materials.enemy);

		auto& t = entity.transform;
		t.localScale = vec3(0.1f, 0.1f, 0.3f);

		auto const& playerTransform = entities.player.seeker.simple.transform;
		t.localPosition = playerTransform.localPosition;
		t.localRotation = playerTransform.localRotation;
		entity.rigidbody.localVelocity = (playerTransform.localRotation * vec3(0, 0, -1)) * 2.0f;
	}



	ReturnCode Scene::update()
	{
		BEGIN_ANYALL();
		{
			auto& e = entities;

			e.cameraWorld.update();

			if (e.player.lives > 0)
			{
				for (auto& corner : e.corners)
				{
					DO_ANYALL(corner.update());
				}
				DO_ANYALL(e.ground.update());
				DO_ANYALL(e.player.update());
				DO_ANYALL(updateEnemy());
				DO_ANYALL(updatePlayerBullets());
			}
			//~ if (e.player.lives > 0)

			// hud
			updateHud();
			DO_ANYALL(e.buttonMenu.update());
		}
		return END_ANYALL();
	}

	ReturnCode Scene::updateEnemy()
	{
		auto& e = entities;

		BEGIN_ANYALL();

		vec3 const& playerPos = e.player.seeker.simple.transform.localPosition;

		// seek playerPos
		e.enemy.seeker.targetPosition = playerPos;

		DO_ANYALL(e.enemy.update());

		vec3& enemyPos = e.enemy.seeker.simple.transform.localPosition;
		// if colliding with player
		if (toleranceEqual2(playerPos, enemyPos, 0.4f))
		{
			--e.player.lives; // hurt player
			enemyPos = vec3(4, 0, 4); // respawn enemy
		}

		return END_ANYALL();
	}

	ReturnCode Scene::updatePlayerBullets()
	{
		auto& bullets = entities.liveBullets;

		BEGIN_ANYALL();

		// update shoot event
		{
			bool shoot = InputState::DownFirst == getKeyboardState(' ')
				|| InputState::DownFirst == getKeyboardState('e');
			if (shoot)
			{
				// spawn bullet
				auto& bullet = bullets.emplace_back();
				initBullet(bullet);
			}
		}

		// update each bullet entity
		for (auto& bullet : bullets)
		{
			DO_ANYALL(bullet.update()); // TODO fix this
		}

		// destroy out of bounds bullets
		{
			auto const endit = bullets.end();
			auto deadit = std::remove_if(bullets.begin(), endit,
				[this](SimpleEntity& bullet)
			{
				return !bulletLimits.contains(bullet.transform.localPosition);
			});
			/*for (auto it = deadit; it != endit; ++it)
			{
				it->destroy();
			}*/
			bullets.erase(deadit, endit);
		}

		// check for collisions
		{
			std::vector<bool> hitStates(bullets.size()); // all false to start

			for (size_t i = 0; i < bullets.size(); ++i)
			{
				auto& bullet = bullets[i];
				auto& enemyTransform = entities.enemy.seeker.simple.transform;

				float const radius = 0.5f;
				if (toleranceEqual2(
					enemyTransform.localPosition,
					bullet.transform.localPosition,
					radius * radius))
				{
					// bullet hit an enemy
					hitStates[i] = true;
					enemyTransform.localPosition = vec3(4, 0, 4); // respawn enemy
				}
			}

			// erase bullets where hitState is true
			{
				size_t j = 0;
				auto const endit = bullets.end();
				auto deadit = std::remove_if(bullets.begin(), endit,
					[&hitStates, j](SimpleEntity& bullet) mutable
				{
					return hitStates[j++];
				});
				bullets.erase(deadit, endit);
			}
		}

		return END_ANYALL();
	}

	void Scene::updateHud()
	{
		auto& e = entities;

		vec2 const hudHalfSize = e.cameraHud.projection.calculateHalfSize();
		vec3 const buttonHalfSize = 0.5f
			* e.buttonMenu.button.backgroundTransform.localScale
			* e.buttonMenu.scaleHovered;

		vec2 const menuButtonPos = vec2(
			-hudHalfSize.x + buttonHalfSize.x,
			hudHalfSize.y - buttonHalfSize.y
		);
		e.buttonMenu.button.transform.localPosition = vec3(menuButtonPos, 0);

		// Lives Text
		e.textLives.text = "Lives: " + std::to_string(e.player.lives);
		e.textLives.position = vec2(hudHalfSize.x - buttonHalfSize.x, hudHalfSize.y - buttonHalfSize.y);

		// Score Text
		e.textScore.text = "Score: " + std::to_string(e.player.score);
		e.textScore.position = vec2(-hudHalfSize.x, hudHalfSize.y - buttonHalfSize.y * 3);

		// Game Over Text
		e.textGameOver.text = "Game Over";
		e.textGameOver.renderer.modelMatrix = makeTransformMatrix(vec3(-60, 0, 0), quat(), vec3(2, 2, 1));
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
			DO_ANYALL(e.ground.render());
			for (auto& bullet : e.liveBullets)
			{
				DO_ANYALL(bullet.render());
			}
			DO_ANYALL(e.enemy.render());
			DO_ANYALL(e.player.render());
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
			DO_ANYALL(e.textScore.render());
			DO_ANYALL(e.textLives.render());

			if (e.player.lives <= 0)
			{
				DO_ANYALL(e.textGameOver.render());
			}

			DO_ANYALL(e.buttonMenu.render());
		}
		return END_ANYALL();
	}



#pragma endregion ~Scene
}
